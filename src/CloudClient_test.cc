#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../src/network/cloud/CloudClient.h"

class CloudClientTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        res = 0;
        data = "";
        cloudClient = new CloudClient("2F1F338D1064DBCF", "CE3EAEE20D55D85D", "D116BF8A2CCE15F2");
    }

    virtual void TearDown()
    {
        delete cloudClient;
    }

protected:
    int res;
    std::string data;
    CloudClient *cloudClient;
};

TEST_F(CloudClientTest, getDeviceToken)
{
    res = cloudClient->getDeviceToken(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

/* TEST(CloudClient, getDateTime)
{
    CloudClient cloudClient("2F1F338D1064DBCF", "CE3EAEE20D55D85D", "D116BF8A2CCE15F2");
    std::string data = "";
    int res = cloudClient.getDateTime(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST(CloudClient, getVersion)
{
    CloudClient cloudClient("2F1F338D1064DBCF", "CE3EAEE20D55D85D", "D116BF8A2CCE15F2");
    std::string data = "";
    int res = cloudClient.getVersion(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
}

TEST(CloudClient, getUserInfo)
{
    CloudClient cloudClient("2F1F338D1064DBCF", "CE3EAEE20D55D85D", "D116BF8A2CCE15F2");
    std::string data = "";
    int res = cloudClient.getUserInfo(data);

    EXPECT_EQ(res, 0);
    EXPECT_NE(data, "");
} */


/* int getDeviceToken(string &replayData);
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
int deleteSubDevice(); */


