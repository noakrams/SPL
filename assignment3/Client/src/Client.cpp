//
// Created by moavl@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <iostream>
#include <ConnectionHandler.h>
#include <FromKeyboard.h>
#include <thread>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

int main(int argc, char *argv[]) {
    bool bye = false;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    //-----------------------------------------------------------------
    std::string input;
    std::vector<std::string> words;
    getline(std::cin, input);
    boost::split(words, input, boost::is_any_of(" "));
    int pos = words.at(1).find(':');
    string host = words.at(1).substr(0, pos);
    string port = words.at(1).substr(pos + 1);
    string username = words.at(2);
    string password = words.at(3);
    FrameCONNECT *frame = new FrameCONNECT(host, username, password);
    ConnectionHandler *handler = new ConnectionHandler(host, stoi(port));
    DataBase *dataBase = new DataBase(handler);
    dataBase->setUserLogged(username);
    cout << frame->toString() << endl;
    Protocol *protocol = new Protocol(handler, dataBase, &bye);
    if (!handler->connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    handler->sendFrameAscii(frame->toString(), '\0');//changed
    //--------------------------------------------------------------------
    string message = "";
    handler->getFrameAscii(message, '\0');
    protocol->process(message);
    bool connect = protocol->connectCheck(message);

    while (!bye && connect) {
        FromKeyboard fromKeyboard = FromKeyboard(handler, protocol, dataBase, &bye);
        std::thread keyboardThread(&FromKeyboard::run, fromKeyboard);
        while (!bye) {
            string message = "";
            handler->getFrameAscii(message, '\0');
            protocol->process(message);
        }
        keyboardThread.join();
    }
    delete frame;
    delete handler;
    delete protocol;
    delete dataBase;
    return 0;
}







