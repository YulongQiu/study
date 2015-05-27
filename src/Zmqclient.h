#ifndef ZMQCLIENT_H
#define ZMQCLIENT_H

#include <string>
#include <Pt/System/Thread.h>
#include <Pt/Signal.h>
#include <zbroker.h>


class ReceiveThread;

class ZMessage
{
public:
    ZMessage();
    void setTo(const std::string &to);
    void setFrom(const std::string &from);
    void setData(const std::string &data);
    std::string getTo() const;
    std::string getFrom() const;
    std::string getData() const;

private:
    std::string _msg_form;
    std::string _msg_to;
    std::string _msg_data;
};

class ZmqClient  : public Pt::Connectable
{
public:
    ZmqClient(const std::string& topic);
    ~ZmqClient();
    void start();
    bool sendMessage(const std::string &to, const std::string &msg);

public:
    void threadStartedSlot();
    void receiveMessageSlot(const ZMessage &msg);

public:
    Pt::Signal<const ZMessage&> message;
    Pt::Signal<> receiveThreadStarted;

private:
    void* createSocket();
    void  destorySocket(void *backend);

private:
    bool               _isStarted;
    const std::string  _topic;
    ReceiveThread     *_receiveThread;
};


#endif // ZMQCLIENT_H
