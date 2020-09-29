//
// Created by moavl@wincs.cs.bgu.ac.il on 12/01/2020.
//
using namespace std;

#include <string>
#include <map>
#include <vector>
#include "ConnectionHandler.h"
#include "Book.h"

#ifndef BOOST_ECHO_CLIENT_DATABASE_H
#define BOOST_ECHO_CLIENT_DATABASE_H

class DataBase {
private:
    map<string, vector<Book *>> inventory; // key - genre, value - book
    ConnectionHandler *handler{};
    atomic<int> genreId{};
    atomic<int> receiptId{};
    map<string, int> genreIdMap;
    map<int, Frame*> receiptIdMap;
    map<string, string> borrowedFrom;
    vector <string> wishList;
    string userLogged;
    vector<Frame*> frames;

public:
    DataBase(ConnectionHandler *handler);

    DataBase(const DataBase&);

    DataBase &operator=(const DataBase &dataBase);

    virtual ~DataBase();

    void addBook(Book *book);

    int addGenreID(string genre);

    int getReceiptID();

    void addReceiptID(Frame &frame, int id);

    void setUserLogged(string name);

    string getUserLogged();

    int getSubscriptionId(string genre);

    void unsubscribeGenre(string genre);

    Book* getBook(string bookName, string genre);

    void removeBook(string bookName, string genre);

    Frame* getReceiptFrame(int receiptID);

    void addBorrow(string bookName, string borrower);

    string getBorrower(string bookName);

    string getGenreList(string genre);

    void addToWishList(string bookName);

    bool ifWishList (string bookName);

    void addToFrames(Frame *frame);
};

#endif //BOOST_ECHO_CLIENT_DATABASE_H