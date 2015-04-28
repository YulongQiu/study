#ifndef API_URL_H
#define API_URL_H

#include <string>
using namespace std;

class ApiUrl
{
public:
	ApiUrl();
	~ApiUrl();
	string getTokenUrl();
	string getWebValidUrl();
	string getDateTimeUrl();
	string getUserInfoUrl();
	string getDevicePropertyUrl();
	string getDeviceSettingUrl();
	string getSubDeviceUrl();
	string getVersionUrl();
	string getQRCodeUrl();
	string getTriggerUrl();
	string getFactoryResetUrl();
	string getDiagnoseInfoUrl();
	string getHeartbeatUrl();
	string getBatchHeartbeatUrl();
	string getCloudCmdUrl();
	string getSubDeviceOfflineUrl();
	string getNoticeUserUrl();
	string getNoticeConfigUrl();
    string getMQConfigUrl();
    //string getLocationUrl();

private:
	string _apiUrl;
};

#endif
