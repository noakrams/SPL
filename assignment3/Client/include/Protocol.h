//
// Created by noa on 12/01/2020.
//

#include "ConnectionHandler.h"
#include "DataBase.h"
using namespace std;
#ifndef BOOST_ECHO_CLIENT_PROTOCOL_H
#define BOOST_ECHO_CLIENT_PROTOCOL_H

#endif //BOOST_ECHO_CLIENT_PROTOCOL_H

class Protocol {
private:
    ConnectionHandler *connectionHandler;
    DataBase *dataBase;
    bool *bye;
public:
    Protocol(ConnectionHandler *connectionHandler, DataBase *dataBase, bool *bye);

    Protocol(const Protocol &aProtocol);

    Protocol &operator=(const Protocol &aProtocol);

    virtual ~Protocol();

    void process(std::string &message);

    bool connectCheck(std::string &message);

};