//
// Created by moavl@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <DataBase.h>
#include <map>
#include <utility>

using namespace std;

DataBase::DataBase(ConnectionHandler *handler) : inventory(), handler(handler), genreId(0), receiptId(0), genreIdMap(),
                                                 receiptIdMap(), borrowedFrom(), wishList(), userLogged(), frames() {}


DataBase::DataBase(const DataBase &dataBase) : inventory(dataBase.inventory), handler(dataBase.handler),
                                               genreId(dataBase.genreId.load()), receiptId(dataBase.receiptId.load()),
                                               genreIdMap(dataBase.genreIdMap), receiptIdMap(dataBase.receiptIdMap),
                                               borrowedFrom(dataBase.borrowedFrom),
                                               wishList(dataBase.wishList), userLogged(dataBase.userLogged), frames(dataBase.frames) {}

DataBase &DataBase::operator=(const DataBase &dataBase) {
    return *this;
}

DataBase::~DataBase() {
    for (pair<string,vector<Book *>> curr: inventory) {
        for (auto &i : curr.second) {
            delete i;
        }
        curr.second.clear();
    }
    for (pair<const int, Frame *> curr: receiptIdMap) {
        delete curr.second;
    }
    for(auto & frame : frames) {
        delete frame;
    }
    inventory.clear();
    genreIdMap.clear();
    receiptIdMap.clear();
    borrowedFrom.clear();
    wishList.clear();
    frames.clear();
    delete handler;
}


void DataBase::addBook(Book *book) {
    if (inventory.find(book->getGenre()) != inventory.end()) {
        inventory[book->getGenre()].push_back(book);
    } else {
        vector<Book *> books;
        books.push_back(book);
        inventory[book->getGenre()] = books;
    }

}


int DataBase::addGenreID(string genre) {
    if (genreIdMap.find(genre) == genreIdMap.end()) {
        genreId.operator++();
        int a = genreId;
        genreIdMap[genre] = a;
        return a;
    }
    return genreIdMap[genre];
}

int DataBase::getReceiptID() {
    return receiptId.operator++();
}


void DataBase::addReceiptID(Frame &frame, int id) {
    receiptIdMap[id] = &frame;
}


void DataBase::setUserLogged(string name) {
    userLogged = name;
}


string DataBase::getUserLogged() {
    return userLogged;
}

int DataBase::getSubscriptionId(string genre) {
    return genreIdMap[genre];
}

void DataBase::unsubscribeGenre(string genre) {
    genreIdMap.erase(genre);
}

Book *DataBase::getBook(string bookName, string genre) {
    Book *book = nullptr;
    for (auto &i : inventory[genre]) {
        if (i->getName() == bookName) {
            book = i;
            return book;
        }
    }
    return book;
}

void DataBase::removeBook(string bookName, string genre) {
    for (unsigned int i = 0; i < inventory[genre].size(); i++) {
        if (inventory[genre].at(i)->getName() == bookName) {
            inventory[genre].erase(inventory[genre].begin() + i);
        }
    }
    for (unsigned int i = 0; i < wishList.size(); ++i) {
        if (bookName == wishList[i]) {
            wishList.erase(wishList.begin() + i);
        }
    }

}

Frame *DataBase::getReceiptFrame(int receiptID) {
    Frame *frame = nullptr;
    frame = receiptIdMap[receiptID];
    return frame;
}

void DataBase::addBorrow(string bookName, string borrower) {
    borrowedFrom[bookName] = borrower;
}

string DataBase::getBorrower(string bookName) {
    return borrowedFrom[bookName];
}

string DataBase::getGenreList(string genre) {
    string output = "";
    if (inventory.find(genre) != inventory.end()) {
        output = getUserLogged() + ":";
        for (unsigned int i = 0; i < inventory[genre].size(); ++i) {
            if (inventory[genre].size() - 1 == i) {
                output = output + inventory[genre].at(i)->getName();
            } else {
                output = output + inventory[genre].at(i)->getName() + ",";
            }
        }
    }
    return output;
}


void DataBase::addToWishList(string bookName) {
    wishList.push_back(bookName);
}

bool DataBase::ifWishList(string bookName) {
    for (const auto &i : wishList) {
        if (i == bookName) {
            return true;
        }
    }
    return false;
}

void DataBase::addToFrames(Frame *frame) {
    frames.push_back(frame);
}


