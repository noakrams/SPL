//
// Created by moavl@wincs.cs.bgu.ac.il on 12/01/2020.
//
using namespace std;
#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H

#include <vector>
#include <string>

class Book {
private:
    bool available;
    std::string name;
    std::string owner;
    string genre;
    string lastHolder;
public:
    Book(string name, string genre);

    Book(const Book& book);

    Book& operator=(const Book& book);

    virtual ~Book();

    void setOwner(string owner);

    string getName();

    string getGenre();

    string getLastHolder();

    void setLastHolder(string lastHolder);
};

#endif //BOOST_ECHO_CLIENT_BOOK_H