#include "CloudClient.h"

CloudClient::CloudClient(string productId, string productKey, string productSecret)
    : _productId(productId), _productKey(productKey), _productSecret(productSecret)
{

}

CloudClient::~CloudClient()
{

}

int CloudClient::getDeviceToken(string &replayData)
{
    string url = _apiUrl.getTokenUrl();
    addCommonHeader(url);

    RestClient::ctypelist headers;
    RestClient::response res;

    res = RestClient::get(url, headers);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }

    replayData = res.body;
    return 0;

}

int CloudClient::getDateTime(string &replayData)
{
    RestClient::ctypelist headers;
    RestClient::response res;

    res = RestClient::get(_apiUrl.getDateTimeUrl(), headers);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }

    replayData = res.body;
    return 0;
}

int CloudClient::getVersion()
{

}

int CloudClient::getUserInfo()
{

}

int CloudClient::getQRCode()
{

}

int CloudClient::getMQConfig()
{

}

int CloudClient::getLocation()
{

}

int CloudClient::getDeviceProperty()
{

}

int CloudClient::putDeviceProperty()
{

}

int CloudClient::registerDevice()
{

}

int CloudClient::uploadHeartbeat()
{

}

int CloudClient::uploadLog()
{

}

int CloudClient::downloadFile()
{

}

int CloudClient::factoryReset()
{

}

int CloudClient::createSubDevice()
{

}

int CloudClient::deleteSubDevice()
{

}

void CloudClient::addCommonHeader(string &url)
{
     url += "?DID=" + _productId + "&productkey=" + _productKey
    + "&productsecret=" + _productSecret
    + "&from=1";
}
