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
        cloudClient = new CloudClient("2F1F338D1064DBCF", "CE3EAEE20D55D85D", "D116BF8A2CCE15F2");
        std::string token;
        cloudClient->getDeviceToken(token);
    }

    virtual void TearDown()
    {
        delete cloudClient;
    }

    void printData();
    // std::string getDMAGIC();


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

/* std::string CloudClientTest::getDMAGIC()
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
} */
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
*/


TEST_F(CloudClientTest, getDeviceProperty)
{
    res = cloudClient->getDeviceProperty(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}
/*
TEST_F(CloudClientTest, putDeviceProperty)
{
    res = cloudClient->putDeviceProperty(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, registerDevice)
{
    res = cloudClient->registerDevice(data);

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

TEST_F(CloudClientTest, uploadLog)
{
    res = cloudClient->uploadLog(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}

TEST_F(CloudClientTest, downloadFile)
{
    res = cloudClient->downloadFile(data);

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


TEST_F(CloudClientTest, createSubDevice)
{
    res = cloudClient->createSubDevice(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}


TEST_F(CloudClientTest, deleteSubDevice)
{
    res = cloudClient->deleteSubDevice(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
} */
