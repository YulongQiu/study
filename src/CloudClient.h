#ifndef CLOUD_CLIENT_H
#define CLOUD_CLIENT_H
#include <string>
#include <http/restclient.h>
#include "ApiUrl.h"

using namespace std;

class CloudClient
{
public:
    CloudClient(string productId, string productKey, string productSecret);
    ~CloudClient();
    int getDeviceToken(string &replayData);
    int getDateTime(string &replayData);
    int getVersion();
    int getUserInfo();
    int getQRCode();
    int getMQConfig();
    int getLocation();
    int getDeviceProperty();
    int putDeviceProperty();
    int registerDevice();
    int uploadHeartbeat();
    int uploadLog();
    int downloadFile();
    int factoryReset();
    int createSubDevice();
    int deleteSubDevice();

private:
    void addCommonHeader(string &url);

private:
    string _productId;
    string _productKey;
    string _productSecret;
    ApiUrl _apiUrl;
};

#endif
