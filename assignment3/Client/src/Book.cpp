//
// Created by moavl@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <Book.h>

Book::Book(string genre, string name) : available(true), name(name), owner(), genre(genre),lastHolder("-1")  {}


Book::Book(const Book &book): available(book.available),name(book.name), owner(book.owner),genre(book.genre), lastHolder(book.lastHolder) {}


Book &Book::operator=(const Book &book) {
    available = book.available;
    name = book.name;
    owner = book.owner;
    genre = book.genre;
    lastHolder = book.lastHolder;
    return *this;
}


Book::~Book() = default;

void Book::setOwner(string owner) {
    this->owner = owner;
}

string Book::getName() {
    return name;
}

string Book::getGenre() {
    return genre;
}

string Book::getLastHolder() {
    return lastHolder;
}

void Book::setLastHolder(string lastHolder) {
    this->lastHolder = lastHolder;
}
