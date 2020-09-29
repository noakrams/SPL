//
// Created by moavl@wincs.cs.bgu.ac.il on 13/01/2020.
//
using namespace std;
#ifndef BOOST_ECHO_CLIENT_FRAME_H
#define BOOST_ECHO_CLIENT_FRAME_H

#include <string>

#endif //BOOST_ECHO_CLIENT_FRAME_H

class Frame {
private:

public:
    Frame();

    Frame(const Frame &other);

    Frame &operator=(const Frame &frame);

    virtual ~Frame();

    virtual string toString() = 0;

    virtual string execute() = 0;
};


class FrameCONNECT : public Frame {
private:
    string host;
    string name;
    string password;
public:
    FrameCONNECT(string host, string name, string password);

    FrameCONNECT(const FrameCONNECT &frameConnect);

    FrameCONNECT &operator=(const FrameCONNECT &frameConnect);

    virtual ~FrameCONNECT();

    virtual string toString() ;

    virtual string execute() ;
};

class FrameDISCONNECT : public Frame {
private:
    string receipt;
public:
    FrameDISCONNECT(string receipt);

    FrameDISCONNECT(const FrameDISCONNECT &frameDisconnect);

    FrameDISCONNECT &operator=(const FrameDISCONNECT &frameDisconnect);

    virtual ~FrameDISCONNECT() override;

    virtual string toString() ;

    virtual string execute() ;

};


class FrameSEND : public Frame {
private:
    string destination;
    string body;
    string user;
    string bookName;
public:
    FrameSEND(string destination, string body);

    FrameSEND(const FrameSEND &frameSend);

    FrameSEND &operator=(const FrameSEND &frameSend);

    virtual ~FrameSEND() override;

    virtual string toString() ;

    virtual string execute() ;

    string getUser();

    string getBookName();
};

class FrameUNSUBSCRIBE : public Frame {
private:
    string id;
    string genre;
public:
    FrameUNSUBSCRIBE(string id, string genre);

    FrameUNSUBSCRIBE(const FrameUNSUBSCRIBE &frameUnsubscribe);

    FrameUNSUBSCRIBE &operator=(const FrameUNSUBSCRIBE &frameUnsubscribe);

    virtual ~FrameUNSUBSCRIBE() override;

    virtual string toString() ;

    virtual string execute() ;

    string getGenre();
};

using namespace std;

class FrameSUBSCRIBE : public Frame {
private:
    string destination;
    string id;
    string receipt;
public:
    FrameSUBSCRIBE(string destination, string id, string receipt);

    FrameSUBSCRIBE(const FrameSUBSCRIBE &frameSubscribe);

    FrameSUBSCRIBE &operator=(const FrameSUBSCRIBE &frameSubscribe);

    virtual ~FrameSUBSCRIBE() override;

    virtual string toString() ;

    virtual string execute() ;

    string getGenre();


};