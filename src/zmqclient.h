#ifndef ZMQCLIENT_H
#define ZMQCLIENT_H

#include <zmq.h>
#include <string>
#include "Utils.h"
#include <Pt/System/Thread.h>
#include <Pt/Signal.h>
//#include <Pt/Connection.h>


//#include "ZbrokerRecvThread.h"
class ReceiveThread;

class ZmqClient  : public Pt::Connectable//public Pt::System::DetachedThread//,
{
public:
    ZmqClient(std::string topic);
    void init(void slot(std::string));
    void start();
    void sendMessage(const std::string &to, const std::string &msg);
    //void receiveMessge();

public:
    void threadStartedSlot();
    void receiveMessageSlot(std::string data);

public:
    Pt::Signal<std::string> _message;
protected:
    void run();

private:
    std::string _topic;
    ReceiveThread *_receiveThread;


};


class ReceiveThread : public Pt::System::DetachedThread
{
public:
            ReceiveThread(ZmqClient *zmqclient);
    virtual ~ReceiveThread();

protected:
    void run();
    void destroy();

private:
    Pt::Signal<> _started;
    Pt::Signal<std::string> _received;
    ZmqClient *_pZmqclient;
};


#endif // ZMQCLIENT_H
