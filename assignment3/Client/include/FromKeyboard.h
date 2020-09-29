
#ifndef BOOST_ECHO_CLIENT_FROMKEYBOARD_H
#define BOOST_ECHO_CLIENT_FROMKEYBOARD_H

#include <condition_variable>
#include "ConnectionHandler.h"
#include "Protocol.h"

class FromKeyboard {
private:
    ConnectionHandler *handler;
    Protocol *protocol;
    DataBase *dataBase;
    bool *bye;
public:

    FromKeyboard(ConnectionHandler *handler, Protocol *aProtocol, DataBase *dataBase1, bool *bye);

    void run();

};

#endif //BOOST_ECHO_CLIENT_FROMKEYBOARD_H