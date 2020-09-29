#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include <vector>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    BaseAction(const BaseAction& other);
    BaseAction(BaseAction&& other);
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    std::string actionStatusToString(ActionStatus status) const;
    virtual BaseAction* clone()=0;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    CreateUser(std::string name, std::string ra);
    CreateUser(const CreateUser& other);
    CreateUser(CreateUser&& other);
    ~CreateUser();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
private:
    std::string name;
    std::string ra;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(std::string name);
    ChangeActiveUser(const ChangeActiveUser& other);
    ~ChangeActiveUser();
    ChangeActiveUser(ChangeActiveUser&& other);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();

private:
    std::string changeTo;

};

class DeleteUser : public BaseAction {
public:
    DeleteUser(std::string name);
    DeleteUser(const DeleteUser& other);
    ~DeleteUser();
    DeleteUser(DeleteUser&& other);
    virtual void act(Session & sess);
    virtual std::string toString() const;
    BaseAction* clone();

private:
    std::string deleteUser;
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(std:: string dupUser, std:: string newUser);
    DuplicateUser(const DuplicateUser& other);
    DuplicateUser(DuplicateUser&& other);
    ~DuplicateUser();
    virtual void act(Session & sess);
    virtual std::string toString() const;
    BaseAction* clone();
private:
    std::string dupUser;
    std::string newUser;
};

class PrintContentList : public BaseAction {
public:
    PrintContentList();
    PrintContentList(const PrintContentList& other);
    PrintContentList(PrintContentList&& other);
    ~PrintContentList();
    virtual void act (Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
    PrintWatchHistory(const PrintWatchHistory& other);
    PrintWatchHistory(PrintContentList&& other);
    ~PrintWatchHistory();
    virtual void act (Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};


class Watch : public BaseAction {
public:
    Watch(const long id);
    Watch(const Watch& other);
    ~Watch();
    Watch(Watch&& other);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
private:
    const long id;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog(const PrintActionsLog& other);
    PrintActionsLog(PrintActionsLog&& other);
    ~PrintActionsLog();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();

};

class Exit : public BaseAction {
public:
    Exit();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};
#endif