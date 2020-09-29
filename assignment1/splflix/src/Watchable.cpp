//
// Created by moavl on 23/11/2019.
//

#include "../include/Watchable.h"
#include "../include/User.h"
#include <string>
using namespace std;


//constructor
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags):
        id(id), length(length), tags(tags) {}

//copy constructor
Watchable::Watchable(const Watchable &other): id(other.id), length(other.length) ,tags(other.tags){}


//destructor
Watchable::~Watchable() {
    tags.clear();
}


long Watchable::getId() const {
    return id;
}

int Watchable::getLength() const {
    return length;
}

const std::vector<std::string> &Watchable::getTags() const {
    return tags;
}


//---------------------movie--------------------

//constructor
Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string> &tags):
        Watchable(id, length, tags), name(name) {}

//copy constructor
Movie::Movie(const Movie &other): Watchable(other), name(other.name) {}


//destructor
Movie::~Movie() {}


std::string Movie::toString() const {
    string t = "[";
    for (unsigned int i = 0; i < getTags().size(); i++) {
        if(i < getTags().size()-1) {
            t = t + getTags()[i] + ", ";
        }
        else {
            t = t + getTags()[i];
        }
    }
    t = t + "]";
    string s = to_string(getId()) + ". " + name + " " + to_string(getLength()) + " minutes " + t;
    return s;
}

Watchable *Movie::getNextWatchable(Session &s) const {
    Watchable *next = s.getActiveUser()->getRecommendation(s);
    return next;
}

std::string Movie::toStringWatch() const {
    return name;
}


Watchable *Movie::clone() {
    return new Movie(*this);
}


//--------------------------episode-----------------------------------


//constructor
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags, long nextEpisodeId):
                 Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode),  nextEpisodeId(nextEpisodeId) {}

//copy constructor
Episode::Episode(const Episode &other):
Watchable(other), seriesName(other.seriesName), season(other.season), episode(other.episode), nextEpisodeId(other.nextEpisodeId) {}

//destructor
Episode::~Episode() {}


std::string Episode::toString() const {
    string t = "[";
    for (unsigned int i = 0; i < getTags().size(); i++) {
        if(i < getTags().size()-1)
            t = t + getTags()[i] + ", ";
        else
            t = t + getTags()[i];
    }
    t = t + "]";
    string s = to_string(getId()) + ". " + seriesName + " S" + to_string(season) + "E" + to_string(episode)+ " " + to_string(getLength()) + " minutes " + t;
    return s;
}


Watchable *Episode::getNextWatchable(Session &s) const {
    Watchable *next = nullptr;
    if(nextEpisodeId == -1 || s.getContent()[s.getContent().size() - 1]->getId() == getId())
        next = s.getActiveUser()->getRecommendation(s);
    else
        next = s.getContent()[nextEpisodeId - 1];
    return next;
}

std::string Episode::toStringWatch() const {
    return seriesName + " S" + to_string(season) + "E" + to_string(episode);
}

void Episode::setNextId(long id) {
    nextEpisodeId = id;
}

Watchable *Episode::clone() {
    return new Episode(*this);
}


