#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>

class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags); //constructor
    Watchable(const Watchable &other); //copy constructor
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual std::string toStringWatch() const=0;
    long getId() const;
    int getLength() const;
    const std::vector<std::string> &getTags() const;
    virtual Watchable *clone()=0;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags); //constructor
    Movie(const Movie &other); //copy constructor
    ~Movie();//destructor
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string toStringWatch() const;
    Watchable *clone();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string &seriesName, int length, int season, int episode, const std::vector<std::string> &tags, long nextEpisodeId);
    Episode(const Episode &other); //copy constructor
    ~Episode();//destructor
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string toStringWatch() const;
    void setNextId(long id);
    Watchable *clone();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif