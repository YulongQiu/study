#include "CloudClient.h"

CloudClient::CloudClient(string productId, string productKey, string productSecret)
    : _productId(productId), _productKey(productKey), _productSecret(productSecret)
{

}

CloudClient::~CloudClient()
{

}

int CloudClient::getDeviceToken(/*string magic, */string &replayData)
{
    string url = _apiUrl.getTokenUrl();
    addCommonHeader(url);
    //url += "&DMAGIC=" + magic;
    RestClient::ctypelist headers;
    RestClient::response res;

    res = RestClient::get(url, headers);

    if (res.code != REST_CODE_OK)
    {
        // LOG(WARNING) << "Get device token failed.";
        return -1;
    }

    Json::Reader jsReader;
    Json::Value jsValue;
    bool parseSuccess = jsReader.parse(res.body, jsValue);
    if(!parseSuccess)
    {
        // LOG(WARNING) << "Device token parse failed.";
        return -1;
    }

     bool isString = jsValue["data"]["access_token"].isString();
    /*if(!isString)
    {
        return -1;
    } */
    _deviceToken = jsValue["data"]["access_token"].asString();

    replayData = res.body;
    return 0;

}

int CloudClient::getDateTime(string &replayData)
{
    RestClient::response res;
    res = httpGet(_apiUrl.getDateTimeUrl());

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }

    replayData = res.body;
    return 0;
}

int CloudClient::getVersion(string currentVersion, string &replayData)
{
    string url = _apiUrl.getVersionUrl();
    addCommonHeader(url);
    //url += "&PID=M500";

    RestClient::response res;
    res = httpPut(url, _deviceToken, currentVersion);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }

    replayData = res.body;
    return 0;
}

int CloudClient::getUserInfo(string &replayData)
{
    std::string url = _apiUrl.getUserInfoUrl();;
    RestClient::response res = httpGet(url, _deviceToken);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
}

int CloudClient::getQRCode(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::getMQConfig(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::getLocation(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::getDeviceProperty(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::putDeviceProperty(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::registerDevice(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::uploadHeartbeat(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::uploadLog(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::downloadFile(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::factoryReset(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::createSubDevice(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::deleteSubDevice(string &replayData)
{
    replayData = "1";
    return 0;
}

void CloudClient::addCommonHeader(string &url)
{
     url += "?DID=" + _productId + "&productkey=" + _productKey
    + "&productsecret=" + _productSecret
    + "&from=1&PID=M500";
}

void CloudClient::addToken(RestClient::ctypelist &headers)
{
    headers.push_back("DTOKEN:"+_deviceToken);
}

/* string CloudClient::getMagic()
{
    char timeStamp[128];
    struct timeb tp;
    ftime(&tp);
    sprintf(timeStamp, "%ld", tp.time / 1000);

    std::string key=_puk+_sn+timeStamp;
    unsigned char sig[16];
    char	 str[33];
    md5_buffer(key.c_str(), key.size(), (void*)sig);
    md5_sig_to_string(sig, str, sizeof(str));

    return std::string(str);
} */

RestClient::response CloudClient::httpGet(string url, string token, string data)
{
    RestClient::ctypelist headers;
    if(!token.empty())
    {
        headers.push_back("DTOKEN:"+token);
    }

    addCommonHeader(url);
    if (!data.empty())
    {
        url += "&"+data;
    }

    return RestClient::get(url, headers);

}

RestClient::response CloudClient::httpPut(string url , string token, string data)
{
    RestClient::ctypelist headers;
    if(!token.empty())
    {
        headers.push_back("DTOKEN:"+token);
    }

    addCommonHeader(url);
    /* if (!data.empty())
    {
        url += "&"+data;
    } */

    return RestClient::put(url, headers, data);
}

