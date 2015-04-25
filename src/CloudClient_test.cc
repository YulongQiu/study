#include "gtest/gtest.h"
#include "gmock/gmock.h"

//#include "../src/network/cloud/CloudClient.h"
#include "CloudClient.h"

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

/* TEST_F(CloudClientTest, getDeviceToken)
{
    res = cloudClient->getDeviceToken(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
} */

TEST_F(CloudClientTest, getUserInfo)
{

    res = cloudClient->getUserInfo(data);

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

TEST_F(CloudClientTest, checkUbootVersion)
{
    Json::Value currentVersion;
    currentVersion["cmd"] = "ubootVersion";
    //Json::Value versionNo["versionNO"] = "4.1.2.1";
    currentVersion["data"]["versionNO"] = "4.1.2.1";
    //std::cout << currentVersion.toStyledString() << std::endl;
    std::string version = currentVersion.toString();
    res = cloudClient->getVersion(version, data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
    printData();
}


/*
TEST_F(CloudClientTest, getQRCode)
{
    res = cloudClient->getQRCode(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, getMQConfig)
{
    res = cloudClient->getMQConfig(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, getLocation)
{
    res = cloudClient->getLocation(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, getDeviceProperty)
{
    res = cloudClient->getDeviceProperty(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, putDeviceProperty)
{
    res = cloudClient->putDeviceProperty(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, registerDevice)
{
    res = cloudClient->registerDevice(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, uploadHeartbeat)
{
    res = cloudClient->uploadHeartbeat(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, uploadLog)
{
    res = cloudClient->uploadLog(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, downloadFile)
{
    res = cloudClient->downloadFile(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST_F(CloudClientTest, factoryReset)
{
    res = cloudClient->factoryReset(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}


TEST_F(CloudClientTest, createSubDevice)
{
    res = cloudClient->createSubDevice(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}


TEST_F(CloudClientTest, deleteSubDevice)
{
    res = cloudClient->deleteSubDevice(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
} */
