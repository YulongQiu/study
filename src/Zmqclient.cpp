#include "Zmqclient.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>
#include <logger/logger.h>

ZMessage::ZMessage()
{
    _msg_data = "";
    _msg_form = "";
    _msg_to = "";
}

void ZMessage::setTo(const std::string &to)
{
    _msg_to = to;
}

void ZMessage::setFrom(const std::string &from)
{
    _msg_form = from;
}

void ZMessage::setData(const std::string& data)
{
    _msg_data = data;
}

std::string ZMessage::getTo() const
{
    return _msg_to;
}

std::string ZMessage::getFrom() const
{
    return _msg_form;
}

std::string ZMessage::getData() const
{
    return _msg_data;
}


class ReceiveThread : public Pt::System::DetachedThread
{
public:
    ReceiveThread(const std::string &topic);
    virtual ~ReceiveThread();

protected:
    void run();
    void destroy();

public:
    std::string _topic;
    Pt::Signal<> started;
    Pt::Signal<const ZMessage&> received;
};


ReceiveThread::ReceiveThread(const std::string &topic)
    : _topic(topic)
{

}

ReceiveThread::~ReceiveThread()
{

}

void ReceiveThread::run()
{
    LOG(INFO) << "Zmq receive thread is runing.";
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, ZBROKER_BACKEND_SOCKET);
    assert(rc == 0);

    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, TOPIC_BROADCAST, strlen(TOPIC_BROADCAST));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, _topic.c_str(), _topic.size());
    LOG(INFO) << "Subscribe topic is " << _topic;

    // broadcast started.
    started.send();

    zmq_pollitem_t pollitems[] = { { NULL, 0, ZMQ_POLLIN, 0 } };


    /* poll */
    pollitems[0].socket = subscriber;

    while (1)
    {
        if (zmq_poll(pollitems, 1, -1) == -1)
        {
            LOG(ERROR) << "Failed to poll zmq.";
            break;
        }

        if (pollitems[0].revents & ZMQ_POLLIN)
        {
            int more, flags;
            size_t moresz = sizeof(more);
            ZMessage msg;

            while(1)
            {
                zmq_msg_t zmsg;

                if (zmq_msg_init(&zmsg) != 0)
                {
                    LOG(ERROR) << "Failed to init zmq message.";
                    break;
                }

                if (zmq_recvmsg(subscriber, &zmsg, 0) == -1)
                {

                    LOG(ERROR) << "Failed to receive msg.";
                    zmq_msg_close(&zmsg);
                    break;
                }

                if (zmq_getsockopt(subscriber, ZMQ_RCVMORE, &more, &moresz) == -1)
                {
                    LOG(ERROR) << "Failed to set socket option.";
                    more = 0;
                }

                if (more)
                {
                    flags = ZMQ_SNDMORE;
                }
                else
                {
                    flags = 0;
                }

                if(flags == ZMQ_SNDMORE) // Parse TOPIC FORM & TO
                {
                    char *szBuff;
                    int cnt = zmq_msg_size(&zmsg);

                    szBuff = (char*)malloc(cnt+1);
                    if(szBuff)
                    {
                        const char *split = "#";
                        char *to, *from;

                        memcpy(szBuff, (void *)zmq_msg_data(&zmsg), cnt);
                        szBuff[cnt] = '\0';

                        to = strtok(szBuff,split);
                        from = strtok(NULL,split);
                        if(to && from)
                        {
                            msg.setFrom(std::string(from));
                            msg.setTo(std::string(to));
                        }
                        free(szBuff);
                    }
                }
                else    // parse data
                {
                   if(!msg.getFrom().empty())
                   {
                       int cnt = zmq_msg_size(&zmsg);
                       void *buf = (void *)malloc(cnt+1);
                       if (buf)
                       {
                           char *pData = (char*)buf;
                           memcpy(buf, (void *)zmq_msg_data(&zmsg), cnt);
                           pData[cnt] = '\0';
                           std::string data((char*)buf);
                           msg.setData(data);
                           received.send(msg);
                           free(buf);
                       }
                    }
                }

                zmq_msg_close(&zmsg);

                if (flags == 0)
                {
                    break;
                }
            } // while
        } // if
    } // while
}

void ReceiveThread::destroy()
{
    delete this;
}


/* ZmqClient */
ZmqClient::ZmqClient(const std::string &topic)
    : _topic(topic)
{
    _isStarted = false;
    _receiveThread = new ReceiveThread(topic);
    _receiveThread->started += Pt::slot(*this, &ZmqClient::threadStartedSlot);
    _receiveThread->received += Pt::slot(*this, &ZmqClient::receiveMessageSlot);
}

ZmqClient::~ZmqClient()
{
    delete _receiveThread;
    _receiveThread = NULL;
}


void ZmqClient::start()
{
    if (!_isStarted)
    {
       _receiveThread->start(512 * 1024);
       _isStarted = true;
    }
}

static int send_msg(void *endpoint, void *msg, int size, int flag)
{
    zmq_msg_t zmsg;
    int ret;

    if (zmq_msg_init(&zmsg) != 0) {
            return -1;
    }

    zmq_msg_init_size(&zmsg, size);
    memcpy((void *)zmq_msg_data(&zmsg), msg, zmq_msg_size(&zmsg));

    ret = zmq_sendmsg(endpoint, &zmsg, flag);

    zmq_msg_close(&zmsg);

    return ret;
}

bool ZmqClient::sendMessage(const std::string &to, const std::string &msg)
{
    int result = 0;
    if (msg.size() > 0)
    {
        void *backend = NULL;
        backend = createSocket();
        char buff[128];
        sprintf(buff, "%s#%s", to.c_str(), _topic.c_str());

        result = send_msg(backend, (void *)buff, strlen(buff), ZMQ_SNDMORE);
        if (result != -1)
        {
            result = send_msg(backend, (void *)msg.c_str(), msg.size(), 0);
        }

        destorySocket(backend);
    }

    return (result > 0) ? true : false;
}


void ZmqClient::receiveMessageSlot(const ZMessage &msg)
{
    message.send(msg);
}

void* ZmqClient::createSocket()
{
    int timeout = 100;
    int result = 0;

    void *context = zmq_ctx_new();
    void *backend = zmq_socket(context, ZMQ_PUSH);
    if (!backend)
    {
        return NULL;
    }

    result = zmq_connect(backend, ZBROKER_FRONTEND_SOCKET);
    if (result != 0)
    {
        zmq_close(backend);
        return NULL;
    }

    result = zmq_setsockopt(backend, ZMQ_SNDTIMEO, &timeout, sizeof(int));
    if (result != 0)
    {
        zmq_disconnect(backend, ZBROKER_FRONTEND_SOCKET);
        zmq_close(backend);
        return NULL;
    }

    result = zmq_setsockopt(backend, ZMQ_RCVTIMEO, &timeout, sizeof(int));
    if (result != 0)
    {
        zmq_disconnect(backend, ZBROKER_FRONTEND_SOCKET);
        zmq_close(backend);
        return NULL;
    }

    return backend;
}

void  ZmqClient::destorySocket(void *backend)
{
    zmq_disconnect(backend, ZBROKER_FRONTEND_SOCKET);
    zmq_close(backend);
}

void ZmqClient::threadStartedSlot()
{
   receiveThreadStarted.send();
}




