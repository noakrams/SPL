//
// Created by noa on 12/01/2020.
//

using namespace std;

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include "Protocol.h"

Protocol::Protocol(ConnectionHandler *connectionHandler_, DataBase *dataBase, bool *bye) : connectionHandler(
        connectionHandler_), dataBase(dataBase), bye(bye){}


Protocol::Protocol(const Protocol &aProtocol) : connectionHandler(aProtocol.connectionHandler),
                                                dataBase(aProtocol.dataBase), bye(aProtocol.bye) {}


Protocol &Protocol::operator=(const Protocol &aProtocol) {
    connectionHandler = aProtocol.connectionHandler;
    dataBase = aProtocol.dataBase;
    bye = aProtocol.bye;
    return *this;
}

Protocol::~Protocol() {}


void Protocol::process(std::string &message) {
    cout << message << endl;
    std::vector<std::string> words;
    boost::split(words, message, boost::is_any_of("\n"));
    if (!words.empty()) {
        if (words.at(0) == "CONNECTED") {
            cout << "Login successful" << endl;
        } else if (words.at(0) == "RECEIPT") {
            string id = words.at(1).substr(words.at(1).find_first_of(':') + 1);
            Frame *frame = dataBase->getReceiptFrame(stoi(id));
            dataBase->addToFrames(frame);
            string s = frame->execute();
            if (s == "disconnect") {
                *bye = true;
            } else if (s == "subscribe") {
                FrameSUBSCRIBE *frameSub = (FrameSUBSCRIBE *) frame;
                dataBase->addToFrames(frameSub);
                string genre = frameSub->getGenre();
                cout << "Joined club " + genre.substr(genre.find_first_of(':') + 1) << endl;

            } else {
                FrameUNSUBSCRIBE *frameUnsub = (FrameUNSUBSCRIBE *) frame;
                dataBase->addToFrames(frameUnsub);
                string genre = frameUnsub->getGenre();
                cout << "Exited club " + genre.substr(genre.find_first_of(':') + 1) << endl;
            }
        }
        if (words.at(0) == "MESSAGE") {
            std::vector<std::string> body;
            boost::split(body, words.at(5), boost::is_any_of(" "));
            string genre = words.at(3).substr(words.at(3).find_first_of(':') + 1);

            if (body.size() == 5 && body[3] == "borrow") {
                string bookToBorrow = body.at(4);
                Book *book = dataBase->getBook(bookToBorrow, genre);
                if (book != nullptr) {
                    FrameSEND *toSend = new FrameSEND(genre, dataBase->getUserLogged() + " has " + bookToBorrow);
                    dataBase->addToFrames(toSend);
                    connectionHandler->sendFrameAscii(toSend->toString(), '\0');
                }
            } else if (body.size() == 3 && body[1] == "has") {
                if (dataBase->getBook(body.at(2), genre) == nullptr && dataBase->ifWishList(body.at(2))) {
                    FrameSEND *toSend = new FrameSEND(genre, "Taking " + body.at(2) + " from " + body.at(0));
                    dataBase->addToFrames(toSend);
                    connectionHandler->sendFrameAscii(toSend->toString(), '\0');
                    Book *toAdd = new Book(body.at(2), genre);
                    toAdd->setLastHolder(body.at(0));
                    dataBase->addBook(toAdd);//add to the map
                }
            } else if (body.size() == 4 && body[2] == "from") {
                if (body.at(3) == dataBase->getUserLogged()) {
                    dataBase->addBorrow(body.at(1), body.at(3));
                    dataBase->removeBook(body.at(1), genre);
                }
            } else if (body.size() == 4 && body[0] == "Returning") {
                if (body.at(3) == dataBase->getUserLogged()) {
                    Book *toAdd = new Book(body.at(1), genre);
                    toAdd->setLastHolder(dataBase->getBorrower(body.at(1)));
                    dataBase->addBook(toAdd);
                }
            } else if (body.size() == 2 && body[1] == "status") {
                FrameSEND *toSend = new FrameSEND(genre, dataBase->getGenreList(genre));
                dataBase->addToFrames(toSend);
                connectionHandler->sendFrameAscii(toSend->toString(), '\0');
            }
        }
        if (words.at(0) == "ERROR") {
            *bye = true;
        }
    }
}

bool Protocol::connectCheck(std::string &message) {
    string command = message.substr(0, message.find('\n'));
    if (command == "CONNECTED") {
        return true;
    }
    cout << message << endl;
    return false;
}




