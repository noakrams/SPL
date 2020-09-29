#include <FromKeyboard.h>
#include <boost/algorithm/string.hpp>

//
// Created by moavl@wincs.cs.bgu.ac.il on 10/01/2020.
//

FromKeyboard::FromKeyboard(ConnectionHandler *handler, Protocol *aProtocol, DataBase *dataBase1, bool *bye) :
        handler(handler), protocol(aProtocol), dataBase(dataBase1), bye(bye) {}


void FromKeyboard::run() {
    while (1) {
        std::string input;
        std::vector<std::string> words;
        getline(std::cin, input);
        boost::split(words, input, boost::is_any_of(" "));
        if (words.at(0) == "join") {
            std::string genre = words.at(1);
            string user = dataBase->getUserLogged();
            int subscriptionId = dataBase->addGenreID(genre);
            int receiptID = dataBase->getReceiptID();
            FrameSUBSCRIBE *frame = new FrameSUBSCRIBE(genre, to_string(subscriptionId), to_string(receiptID));
            dataBase->addToFrames(frame);
            dataBase->addReceiptID(*frame, receiptID);
            handler->sendFrameAscii(frame->toString(), '\0');//changed
        } else if (words.at(0) == "exit") {
            std::string genre = words.at(1);
            int genreId = dataBase->getSubscriptionId(genre);
            FrameUNSUBSCRIBE *frame = new FrameUNSUBSCRIBE(to_string(genreId), genre);
            dataBase->addToFrames(frame);
            handler->sendFrameAscii(frame->toString(), '\0');
            dataBase->addReceiptID(*frame, genreId);
            dataBase->unsubscribeGenre(genre);
        } else if (words.at(0) == "add") {
            std::string genre = words.at(1);
            std::string bookName = words.at(2);
            Book *book = new Book(genre, bookName);
            dataBase->addBook(book);
            book->setOwner(dataBase->getUserLogged());
            FrameSEND *frame = new FrameSEND(genre, dataBase->getUserLogged() + " has added the book " + bookName);
            dataBase->addToFrames(frame);
            handler->sendFrameAscii(frame->toString(), '\0');
        } else if (words.at(0) == "borrow") {
            std::string genre = words.at(1);
            std::string book = words.at(2);
            dataBase->addToWishList(book);
            FrameSEND *frame = new FrameSEND(genre, dataBase->getUserLogged() + " wish to borrow " + book);
            dataBase->addToFrames(frame);
            handler->sendFrameAscii(frame->toString(), '\0');
        } else if (words.at(0) == "return") {
            std::string genre = words.at(1);
            std::string book = words.at(2);
            string lastHolder = dataBase->getBook(book, genre)->getLastHolder();
            if (lastHolder != "-1") {
                FrameSEND *frame = new FrameSEND(genre, "Returning " + book + " to " + lastHolder);
                dataBase->addToFrames(frame);
                handler->sendFrameAscii(frame->toString(), '\0');
                dataBase->removeBook(book, genre);
            }
        } else if (words.at(0) == "status") {
            std::string genre = words.at(1);
            FrameSEND *frame = new FrameSEND(genre, "book status");
            dataBase->addToFrames(frame);
            handler->sendFrameAscii(frame->toString(), '\0');
        } else if (words.at(0) == "logout") {
            int receiptId = dataBase->getReceiptID();
            FrameDISCONNECT *frame = new FrameDISCONNECT(to_string(receiptId));
            dataBase->addToFrames(frame);
            dataBase->addReceiptID(*frame, receiptId);
            handler->sendFrameAscii(frame->toString(), '\0'); //changed
            break;
        }
    }
}
