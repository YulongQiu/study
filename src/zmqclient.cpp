#include "zmqclient.h"
#include <assert.h>
#include <string.h>
#include <jsoncpp/jsoncpp.h>
#include "ZeroMQ.h"
#include <iostream>
//#include <Pt/Connectable.h>
#include <stdlib.h>
#include <unistd.h>
// #include <zmq.h>

#define TOPIC_RF  "RF"
#define TOPIC_ZPOWER  "ZPOWER"
#define TOPIC_XCONNECTION  "XCONNECTION"
#define TOPIC_XPOWER  "XPOWER"
#define TOPIC_WEB  "WEB"
#define TOPIC_XSYSTEM  "XSYSTEM"
#define TOPIC_BROADCAST  "BROADCAST"

#define ZBROKER_FRONTEND_SOCKET  "tcp://127.0.0.1:9527"
#define ZBROKER_BACKEND_SOCKET "tcp://127.0.0.1:9981"

ZmqClient::ZmqClient(std::string topic)
{
    _topic = topic;
    _receiveThread = new ReceiveThread(this);
}

void ZmqClient::init(void slot(std::string))
{
    _message += Pt::slot(slot);
}

void ZmqClient::start()
{
#if 1
    _receiveThread->start(512 * 1024);
#endif

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

bool sendMsg(const char *pFrom, const char *pTo, const char *pData, int count)
{
    int timeout = 100;
    int result = 0;

    tsZeroMQ_MSG CMsg = {(char *)pFrom, (char *)pTo, (char *)pData, count};

    void *context = zmq_ctx_new();
    void *backend = zmq_socket(context, ZMQ_PUSH);
    if (!backend)
    {
        return false;
    }

    result = zmq_connect(backend, ZBROKER_FRONTEND_SOCKET);
    if (result != 0)
    {
        zmq_close(backend);
        return false;
    }

    result = zmq_setsockopt(backend, ZMQ_SNDTIMEO, &timeout, sizeof(int));
    if (result != 0)
    {
        zmq_disconnect(backend, ZBROKER_FRONTEND_SOCKET);
        zmq_close(backend);
        return false;
    }

    result = zmq_setsockopt(backend, ZMQ_RCVTIMEO, &timeout, sizeof(int));
    if (result != 0)
    {
        zmq_disconnect(backend, ZBROKER_FRONTEND_SOCKET);
        zmq_close(backend);
        return false;
    }

    if(strlen(CMsg.msg_to))
    {
        char buff[128];
        sprintf(buff, "%s#%s", pTo, pFrom);
        send_msg(backend, (void *)buff, strlen(buff), ZMQ_SNDMORE);
        send_msg(backend, (void *)pData, count, 0);
    }

    zmq_disconnect(backend, ZBROKER_FRONTEND_SOCKET);
    zmq_close(backend);
    context = NULL;
    backend = NULL;
    return true;

}

void ZmqClient::sendMessage(const std::string &to, const std::string &msg)
{
    sendMsg(_topic.c_str(), to.c_str(),
            (char *)msg.c_str(), msg.size());

    sleep(1);
}


#if 0
void ZmqClient::receiveMessge()
{
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, ZBROKER_BACKEND_SOCKET);
    assert(rc == 0);

    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, TOPIC_BROADCAST, strlen(TOPIC_BROADCAST));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, TOPIC_WEB, strlen(TOPIC_WEB));

    zmq_pollitem_t pollitems[] = { { NULL, 0, ZMQ_POLLIN, 0 } };


    /* poll */
    pollitems[0].socket = subscriber;

    while (1)
    {
        if (zmq_poll(pollitems, 1, -1) == -1)
        {
            break;
        }

        if (pollitems[0].revents & ZMQ_POLLIN)
        {
            int more, flags;
            size_t moresz = sizeof(more);
            tsZeroMQ_MSG *psmsg;

            /* Data Receive */
            psmsg = eZeroMQ_NewMessage();
            if(!psmsg)
            {
                // DBG_vPrintf(DBG_ZEROMQ, "Failed to Creat Event.\n");
                std::cout << "Failed to Creat Event.\n";
            }

            while(psmsg)
            {
                zmq_msg_t zmsg;

                if (zmq_msg_init(&zmsg) != 0)
                {
                    break;
                }

                if (zmq_recvmsg(subscriber, &zmsg, 0) == -1)
                {
                    // DBG_vPrintf(DBG_ZEROMQ, "Failed to receive msg: %s.\n", zmq_strerror(errno));
                    std::cout << "Failed to receive msg.\n";
                    zmq_msg_close(&zmsg);
                    break;
                }

                if (zmq_getsockopt(subscriber, ZMQ_RCVMORE, &more, &moresz) == -1)
                {
                    // DBG_vPrintf(DBG_ZEROMQ, "Failed to set socket option: %s.\n", zmq_strerror(errno));
                    std::cout << "Failed to set socket option.\n";
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
                        char *to, *form;

                        memcpy(szBuff, (void *)zmq_msg_data(&zmsg), cnt);
                        szBuff[cnt] = '\0';

                        to = strtok(szBuff,split);
                        form = strtok(NULL,split);
                        if(to && form)
                        {
                            psmsg->msg_form = (char *)malloc(strlen(form)+1);
                            psmsg->msg_to = (char *)malloc(strlen(to)+1);
                            if(psmsg->msg_form)
                            {
                                strcpy(psmsg->msg_form, form);
                                psmsg->msg_form[strlen(form)] = '\0';
                            }
                            if(psmsg->msg_to)
                            {
                                strcpy(psmsg->msg_to, to);
                                psmsg->msg_to[strlen(to)] = '\0';
                            }
                        }
                        free(szBuff);
                    }
                }
                else    // parse data
                {
                    if(psmsg->msg_form)
                    {
                        psmsg->msg_cnt = zmq_msg_size(&zmsg);
                        psmsg->msg_data = (void *)malloc(psmsg->msg_cnt+1);
                        if (psmsg->msg_data)
                        {
                            char *pData = (char*)psmsg->msg_data;
                            memcpy(psmsg->msg_data, (void *)zmq_msg_data(&zmsg), psmsg->msg_cnt);
                            pData[psmsg->msg_cnt] = '\0';
                            //eUtils_QueueQueue(&psZMQWorker->queue, psmsg);
                            std::string data((char*)psmsg->msg_data);
                            std::cout << "receive: \n" << data << std::endl;
                        }
                        else
                        {
                            eZeroMQ_DestoryMessage(psmsg);
                        }
                    }
                    else
                    {
                        // DBG_ERR_vPrintf("Unkonw MSG: cnt[%d]\n", zmq_msg_size(&zmsg));
                        std::cout << "Unkonw MSG \n";
                        eZeroMQ_DestoryMessage(psmsg);
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
#endif

void ZmqClient::receiveMessageSlot(std::string data)
{
    _message.send(data);
}

void ZmqClient::run()
{
    std::cout << "runing" << std::endl;
}


void ZmqClient::threadStartedSlot()
{
    std::cout << "Receice theard started." << std::endl;
}

ReceiveThread::ReceiveThread(ZmqClient *zmqclient)
{
    _pZmqclient = zmqclient;
    _started += Pt::slot(*_pZmqclient, &ZmqClient::threadStartedSlot);
    _received += Pt::slot(*_pZmqclient, &ZmqClient::receiveMessageSlot);
}

ReceiveThread::~ReceiveThread()
{

}

void ReceiveThread::run()
{
#if 0
    int i = 10;
    while (i-- > 0)
    {
        std::cout << "hello world" << std::endl;
    }
#endif

    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, ZBROKER_BACKEND_SOCKET);
    assert(rc == 0);

    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, TOPIC_BROADCAST, strlen(TOPIC_BROADCAST));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, TOPIC_WEB, strlen(TOPIC_WEB));

    // broadcast started.
    Json::Value data;
    data["cmd"] = "nonregister";
    data["data"] = "xweb started";
    _started.send();

    zmq_pollitem_t pollitems[] = { { NULL, 0, ZMQ_POLLIN, 0 } };


    /* poll */
    pollitems[0].socket = subscriber;

    while (1)
    {
        if (zmq_poll(pollitems, 1, -1) == -1)
        {
            break;
        }

        if (pollitems[0].revents & ZMQ_POLLIN)
        {
            int more, flags;
            size_t moresz = sizeof(more);
            tsZeroMQ_MSG *psmsg;

            /* Data Receive */
            psmsg = eZeroMQ_NewMessage();
            if(!psmsg)
            {
                // DBG_vPrintf(DBG_ZEROMQ, "Failed to Creat Event.\n");
                std::cout << "Failed to Creat Event.\n";
            }

            while(psmsg)
            {
                zmq_msg_t zmsg;

                if (zmq_msg_init(&zmsg) != 0)
                {
                    break;
                }

                if (zmq_recvmsg(subscriber, &zmsg, 0) == -1)
                {
                    // DBG_vPrintf(DBG_ZEROMQ, "Failed to receive msg: %s.\n", zmq_strerror(errno));
                    std::cout << "Failed to receive msg.\n";
                    zmq_msg_close(&zmsg);
                    break;
                }

                if (zmq_getsockopt(subscriber, ZMQ_RCVMORE, &more, &moresz) == -1)
                {
                    // DBG_vPrintf(DBG_ZEROMQ, "Failed to set socket option: %s.\n", zmq_strerror(errno));
                    std::cout << "Failed to set socket option.\n";
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
                        char *to, *form;

                        memcpy(szBuff, (void *)zmq_msg_data(&zmsg), cnt);
                        szBuff[cnt] = '\0';

                        to = strtok(szBuff,split);
                        form = strtok(NULL,split);
                        if(to && form)
                        {
                            psmsg->msg_form = (char *)malloc(strlen(form)+1);
                            psmsg->msg_to = (char *)malloc(strlen(to)+1);
                            if(psmsg->msg_form)
                            {
                                strcpy(psmsg->msg_form, form);
                                psmsg->msg_form[strlen(form)] = '\0';
                            }
                            if(psmsg->msg_to)
                            {
                                strcpy(psmsg->msg_to, to);
                                psmsg->msg_to[strlen(to)] = '\0';
                            }
                        }
                        free(szBuff);
                    }
                }
                else    // parse data
                {
                    if(psmsg->msg_form)
                    {
                        psmsg->msg_cnt = zmq_msg_size(&zmsg);
                        psmsg->msg_data = (void *)malloc(psmsg->msg_cnt+1);
                        if (psmsg->msg_data)
                        {
                            char *pData = (char*)psmsg->msg_data;
                            memcpy(psmsg->msg_data, (void *)zmq_msg_data(&zmsg), psmsg->msg_cnt);
                            pData[psmsg->msg_cnt] = '\0';
                            //eUtils_QueueQueue(&psZMQWorker->queue, psmsg);
                            std::string data((char*)psmsg->msg_data);
                            //std::cout << "receive: \n" << data << std::endl;
                            _received.send(data);
                        }
                        else
                        {
                            eZeroMQ_DestoryMessage(psmsg);
                        }
                    }
                    else
                    {
                        // DBG_ERR_vPrintf("Unkonw MSG: cnt[%d]\n", zmq_msg_size(&zmsg));
                        std::cout << "Unkonw MSG \n";
                        eZeroMQ_DestoryMessage(psmsg);
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
    std::cout << "destroy" << std::endl;
}

