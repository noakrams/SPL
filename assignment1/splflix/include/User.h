#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "../include/Watchable.h"
#include "../include/Session.h"

class Session;
class Watchable;

class User{
public:
    User(const std::string& name); //constructor
    User(const User &other); //copy constructor
    User(User &&other); //move constructor
    User& operator=(const User &other); //copy assignment operator
    User& operator=(User &&other); //move assignment operator
    virtual ~User(); //destructor
    std::string getName() const;
    std::vector<Watchable *> get_history() const;
    void setName(std::string name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual User* clone() = 0;
    virtual void upDate(Watchable*) = 0;
    virtual void addToHistory (Watchable*) = 0;
    virtual void setHistory(std::vector<Watchable *> newHistory);
protected:
    std::vector<Watchable*> history;
private:
    std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name); //constructor
    LengthRecommenderUser(const LengthRecommenderUser &other); //copy constructor
    LengthRecommenderUser(LengthRecommenderUser &&other); //move constructor
    LengthRecommenderUser& operator=(const LengthRecommenderUser &other); //copy assignment operator
    LengthRecommenderUser& operator=(LengthRecommenderUser &&other); //move assignment operator
    virtual ~LengthRecommenderUser(); //destructor
    virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
    virtual void upDate(Watchable*);
    virtual void addToHistory (Watchable*);
private:
    int beenWatched;
    int totalMins;
};


class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name); //constructor
    RerunRecommenderUser(const RerunRecommenderUser &other); //copy constructor
    RerunRecommenderUser(RerunRecommenderUser &&other); //move constructor
    RerunRecommenderUser& operator=(const RerunRecommenderUser &other); //copy assignment operator
    RerunRecommenderUser& operator=(RerunRecommenderUser &&other); //move assignment operator
    virtual ~RerunRecommenderUser(); //destructor
    virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
    virtual void upDate(Watchable*);
    virtual void addToHistory (Watchable*);
private:
    int beenWatched;
    int lastRec;
};


class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name); //constructor
    GenreRecommenderUser(const GenreRecommenderUser &other); //copy constructor
    GenreRecommenderUser(GenreRecommenderUser &&other); //move constructor
    GenreRecommenderUser& operator=(const GenreRecommenderUser &other); //copy assignment operator
    GenreRecommenderUser& operator=(GenreRecommenderUser &&other); //move assignment operator
    virtual ~GenreRecommenderUser(); //destructor
    virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
    virtual void upDate(Watchable*);
    virtual void addToHistory (Watchable*);
private:
    std::map<std::string, int> countTags;
};

#endif