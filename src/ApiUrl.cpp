#include "ApiUrl.h"

ApiUrl()
: _apiUrl("http://api.atsmart.io/v2")
{

}

~ApiUrl()
{

}

string getTokenUrl()
{
	return _apiUrl + "/auth/device/token";
}

string getWebValidUrl()
{
	return _apiUrl + "/device/datetime";
}

string getDateTimeUrl()
{
	return _apiUrl + "/device/datetime";
}

string getUserInfoUrl()
{
	return _apiUrl+"/device/userlist";
}

string getDevicePropertyUrl()
{
	return _apiUrl + "/device/property";
}

string getDeviceSettingUrl()
{
	return _apiUrl + "/device/setting";
}

string getSubDeviceUrl()
{
	return _apiUrl+"/device/sub";
}

string getVersionUrl()
{
	return _apiUrl+"/device/checkVersion";
}

string getQRCodeUrl();
{
	return _apiUrl+"/device_qrcode";
}

string getTriggerUrl()
{
	return _apiUrl + "/device/trigger";
}

string getFactoryResetUrl()
{
	return _apiUrl+"/device/factoryreset";
}

string getDiagnoseInfoUrl()
{
	return _apiUrl + "/uploadfile";
}

string getHeartbeatUrl()
{
	return _apiUrl + "/device/heartbeat";
}

string getBatchHeartbeatUrl()
{
	return _apiUrl + "/device/batch/heartbeat";
}

string getCloudCmdUrl()
{
	return _apiUrl+"/chk/device/data/ret/command";
}

string getConfigInfoUrl()
{
	return _apiUrl+"/config/mqinfo";
}

string getSubDeviceOfflineUrl()
{
	return _apiUrl+"/device/offline";
}

string getNoticeUserUrl()
{
	return _apiUrl+"/mq/pub2user";
}

string getNoticeConfigUrl()
{
	return _apiUrl+"/config/mqinfo";
}
