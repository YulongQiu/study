#include "gtest/gtest.h"
#include "gmock/gmock.h"

//#include "../src/network/cloud/CloudClient.h"
#include "CloudClient.h"
#include "md5/md5.h"

class CloudClientTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        res = 0;
        data = "";
        cloudClient = new CloudClient("2F1F338D1064DBCF", "CE3EAEE20D55D85D",
                                      "D116BF8A2CCE15F2", getDMAGIC());
        std::string token;
        cloudClient->getDeviceToken(token);
    }

    virtual void TearDown()
    {
        delete cloudClient;
    }

    void printData();
    std::string getDMAGIC();


protected:
    int res;
    std::string data;
    CloudClient *cloudClient;
};

void CloudClientTest::printData()
{
    Json::Reader jsReader;
    Json::Value jsValue;
    jsReader.parse(data, jsValue);
    std::cout << "data: \n" << jsValue.toStyledString() << std::endl;
}

std::string CloudClientTest::getDMAGIC()
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

    return std::string(str);
}
/*
TEST_F(CloudClientTest, getDeviceToken)
{
    res = cloudClient->getDeviceToken(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, getDateTime)
{
    res = cloudClient->getDateTime(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, getUserInfo)
{

    res = cloudClient->getUserInfo(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, getQRCode)
{
    res = cloudClient->getQRCode(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, checkUbootVersion)
{
    Json::Value currentVersion;
    currentVersion["cmd"] = "ubootVersion";
    currentVersion["data"]["versionNo"] = "4.1.2.1";
    //std::cout << currentVersion.toStyledString() << std::endl;
    std::string version = currentVersion.toString();
    res = cloudClient->getVersion(version, data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, checkVersion)
{
    Json::Value currentVersion;
    currentVersion["cmd"] = "systemVersion";
    currentVersion["data"]["versionNo"] = "1.2.4.0";

    std::string version = currentVersion.toString();
    std::cout << version << std::endl;
    res = cloudClient->getVersion(version, data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}


TEST_F(CloudClientTest, getMQConfig)
{
    res = cloudClient->getMQConfig(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}


TEST_F(CloudClientTest, uploadHeartbeat)
{
    res = cloudClient->uploadHeartbeat(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, registerDevice)
{
    Json::Value sn;
    sn["id"] = "p_deviceSN";
    sn["type"] = "string";
    sn["value"] = "XPCA1403CNSIOLW7";

    Json::Value version;
    version["id"] = "p_version";
    version["type"] = "string";
    version["value"] = "1.2.4.6";

    Json::Value ssid;
    ssid["id"] = "p_hostSSID";
    ssid["type"] = "string";
    ssid["value"] = "";

    Json::Value router;
    router["id"] = "p_hostRouter";
    router["type"] = "string";
    router["value"] = "";

    Json::Value signalStrength;
    signalStrength["id"] = "p_netSignalStrength";
    signalStrength["type"] = "string";
    signalStrength["value"] = "3";

    Json::Value mac;
    mac["id"] = "p_mac";
    mac["type"] = "string";
    mac["value"] = "00:24:e8:08:77:6c\n";

    Json::Value deviceLock;
    deviceLock["id"] = "p_deviceLock";
    deviceLock["type"] = "bool";
    deviceLock["value"] = false;

    Json::Value port1;
    port1["id"] = "p_port1";
    port1["type"] = "bool";
    port1["value"] = true;

    Json::Value port1_lock;
    port1_lock["id"] = "p_port1_lock";
    port1_lock["type"] = "bool";
    port1_lock["value"] = false;

    Json::Value port2;
    port2["id"] = "p_port2";
    port2["type"] = "bool";
    port2["value"] = true;

    Json::Value port2_lock;
    port2_lock["id"] = "p_port2_lock";
    port2_lock["type"] = "bool";
    port2_lock["value"] = false;

    Json::Value port3;
    port3["id"] = "p_port3";
    port3["type"] = "bool";
    port3["value"] = true;

    Json::Value port3_lock;
    port3_lock["id"] = "p_port3_lock";
    port3_lock["type"] = "bool";
    port3_lock["value"] = false;

    Json::Value port4;
    port4["id"] = "p_port4";
    port4["type"] = "bool";
    port4["value"] = true;

    Json::Value port4_lock;
    port4_lock["id"] = "p_port4_lock";
    port4_lock["type"] = "bool";
    port4_lock["value"] = false;

    Json::Value array;
    array.append(sn);
    array.append(version);
    array.append(ssid);
    array.append(router);
    array.append(signalStrength);
    array.append(mac);
    array.append(deviceLock);
    array.append(port1);
    array.append(port1_lock);
    array.append(port2);
    array.append(port2_lock);
    array.append(port3);
    array.append(port3_lock);
    array.append(port4);
    array.append(port4_lock);


    //data.append(sn);
    Json::Value properties;
    //properties["id"] = "FDE7152926FE932C";
    properties["cmd"] = "PropertyCreate";
    //properties["name"] = "XPOWER";
    properties["data"] = array;

    string str = properties.toString();
    cout << "properties: \n" << str << endl;
    res = cloudClient->registerDevice(str ,data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, getDeviceProperty)
{
    res = cloudClient->getDeviceProperty(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}




TEST_F(CloudClientTest, factoryReset)
{
    res = cloudClient->factoryReset(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, getTrigger)
{
    res = cloudClient->getTrigger(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}
*/


TEST_F(CloudClientTest, createSubDevice)
{
    string subDeviceMac("000D6F0000AA3BD3");
    res = cloudClient->createSubDevice(subDeviceMac, data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}


TEST_F(CloudClientTest, bindSubDevice)
{
    string subDeviceID("EF9BE91457682163");
    res = cloudClient->bindSubDevice(subDeviceID, data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, deleteSubDevice)
{
    string subDeviceID("EF9BE91457682163");
    res = cloudClient->deleteSubDevice(subDeviceID, data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

/*

TEST_F(CloudClientTest, uploadLog)
{
    res = cloudClient->uploadLog(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, putDeviceProperty)
{
    res = cloudClient->putDeviceProperty(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

 */


