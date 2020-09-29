#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"


class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session( const Session &other);
    Session(Session &&other);
    Session& operator=(const Session &other);
    Session& operator=( Session &&other);
    ~Session();
    void clear();
    void start();
    const std::unordered_map<std::string,User*>& getUserMap() const;
    User *&getUser(std::string name);
    User* getActiveUser();
    const std::vector<BaseAction*>& getActionsLog() const;
    const std::vector<Watchable *> &getContent() const;
    void setActiveUser(User*);
    void addToUserMap (User*);
    void deleteUserFromMap (std::string toDelete);
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::vector<Watchable *> copyHistory(std::vector<Watchable *> prevHistory);
};
#endif