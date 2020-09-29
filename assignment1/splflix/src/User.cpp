//
// Created by moavl on 23/11/2019.
//
#include "../include/User.h"
#include <climits>
#include <map>


//constructor
User::User(const std::string& name): history(), name(name){}

//copy constructor
User::User(const User& other): history(), name(other.name) {
    for(unsigned int i = 0; i < other.history.size(); i++) {
        history.push_back(other.history[i]);
    }
}

//move constructor
User::User(User &&other):  history(), name(other.name){
    for (unsigned int i = 0; i < other.history.size(); ++i){
        history.push_back(other.history[i]);
        other.history.at(i) = nullptr;
    }
    other.history.clear();
}

//copy assignment operator
User &User::operator=(const User &other) {
    if(this == &other) {
        return *this;
    }
    for (unsigned int i=0; i<history.size(); i++){
        history[i] = nullptr;
    }
    history.clear();
    name = other.name;
    for (unsigned int i=0; i<other.history.size(); i++){
        history.push_back(other.history[i]);
    }
    return *this;
}

//move assignment operator
User &User::operator=(User &&other) {
    if (this == &other) {
        return *this;
    }
    for (unsigned int i=0; i<history.size(); i++){
        history[i] = nullptr;
    }
    history.clear();
    for (unsigned int i = 0; i < other.history.size(); ++i){
        history.push_back(other.history[i]);
        other.history[i] = nullptr;
    }
    other.history.clear();
    name = other.name;
    return *this;
}

//destructor
User::~User() {
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();
}

std::string User::getName() const {
    return name;
}
std::vector<Watchable*> User::get_history() const {
    return history;
}

void User::setName(std::string newName) {
    name = newName;
}

void User::setHistory(std::vector<Watchable*> newHistory) {
    history.clear();
    history = newHistory;
}

//-------------Length Recommended User-----------------


//constructor
LengthRecommenderUser::LengthRecommenderUser(const std::string &name):
User(name), beenWatched(0), totalMins(0) {}

//copy constructor
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other):
User(other), beenWatched(other.beenWatched), totalMins(other.totalMins) {}

//move constructor
LengthRecommenderUser::LengthRecommenderUser(LengthRecommenderUser &&other):
User(other), beenWatched(other.beenWatched), totalMins(other.totalMins) {}

//copy assignment operator
LengthRecommenderUser &LengthRecommenderUser::operator=(const LengthRecommenderUser &other){
    if (this == &other) {
        return *this;
    }
    history.clear();
    for(unsigned int i = 0; i<other.history.size(); i++) {
        history.push_back(other.history[i]);
    }
    totalMins = other.totalMins;
    beenWatched = other.beenWatched;
    this->setName(other.getName());
    return *this;
}

//move assignment operator
LengthRecommenderUser &LengthRecommenderUser::operator=(LengthRecommenderUser &&other) {
    if(this == &other) {
        return *this;
    }
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();
    for(unsigned int i = 0; i<other.history.size(); i++) {
        history.push_back(other.history[i]);
        other.history[i] = nullptr;
    }
    other.history.clear();
    totalMins = other.totalMins;
    beenWatched = other.beenWatched;
    this->setName(other.getName());
    return *this;
}

//destructor
LengthRecommenderUser::~LengthRecommenderUser() {
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();
}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    if (history.empty()) {
        return nullptr;
    }
    //gets the closets Watchable to the average
    int avgLen = totalMins / beenWatched;
    int minLen = INT_MAX;
    Watchable *rec = nullptr;
    bool foundInHist;
    for (unsigned int i = 0; i < s.getContent().size(); i++) {
        foundInHist = false;
        for (unsigned int j = 0; j < history.size() && !foundInHist; j++) {
            if (s.getContent()[i]->getId() == history[j]->getId()) {
                foundInHist = true;
            }
        }
        if (!foundInHist && abs(s.getContent()[i]->getLength() - avgLen) < minLen) {
            minLen = abs(s.getContent()[i]->getLength() - avgLen);
            rec = s.getContent()[i];
        }
    }
    return rec;
}


User *LengthRecommenderUser::clone() {
    return new LengthRecommenderUser(*this);
}

void LengthRecommenderUser::upDate(Watchable * curr) {
    beenWatched = beenWatched + 1;
    totalMins = totalMins + (curr->getLength());
}

void LengthRecommenderUser::addToHistory(Watchable *toAdd) {
    history.push_back(toAdd);
}





//---------------Rerun Recommender User---------------

//constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name):
User(name), beenWatched(0), lastRec(-1) {}


//copy constructor
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other):
User(other), beenWatched(other.beenWatched), lastRec(other.lastRec)  {}

//move constructor
RerunRecommenderUser::RerunRecommenderUser(RerunRecommenderUser &&other):
User(other), beenWatched(other.beenWatched), lastRec(other.lastRec) {}

