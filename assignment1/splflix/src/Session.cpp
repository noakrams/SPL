//
// Created by noa on 20/11/2019.
//
#include <fstream>
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include "../include/User.h"
#include <unordered_map>
using json = nlohmann::json;
using namespace std;


//constructor
Session::Session(const std::string &configFilePath):
    content(), actionsLog(), userMap(), activeUser() {


    auto *def = new LengthRecommenderUser("default");
    userMap["default"] = def;
    this->setActiveUser(def);


    std::fstream i(configFilePath);
    nlohmann::json buffer = nlohmann::json::parse(i);
    nlohmann::json movies = buffer["movies"];
    nlohmann::json tv_series = buffer["tv_series"];
    int id = 1;


    for (auto &m : movies.items()) {
        nlohmann::json aMovie = m.value();
        int length = aMovie["length"];
        std::vector<std::string> tags;
        for (auto &t : aMovie["tags"]) {
            std::string tag = t;
            tags.push_back((tag));
        }
        Movie *to_add = new Movie(id, (std::string) aMovie["name"], length, tags);
        this->content.push_back(to_add);
        id = id + 1;
    }

    for (auto &s : tv_series.items()) {
        nlohmann::json aShow = s.value();
        int length = aShow["episode_length"];
        std::vector<std::string> tags;
        for (auto &t: aShow["tags"]) {
            std::string tag = t;
            tags.push_back((tag));
        }
        unsigned int s_count = 1;
        for (auto &season : aShow["seasons"]) {
            for (unsigned int i = 1; i <= season; i = i + 1) {
                Episode *to_add = new Episode(id, (std::string) aShow["name"], length, s_count, i, tags, id + 1);
                this->content.push_back(to_add);
                if (s_count == aShow.size() && i == season) {
                    to_add->setNextId(-1);
                }
                id = id + 1;
            }
            s_count = s_count + 1;
        }
    }
}


//copy constructor
Session::Session(const Session &other): content(), actionsLog(), userMap(), activeUser() {
    //copy other content
    for(size_t i = 0; i < other.getContent().size(); i++ ) {
        content.push_back(other.getContent()[i]->clone());
    }

    //copy other actions log
    for (size_t j = 0; j < other.getActionsLog().size(); ++j) {
        actionsLog.push_back(other.getActionsLog()[j]->clone());
    }

    //copy other user map and their history
    for(std::pair <std::string, User *> curr: other.userMap){
        userMap[curr.first]=curr.second->clone();
        std::vector<Watchable*> newHistory = copyHistory(curr.second->get_history());
        userMap[curr.first]->setHistory(newHistory);
    }
    activeUser = userMap[other.activeUser->getName()]; //update active user
}


//move constructor
Session::Session(Session &&other): content(other.getContent()), actionsLog(other.getActionsLog()), userMap(other.userMap), activeUser(other.activeUser) {

    //clean other user map
    for(pair<string, User *> curr: other.userMap) {
        curr.second = nullptr;
    }

    //clear other content
    for(size_t i = 0; i < other.getContent().size(); i++ ) {
        other.content[i] = nullptr;
    }

    //clear other actions log
    for (size_t j = 0; j < other.getActionsLog().size(); ++j) {
        other.actionsLog[j] = nullptr;
    }
}

//copy assignment operator
Session &Session::operator=(const Session &other) {
    if (this == &other) {
        return *this;
    }

    //clear this content
    for(unsigned int i=0; i < content.size(); i++) {
        delete(content[i]);
        content[i] = nullptr;
    }
    content.clear();

    //clear this actions log
    for(unsigned int j = 0; j < actionsLog.size(); j++) {
        delete(actionsLog[j]);
        actionsLog[j] = nullptr;
    }
    actionsLog.clear();

    //clear this user map
    for(pair<string, User *> curr: userMap) {
        delete(curr.second);
        curr.second = nullptr;
    }
    userMap.clear();
    activeUser = nullptr; //clear active user

    //copy other actions log
    for(unsigned int k = 0; k < other.getActionsLog().size(); k++) {
        BaseAction* action = other.getActionsLog()[k]->clone();
        this->actionsLog.push_back(action);
    }

    //copy other content
    for(unsigned int l = 0; l < other.getContent().size(); l++) {
        Watchable* watch = other.getContent()[l]->clone();
        this->content.push_back(watch);
    }

    //copy other users and their history
    for(pair<string, User *> curr: other.userMap) {
        User *user = curr.second->clone();
        userMap[curr.first] = user;
        std::vector<Watchable*> newHistory = copyHistory(curr.second->get_history());
        userMap[curr.first]->setHistory(newHistory);
    }
    activeUser = userMap[other.activeUser->getName()]; //update active user
    return *this;
}

