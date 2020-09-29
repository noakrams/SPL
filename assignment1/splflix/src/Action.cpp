//
// Created by noa on 20/11/2019.
//
#include "../include/Action.h"
#include "../include/User.h"
#include <string>
#include <iostream>
#include <unordered_map>



//constructor
BaseAction::BaseAction(): errorMsg(), status(PENDING) {}


//copy constructor
BaseAction::BaseAction(const BaseAction& other): errorMsg(other.errorMsg),
                                                 status(other.status) {
}

// Move Constructor
BaseAction::BaseAction(BaseAction&& other):errorMsg(other.errorMsg),
                                           status(other.status) {
}

// Destructor
BaseAction::~BaseAction() {
}


ActionStatus BaseAction::getStatus() const {
    return status;
}

std::string BaseAction::actionStatusToString(ActionStatus status) const {
    std::string str = "";
    if (status==COMPLETED)
        str = "Completed";
    if (status==ERROR)
        str = "Error: " + getErrorMsg();
    if (status==PENDING)
        str = "Pending";
    return str;
}


void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<<"Error: " + errorMsg<<std::endl;
}


std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}


//----------------Base Action------------------

//-----Create User-----

//constructor
CreateUser::CreateUser(std::string name, std::string ra): BaseAction(),
                                                          name(name), ra(ra)
{}

//copy constructor
CreateUser::CreateUser(const CreateUser &other) : BaseAction(other), name(other.name), ra(other.ra) {

}
//move constructor
CreateUser::CreateUser(CreateUser &&other) : BaseAction(other), name(other.name), ra(other.ra) {

}

//destructor
CreateUser::~CreateUser () {

}

void CreateUser::act(Session &sess) {

    if (sess.getUserMap().find(name) != sess.getUserMap().end())
    {
        error("there is already a user with that name");
    } else if (ra != "len" && ra != "rer" && ra != "gen")
    {
        error("illegal input for recommendation algorithem");
    }
    else {
        if (ra == "len") {
            sess.addToUserMap(new LengthRecommenderUser(name));
            ra = "Length";

        }  if (ra == "rer") {
            sess.addToUserMap(new RerunRecommenderUser(name));
            ra = "Rerun";

        }  if (ra == "gen") {
            sess.addToUserMap(new GenreRecommenderUser(name));;
            ra = "Genre";

        }
        complete();
    }
}

std::string CreateUser::toString() const {
    return "Create new user " + name + " with " + ra + " recommendation " + actionStatusToString(getStatus());
}

BaseAction *CreateUser::clone() {
    return new CreateUser(*this);
}


//-----Change Active User-----

//constructor
ChangeActiveUser::ChangeActiveUser(std::string changeTo) : BaseAction(), changeTo(changeTo)
{

}

//copy constructor
ChangeActiveUser::ChangeActiveUser(const ChangeActiveUser &other): BaseAction(other), changeTo(other.changeTo) {

}

//move constructor
ChangeActiveUser::ChangeActiveUser(ChangeActiveUser && other): BaseAction(other), changeTo(other.changeTo) {}

//destructor
ChangeActiveUser::~ChangeActiveUser() {
}



void ChangeActiveUser::act(Session &sess) {

    if (sess.getUserMap().find(changeTo) == sess.getUserMap().end()) {
        error("User " + changeTo + " doesn't exist");
    }
    else {
        sess.setActiveUser(sess.getUser(changeTo));
    }
    complete();
}

std::string ChangeActiveUser::toString() const {
    return "Changed active user to " + changeTo + " " + actionStatusToString(getStatus()) ;
}

BaseAction *ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}

// -----DeleteUser-----


//constructor
DeleteUser::DeleteUser(std::string deleteUser) : BaseAction(), deleteUser(deleteUser){//constructor

}

//copy constructor
DeleteUser::DeleteUser(const DeleteUser &other): BaseAction(other), deleteUser(other.deleteUser) {

}

//move constructor
DeleteUser::DeleteUser(DeleteUser &&other): BaseAction(other), deleteUser(other.deleteUser)  {

}

//destructor
DeleteUser::~DeleteUser() {
}


void DeleteUser::act(Session &sess) {

    if (sess.getUserMap().find(deleteUser) == sess.getUserMap().end()) {
        error(" User " + deleteUser + " doesn't exist");
    }
    else
    {
        sess.deleteUserFromMap(deleteUser);
    }
    complete();
}

std::string DeleteUser::toString() const {
    return "User " + deleteUser + " delete " + actionStatusToString(getStatus());
}

BaseAction *DeleteUser::clone() {
    return new DeleteUser(*this);
}

//-----Duplicte User-----

DuplicateUser::DuplicateUser(std::string dupUser, std::string newUser) : BaseAction(), dupUser(dupUser), newUser(newUser) {

}

//copy constructor
DuplicateUser::DuplicateUser(const DuplicateUser &other): BaseAction(other), dupUser(other.dupUser), newUser(other.newUser) {}



//move constructor
DuplicateUser::DuplicateUser(DuplicateUser &&other): BaseAction(other), dupUser(other.dupUser), newUser(other.newUser) {}

