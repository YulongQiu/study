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
    url += "&DMAGIC=" + getMagic();
    RestClient::response res = httpGet(url);;

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
    string url = _apiUrl.getDateTimeUrl();
    addCommonHeader(url);
    RestClient::response res = httpGet(url);;

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }

    replayData = res.body;
    return 0;
}

int CloudClient::getVersion(string &currentVersion, string &replayData)
{
    string url = _apiUrl.getVersionUrl();
    url += "?PID=M500&DID=" + _productId;
    //addCommonHeader(url);

    RestClient::response res;
    res = httpPut(url, _deviceToken, currentVersion);

    /* string url = "http://api.atsmart.io/v2/device/checkVersion?PID=M500&DID=FDE7152926FE932C";
    //string data = {"cmd":"systemVersion","data":{"versionNo":"1.2.4.6"}};
    string token = "19EE69A79C748212";
    RestClient::ctypelist headers;
    headers.push_back("DTOKEN:" + token);
    RestClient::response res = RestClient::put(url, headers, currentVersion); */
    if (res.code != REST_CODE_OK)
    {
        return -1;
    }

    replayData = res.body;
    return 0;
}

int CloudClient::getUserInfo(string &replayData)
{
    std::string url = _apiUrl.getUserInfoUrl();
    addCommonHeader(url);
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
    string url = _apiUrl.getQRCodeUrl();
    addCommonHeader(url);
    url += "&UTYPE=0&QRTYPE=1&SIZE=64&FORMAT=png";

    RestClient::response res = httpGet(url, _deviceToken);
    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
}

int CloudClient::getMQConfig(string &replayData)
{
    string url = _apiUrl.getMQConfigUrl();
    url += "?PID=M500&DID=" + _productId;

    RestClient::response res;
    res = httpGet(url, _deviceToken);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
}
/*
int CloudClient::getLocation(string &replayData)
{
    string url = _apiUrl.getLocationUrl();
    url += "?PID=M500&DID=" + _productId;

    RestClient::response res;
    res = httpGet(url);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
} */

int CloudClient::getDeviceProperty(string &replayData)
{
    string url = _apiUrl.getDevicePropertyUrl();
    url += "?PID=M500&DID=" + _productId;

    RestClient::response res;
    res = httpGet(url, _deviceToken);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
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
    + "&PID=M500";
}

void CloudClient::addToken(RestClient::ctypelist &headers)
{
    headers.push_back("DTOKEN:"+_deviceToken);
}

RestClient::response CloudClient::httpGet(string url, string token, string data)
{
    RestClient::ctypelist headers;
    if(!token.empty())
    {
        headers.push_back("DTOKEN:"+token);
    }

    //addCommonHeader(url);
    if (!data.empty())
    {
        url += "&"+data;
    }
    std::cout << "url: " << url << std::endl;
    return RestClient::get(url, headers);

}

RestClient::response CloudClient::httpPut(string url , string token, string data)
{
    RestClient::ctypelist headers;
    if(!token.empty())
    {
        headers.push_back("DTOKEN:"+token);
    }

    //addCommonHeader(url);
    /* if (!data.empty())
    {
        url += "&"+data;
    } */
    std::cout << "url: " << url << std::endl;
    std::cout << "token: " << token << std::endl;
    std::cout << "data: " << data << std::endl;
    return RestClient::put(url, headers, data);
}

string CloudClient::getMagic()
{
    char timeStamp[128];
    struct timeb tp;
    ftime(&tp);
    sprintf(timeStamp, "%ld", tp.time / 1000);

    std::string key = "0000XPCA1403CNSIOLW7" + std::string(timeStamp);
    unsigned char sig[16];
    char	 str[33];
    md5_buffer(key.c_str(), key.size(), (void*)sig);
    md5_sig_to_string(sig, str, sizeof(str));

    //LOG(DEBUG)<<"_puk="<<_puk<<", _sn="<<_sn<<", timeStamp="<<timeStamp<<", md5="<<std::string(str);

    return string(str);
}