//move assignment operator
Session &Session::operator=(Session &&other) {
    if(this == &other) {
        return *this;
    }
    //clear this content
    for(unsigned int i=0; i < content.size(); i++) {
        delete(content[i]);
        content[i] = nullptr;
    }
    content.clear();

    //clear this actions log
    for(unsigned int j = 0; j < actionsLog.size(); j++) {
        delete(actionsLog[j]);
        actionsLog[j] = nullptr;
    }
    actionsLog.clear();

    //clear this user map and its history
    for(pair<string, User *> curr: userMap) {
        delete(curr.second);
        curr.second = nullptr;
    }
    userMap.clear();

    activeUser = nullptr; //clearing active user

    //copy other content
    for(unsigned int i=0; i<other.content.size(); i++) {
        content.push_back(other.content[i]);
    }

    //copy other actions log
    for(unsigned int i=0; i<other.actionsLog.size(); i++) {
        actionsLog.push_back(other.actionsLog[i]);
    }

    //copy other user map
    for(pair<string, User *> curr: other.userMap) {
        userMap[curr.first] = curr.second;
    }

    activeUser = other.activeUser; //copy active user

    //cleaning other's content
    other.content.clear();
    //cleaning other's actions log
    other.actionsLog.clear();
    //cleaning other's user map
    other.userMap.clear();
    //cleaning other's active user
    other.activeUser = nullptr;

    return *this;
}


//destructor
Session::~Session() {
    clear();
}


//clear function for destructor
void Session::clear() {
    //clear content
    if(!content.empty()) {
        for (size_t i = 0; i < content.size(); i++) {
            if(content[i] != nullptr) {
                delete (content[i]);
            }
        }
    }
    content.clear();
    //clear actions log
    if(!actionsLog.empty()) {
        for (size_t i = 0; i < actionsLog.size(); i++) {
            if(actionsLog[i] != nullptr) {
                delete (actionsLog[i]);
            }
        }
    }
    actionsLog.clear();

    //clear user map and their history
    if(!userMap.empty()) {
        for (pair<string, User *> curr: userMap) {
            if(curr.second != nullptr) {
                delete (curr.second);
            }
        }
    }
    userMap.clear();
    activeUser = nullptr;
}


void Session::start() {
    cout << "SPLFLIX in now on!" << endl;
    string nextAction;
    while(nextAction != "exit") {
        cin >> nextAction;
        if(nextAction == "createuser"){
            string userName;
            string recAlgo;
            cin >> userName;
            cin >> recAlgo;
            CreateUser * actCreateUser = new CreateUser(userName, recAlgo);
            actCreateUser->act(*this);
            actionsLog.push_back(actCreateUser);
        }
        else if(nextAction == "changeuser"){
            string userName;
            cin >> userName;
            ChangeActiveUser *actChangeUser = new ChangeActiveUser(userName);
            actChangeUser->act(*this);
            actionsLog.push_back(actChangeUser);
        }
        else if(nextAction == "deleteuser"){
            string userName;
            cin >> userName;
            DeleteUser *actDeleteUser = new DeleteUser(userName);
            actDeleteUser->act(*this);
            actionsLog.push_back(actDeleteUser);
        }
        else if(nextAction == "dupuser"){
            string original;
            string newUserName;
            cin >> original;
            cin >> newUserName;
            DuplicateUser *actDuplicateUser = new DuplicateUser(original, newUserName);
            actDuplicateUser->act(*this);
            actionsLog.push_back(actDuplicateUser);
        }
        else if(nextAction == "content"){
            PrintContentList *actPrintContent = new PrintContentList;
            actPrintContent->act(*this);
            actionsLog.push_back(actPrintContent);
        }
        else if(nextAction == "watchhist"){
            PrintWatchHistory *actPrintHistory = new PrintWatchHistory;
            actPrintHistory->act(*this);
            actionsLog.push_back(actPrintHistory);
        }
        else if(nextAction == "watch"){
            string id;
            cin >> id;
            std::string::size_type sz;
            Watch * actWatch = new Watch( stol(id,&sz));
            actWatch->act(*this);
            actionsLog.push_back(actWatch);
        }
        else if(nextAction == "log"){
            PrintActionsLog *actPrintLog = new PrintActionsLog;
            actPrintLog->act(*this);
            actionsLog.push_back(actPrintLog);
        }
        else if(nextAction == "print")
        {
            std::unordered_map<std::string,User*>::iterator it = userMap.begin();
            while(it != userMap.end())
            {
                std::cout<< it->first <<std::endl;
                it++;
            }
        }
        else if(nextAction == "exit"){
            Exit *actExit = new Exit;
            actExit->act(*this);
            actionsLog.push_back(actExit);
        }
    }
}


 const std::unordered_map<std::string, User *> &Session::getUserMap() const {
    return userMap;
}

User *&Session::getUser(std:: string name) {
    return userMap[name];
}

User *Session::getActiveUser() {
    return activeUser;
}

const std::vector<BaseAction *> &Session::getActionsLog() const {
    return actionsLog;
}

const std::vector<Watchable *> &Session::getContent() const {
    return content;
}

void Session::setActiveUser(User * changeTo ) {
    activeUser = changeTo;
}

void Session::addToUserMap(User * addUser) {
    userMap[addUser->getName()] = addUser;
}

void Session::deleteUserFromMap(std::string toDelete) {
    delete (userMap[toDelete]);
    userMap.erase(toDelete);

}


std::vector<Watchable *> Session::copyHistory(std::vector<Watchable *> oldHistory) {
    std::vector<Watchable *> newHistory;
    for(Watchable * watch: oldHistory) {
        int id=watch->getId();
        newHistory.push_back(content.at(id));
    }
    return newHistory;
}


