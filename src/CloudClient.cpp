#include "CloudClient.h"


//c/onnect(cli->qrcodeDone, this, &M2MManager::getQRCodeComplete);
//cli->getQRCode(0);



CloudClient::CloudClient(string productId, string productKey, string productSecret, string magic)
    : _productId(productId), _productKey(productKey), _productSecret(productSecret), _magic(magic)
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

int CloudClient::getLocation(string &replayData)
{
    string url = _apiUrl.getLocationUrl();
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

int CloudClient::putDeviceProperty(string &data, string &replayData)
{
    string url = _apiUrl.getDevicePropertyUrl();
    url += "?PID=M500&DID=" + _productId;

    RestClient::response res;
    res = httpPut(url, _deviceToken, data);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
}

int CloudClient::registerDevice(string &properties, string &replayData)
{
    string url = _apiUrl.getDevicePropertyUrl();
    url += "?PID=M500&DID=" + _productId;

    RestClient::ctypelist headers;
    headers.push_back("DTOKEN:" + _deviceToken);
    RestClient::response res;
    res = RestClient::post(url, headers, properties);

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
}

int CloudClient::uploadHeartbeat(string &replayData)
{
    string url = _apiUrl.getHeartbeatUrl();
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

int CloudClient::uploadLog(string &replayData)
{
    std::string logFile = "/tmp/log/xweb.log";
    std::string file = logFile + "." + _productId;
    std::string copyLogfile = "sudo cp " + logFile + " " + file + " && cat /dev/null > " + logFile;
    if (system(copyLogfile.c_str()) == 0)
    {
        std::string uploadCmd = "curl -H \"Content-Type: multipart/related\" --form \"file=@" + file+ "\" ";
        uploadCmd += "\"" + _apiUrl.getDiagnoseInfoUrl() + "?PID=M500&DID=" + _productId
                        + "\" -H \"DTOKEN: "+ _deviceToken +"\"";
        std::cout << "uploadCmd: " << uploadCmd << std::endl;
        if (system(uploadCmd.c_str()) == 0)
        {
            replayData = "Upload seccess.";
            return 0;
        }

    }

    replayData = "Upload failed.";
    return -1;
}

int CloudClient::downloadFile(string &replayData)
{
    replayData = "1";
    return 0;
}

int CloudClient::factoryReset(string &replayData)
{
    string url = _apiUrl.getFactoryResetUrl();
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

int CloudClient::createSubDevice(string &subDeviceMac, string &replayData)
{
    string url = _apiUrl.getSubDeviceUrl();
    url += "/create?PID=M500&DID=" + _productId + "&mac=" + subDeviceMac;

    RestClient::ctypelist headers;
    headers.push_back("DTOKEN:"+_deviceToken);
    RestClient::response res;
    std::cout << "url: " << url << std::endl;
    res = RestClient::post(url, headers, "");

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
}

int CloudClient::bindSubDevice(string &subDeviceId, string &replayData)
{
    string url = _apiUrl.getSubDeviceUrl();
    url += "/bind?PID=M500&DID=" + _productId + "&subDeviceId="+subDeviceId;

    RestClient::ctypelist headers;
    headers.push_back("DTOKEN:"+_deviceToken);
    RestClient::response res;
    res = RestClient::post(url, headers, "");

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
}

int CloudClient::deleteSubDevice(string &subDeviceId, string &replayData)
{
    string url = _apiUrl.getSubDeviceUrl();
    url += "/unbind?PID=M500&DID=" + _productId + "&subDeviceId="+subDeviceId;

    RestClient::ctypelist headers;
    headers.push_back("DTOKEN:"+_deviceToken);
    RestClient::response res;
    res = RestClient::post(url, headers, "");

    if (res.code != REST_CODE_OK)
    {
        return -1;
    }
    replayData = res.body;
    return 0;
}

int CloudClient::getTrigger(string &replayData)
{
    string url = _apiUrl.getTriggerUrl();
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

    std::string key = "0000FDE7152926FE932C" + std::string(timeStamp);
    unsigned char sig[16];
    char	 str[33];
    md5_buffer(key.c_str(), key.size(), (void*)sig);
    md5_sig_to_string(sig, str, sizeof(str));

    //LOG(DEBUG)<<"_puk="<<_puk<<", _sn="<<_sn<<", timeStamp="<<timeStamp<<", md5="<<std::string(str);

    return _magic;
}
