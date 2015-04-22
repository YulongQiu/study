#ifndef CLOUD_CLIENT_H
#define CLOUD_CLIENT_H
#include <string>
#include <http/restclient.h>

using namespace std;

class CloudClient
{
public:
    CloudClient(string productId, string productKey, string productSecret);
    ~CloudClient();
    void getDeviceToken();
    void getDateTime();
    void getVersion();
    void getUserInfo();
    void getQRCode();
    void getMQConfig();
    void getLocation();
    void getDeviceProperty();
    void putDeviceProperty();
    void registerDevice();
    void uploadHeartbeat();
    void uploadLog();
    void downloadFile();
    void factoryReset();
    void createSubDevice();
    void deleteSubDevice();

private:
    string _productId;
    string _productKey;
    string _productSecret;

};

#endif