//copy assignment operator
RerunRecommenderUser &RerunRecommenderUser::operator=(const RerunRecommenderUser &other) {
    if(this == &other) {
        return *this;
    }
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();
    for(unsigned int i = 0; i < other.history.size(); i++) {
        history.push_back(other.history[i]);
    }
    setName(other.getName());
    beenWatched = other.beenWatched;
    lastRec = other.lastRec;
    return *this;
}

//move assignment operator
RerunRecommenderUser &RerunRecommenderUser::operator=(RerunRecommenderUser &&other) {
    if(this == &other) {
        return *this;
    }
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();
    for(unsigned int i = 0; i < other.history.size(); i++) {
        history.push_back(other.history[i]);
        other.history[i] = nullptr;
    }
    other.history.clear();
    setName(other.getName());
    beenWatched = other.beenWatched;
    lastRec = other.lastRec;
    return *this;
}

//destructor
RerunRecommenderUser::~RerunRecommenderUser() {
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();
}


Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    Watchable *rer;
    if(history.empty()) {
        rer = nullptr;
    }
    else if(beenWatched == 1) {
        rer = history[0];
        lastRec = 0;
    }
    else {
        int tmp = (lastRec + 1) % beenWatched;
        rer = history[tmp];
        lastRec = tmp;
    }
    return rer;
}

User *RerunRecommenderUser::clone() {
    return new RerunRecommenderUser(*this);
}


void RerunRecommenderUser::upDate(Watchable* curr) {
    beenWatched = beenWatched + 1;
}

void RerunRecommenderUser::addToHistory(Watchable *toAdd) {
    history.push_back(toAdd);
}



//---------------Genre Recommender User--------------


//constructor
GenreRecommenderUser::GenreRecommenderUser(const std::string &name):
User(name), countTags() {}

//copy constructor
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &other):
User(other), countTags() {
    for(std::pair<std::string, int> curr : other.countTags) {
        countTags[curr.first] = curr.second;
    }
}

//move constructor
GenreRecommenderUser::GenreRecommenderUser(GenreRecommenderUser &&other):
User(other), countTags() {
    for(std::pair<std::string, int> curr : other.countTags) {
        countTags[curr.first] = curr.second;
    }
    other.countTags.clear();
}


//copy assignment operator
GenreRecommenderUser &GenreRecommenderUser::operator=(const GenreRecommenderUser &other) {
    if(this == &other) {
        return *this;
    }
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();
    countTags.clear();
    for(unsigned int i = 0; i < other.history.size(); i++) {
        history.push_back(other.history[i]);
    }
    for(std::pair<std::string, int> curr : other.countTags) {
        countTags[curr.first] = curr.second;
    }
    setName(other.getName());
    return *this;
}

//move assignment operator
GenreRecommenderUser &GenreRecommenderUser::operator=(GenreRecommenderUser &&other) {
    if(this == &other) {
        return *this;
    }
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();

    for(unsigned int i = 0; i < other.history.size(); i++) {
        history.push_back(other.history[i]);
        other.history[i] = nullptr;
    }
    other.history.clear();

    for(std::pair<std::string, int> curr : other.countTags) {
        countTags[curr.first] = curr.second;
    }
    other.countTags.clear();
    this->setName(other.getName());
    return *this;
}

//destructor
GenreRecommenderUser::~GenreRecommenderUser() {
    for(unsigned int i = 0; i < history.size(); i++) {
        history[i] = nullptr;
    }
    history.clear();
    countTags.clear();
}


Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    Watchable *gen = nullptr;
    std::map<std::string, int> tempTags = countTags;

    while(gen == nullptr && !tempTags.empty()) {
        int max = -1;
        std::string maxGenre ="";
        std::map<std::string, int>::iterator it;
        for (it = tempTags.begin(); it != tempTags.end(); it++ )
        {
            if (it->second > max) {
                max = it->second;
                maxGenre = it->first;
            }
        }
        for (unsigned int i = 0; i < s.getContent().size() && gen == nullptr; i++) {
            unsigned int j = 0;
            while ( j < s.getContent()[i]->getTags().size() && gen == nullptr) {
                if (s.getContent()[i]->getTags()[j] == maxGenre) {
                    gen = s.getContent()[i];
                    j++;
                    for (unsigned int k = 0; k < history.size() && gen == s.getContent()[i]; k++) {
                        if (gen->getId() == history[k]->getId()) {
                            gen = nullptr;
                            i++;
                            j = 0;
                        }
                    }
                }
                else {
                    j++;
                }
            }
        }
        if (gen == nullptr)
            tempTags.erase(maxGenre);
    }
    return gen;
}


User *GenreRecommenderUser::clone() {
    return new GenreRecommenderUser(*this);
}

void GenreRecommenderUser::upDate(Watchable * curr) {
    for(unsigned int i = 0; i < curr->getTags().size(); i++){
        countTags[curr->getTags()[i]] =  countTags[curr->getTags()[i]] + 1; // Counting the amount of each tag
    }
}

void GenreRecommenderUser::addToHistory(Watchable *toAdd) {
    history.push_back(toAdd);
}