//destructor
DuplicateUser::~DuplicateUser() {}


void DuplicateUser::act(Session &sess) {
    if ((sess.getUserMap().find(newUser) != sess.getUserMap().end()) || sess.getUserMap().find(dupUser) == sess.getUserMap().end()) {
        error("Name " + newUser + " is already taken or user " + dupUser + " doesn't exist");
    }
    else
    {
        User * copied = sess.getUser(dupUser)->clone();
        for(unsigned int i=0; i<sess.getUser(dupUser)->get_history().size(); i++) {
            copied->get_history().push_back(sess.getUser(dupUser)->get_history()[i]);
            copied->upDate(sess.getUser(dupUser)->get_history()[i]);
        }
        copied->setName(newUser);
        sess.addToUserMap(copied);
    }
    complete();
}


std::string DuplicateUser::toString() const {
    return "Created a copy of " + dupUser + "'s user with the name " + newUser + " " + actionStatusToString(getStatus()) ;
}

BaseAction *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}


//-----Print Content List-----

//constructor
PrintContentList::PrintContentList():BaseAction() {}

//copy constructor
PrintContentList::PrintContentList(const PrintContentList &other): BaseAction(other) {}

// Move Constructor
PrintContentList::PrintContentList(PrintContentList&& other): BaseAction(other) {}

// Destructor
PrintContentList::~PrintContentList() {}


void PrintContentList::act(Session &sess) {
    std::vector<Watchable*> content = sess.getContent();
    for (size_t i = 0; i < content.size() ; ++i) {
        std::cout << content[i]->toString() << std::endl;
    }
    complete();
}

std::string PrintContentList::toString() const {
    return "Print content list " + actionStatusToString(getStatus());
}

BaseAction *PrintContentList::clone() {
    return new PrintContentList(*this);
}



//-----PrintWatchHistory-----

//constructor
PrintWatchHistory::PrintWatchHistory():BaseAction() {}

//copy constructor
PrintWatchHistory::PrintWatchHistory(const PrintWatchHistory &other): BaseAction(other) {}

// Move Constructor
PrintWatchHistory::PrintWatchHistory(PrintContentList&& other): BaseAction(other) {}

// Destructor
PrintWatchHistory::~PrintWatchHistory() {}


void PrintWatchHistory::act(Session &sess) {
    std::vector<Watchable*> history = sess.getUser(sess.getActiveUser()->getName())->get_history();
    std::cout << "Watch History for " + sess.getActiveUser()->getName()<< std::endl;
    for (size_t i = 0; i < history.size() ; ++i) {
        std::cout <<history[i]->toStringWatch()<< std::endl;
    }

    complete();
}

std::string PrintWatchHistory::toString() const {
    return "Print watch history " + actionStatusToString(getStatus());
}

BaseAction *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}


//-----Watch-----

//constructor
Watch::Watch(const long id) : BaseAction(), id(id){
}

//copy constructor
Watch::Watch(const Watch &other): BaseAction(other), id(other.id) {}

//move constructor
Watch::Watch(Watch &&other): BaseAction(other), id(other.id) {}

//destructor
Watch::~Watch() {
}

void Watch::act(Session &sess) {
    Watchable *toWatch = sess.getContent()[id -1];      //( id - 1 ) because the vector starts from 0
    std::string ans;
    while ((toWatch!= nullptr) && (ans!="n")) {
        std::cout << "Watching " + toWatch->toStringWatch() << std::endl;
        sess.getActiveUser()->upDate(sess.getContent()[toWatch->getId() - 1]);          //Updating the relevant fields
        sess.getActiveUser()->addToHistory(sess.getContent()[toWatch->getId() - 1]);  //insert the watchable to the watch history
        toWatch = toWatch->getNextWatchable(sess);
        if (toWatch!= nullptr) {
            std::cout << "We recommend watching " + toWatch->toStringWatch() +
                         " Continue Watching[y/n]" << std::endl;
            std::cin >> ans;
        }
    }
    complete();
}


std::string Watch::toString() const {
    return "Watch " + actionStatusToString(getStatus());
}

BaseAction *Watch::clone() {
    return new Watch(*this);
}


//---------------PrintActionsLog-------------------------

//constructor
PrintActionsLog::PrintActionsLog():BaseAction() {}

//copy constructor
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other): BaseAction(other) {}

//move constructor
PrintActionsLog::PrintActionsLog(PrintActionsLog &&other): BaseAction(other){}

//destructor
PrintActionsLog::~PrintActionsLog() {}

void PrintActionsLog::act(Session &sess) {
    std::vector<BaseAction*> logs = sess.getActionsLog();
    for (size_t i = 0; i < logs.size() ; ++i) {
        std::cout << logs[i]->toString() << std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log " + actionStatusToString(getStatus());
}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

//-----Exit-----


Exit::Exit() : BaseAction(){}


BaseAction *Exit::clone() {
    return new Exit(*this);
}

void Exit::act(Session &sess) {
    complete();
}

std::string Exit::toString() const {
    return "exit " + actionStatusToString(getStatus());
}
