//
// Created by moavl@wincs.cs.bgu.ac.il on 13/01/2020.
//

#include <Frame.h>

Frame::Frame() = default;

Frame::Frame(const Frame &other) = default;

Frame &Frame::operator=(const Frame &frame) = default;

Frame::~Frame() = default;



//----------FrameConnect----------
FrameCONNECT::FrameCONNECT(string host, string name, string password) : host("host:" + host), name("login:" + name),
                                                                        password("passcode:" + password) {}

FrameCONNECT::FrameCONNECT(const FrameCONNECT &frameConnect): host(frameConnect.host), name(frameConnect.name), password(frameConnect.password) {}

FrameCONNECT &FrameCONNECT::operator=(const FrameCONNECT &frameConnect) {
    host = frameConnect.host;
    name = frameConnect.name;
    password = frameConnect.password;
    return *this;
}

FrameCONNECT::~FrameCONNECT() = default;

string FrameCONNECT::toString() {
    return ("CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\n" + name + "\n" + password + "\n\n");
}

string FrameCONNECT::execute() {
    return "connect";
}


//-------------FrameDisconnect------------
FrameDISCONNECT::FrameDISCONNECT(string receipt) : receipt("receipt:" + receipt) {}


FrameDISCONNECT::FrameDISCONNECT(const FrameDISCONNECT &frameDisconnect): receipt(frameDisconnect.receipt) {
}

FrameDISCONNECT &FrameDISCONNECT::operator=(const FrameDISCONNECT &frameDisconnect) {
    receipt = frameDisconnect.receipt;
    return *this;
}

FrameDISCONNECT::~FrameDISCONNECT() = default;

string FrameDISCONNECT::toString() {
    return ("DISCONNECT\n" + receipt + "\n\n");
}

string FrameDISCONNECT::execute() {
    return "disconnect";
}


//---------------FrameSend-------------
FrameSEND::FrameSEND(string destination, string body) : destination("destination:" + destination),
                                                        body(body), user(),bookName() {}

FrameSEND::FrameSEND(const FrameSEND &frameSend): destination(frameSend.destination), body(frameSend.body),
                                                    user(frameSend.user), bookName(frameSend.bookName) {}

FrameSEND &FrameSEND::operator=(const FrameSEND &frameSend) {
    user = frameSend.user;
    bookName = frameSend.bookName;
    destination = frameSend.destination;
    body = frameSend.body;
    return *this;
}

FrameSEND::~FrameSEND() = default;

string FrameSEND::toString() {
    return ("SEND\n" + destination + "\n\n" + body + "\n");
}


string FrameSEND::execute() {
    return "send";
}

string FrameSEND::getUser() {
    return user;
}

string FrameSEND::getBookName() {
    return bookName;
}


//----------------FrameUnSubscribe------------

FrameUNSUBSCRIBE::FrameUNSUBSCRIBE(string id, string genre) : id("id:" + id), genre(genre) {}

FrameUNSUBSCRIBE::FrameUNSUBSCRIBE(const FrameUNSUBSCRIBE &frameUnsubscribe): id(frameUnsubscribe.id), genre(frameUnsubscribe.genre) {}

FrameUNSUBSCRIBE &FrameUNSUBSCRIBE::operator=(const FrameUNSUBSCRIBE &frameUnsubscribe) {
    id = id = frameUnsubscribe.id;
    genre = frameUnsubscribe.genre;
    return *this;
}


FrameUNSUBSCRIBE::~FrameUNSUBSCRIBE() = default;


string FrameUNSUBSCRIBE::toString() {
    return ("UNSUBSCRIBE\n" + id + "\n\n");
}

string FrameUNSUBSCRIBE::execute() {
    return "unsubscribe";
}

string FrameUNSUBSCRIBE::getGenre() {
    return genre;
}


//----------------------FrameSubscribe------------

FrameSUBSCRIBE::FrameSUBSCRIBE(string destination, string id, string receipt) : destination(
        "destination:" + destination), id("id:" + id), receipt("receipt:" + receipt) {}

FrameSUBSCRIBE::FrameSUBSCRIBE(const FrameSUBSCRIBE &frameSubscribe):
        destination(frameSubscribe.destination), id(frameSubscribe.id), receipt(frameSubscribe.receipt) {}

FrameSUBSCRIBE &FrameSUBSCRIBE::operator=(const FrameSUBSCRIBE &frameSubscribe) {
    destination = frameSubscribe.destination;
    id = frameSubscribe.id;
    receipt = frameSubscribe.receipt;
    return *this;
}

FrameSUBSCRIBE::~FrameSUBSCRIBE() = default;

string FrameSUBSCRIBE::toString() {
    return ("SUBSCRIBE\n" + destination + "\n" + id + "\n" + receipt + "\n\n");
}


string FrameSUBSCRIBE::execute() {
    return "subscribe";
}

string FrameSUBSCRIBE::getGenre() {
    return destination;
}



