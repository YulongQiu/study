#ifndef CLOUD_CLIENT_H
#define CLOUD_CLIENT_H
#include <string>
#include <http/restclient.h>
#include <jsoncpp/jsoncpp.h>
#include "ApiUrl.h"
#include <iostream>
extern "C" {
    #include <md5/md5.h>
    #include <sys/timeb.h>
}

using namespace std;

class CloudClient
{
public:
    CloudClient(string productId, string productKey, string productSecret, string magic);
    ~CloudClient();
    int getDeviceToken(/*string magic, */string &replayData);
    int getDateTime(string &replayData);
    int getVersion(string &currentVersion, string &replayData);
    int getUserInfo(string &replayData);
    int getQRCode(string &replayData);
    int getMQConfig(string &replayData);
    //int getLocation(string &replayData);
    int getDeviceProperty(string &replayData);
    int putDeviceProperty(string &replayData);
    int registerDevice(string &properties, string &replayData);
    int uploadHeartbeat(string &replayData);
    int uploadLog(string &replayData);
    int downloadFile(string &replayData);
    int factoryReset(string &replayData);
    int createSubDevice(string &subDeviceMac, string &replayData);
    int bindSubDevice(string &subDeviceId, string &replayData);
    int deleteSubDevice(string &subDeviceId, string &replayData);
    int getTrigger(string &replayData);

public:
    Signal<ReplyId> qrcodeDone;

private:
    void onqrCodeComplete(HttpReply* reply);

    void addCommonHeader(string &url);
    void addToken(RestClient::ctypelist &headers);
    string getMagic();
    RestClient::response httpGet(string url , string token = "", string data = "");
    RestClient::response httpPut(string url , string token = "", string data = "");

private:
    string _productId;
    string _productKey;
    string _productSecret;
    string _deviceToken;
    string _magic;
    ApiUrl _apiUrl;
};

#endif
