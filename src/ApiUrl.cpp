#include "ApiUrl.h"

ApiUrl::ApiUrl()
: _apiUrl("http://qdapi.atsmart.io/v2")
{

}

ApiUrl::~ApiUrl()
{

}

string ApiUrl::getTokenUrl()
{
	return _apiUrl + "/auth/device/token";
}

string ApiUrl::getWebValidUrl()
{
	return _apiUrl + "/device/datetime";
}

string ApiUrl::getDateTimeUrl()
{
	return _apiUrl + "/device/datetime";
}

string ApiUrl::getUserInfoUrl()
{
	return _apiUrl+"/device/userlist";
}

string ApiUrl::getDevicePropertyUrl()
{
	return _apiUrl + "/device/property";
}

string ApiUrl::getDeviceSettingUrl()
{
	return _apiUrl + "/device/setting";
}

string ApiUrl::getSubDeviceUrl()
{
	return _apiUrl+"/device/sub";
}

string ApiUrl::getVersionUrl()
{
	return _apiUrl+"/device/checkVersion";
}

string ApiUrl::getQRCodeUrl()
{
	return _apiUrl+"/device_qrcode";
}

string ApiUrl::getTriggerUrl()
{
	return _apiUrl + "/device/trigger";
}

string ApiUrl::getFactoryResetUrl()
{
	return _apiUrl+"/device/factoryreset";
}

string ApiUrl::getDiagnoseInfoUrl()
{
	return _apiUrl + "/uploadfile";
}

string ApiUrl::getHeartbeatUrl()
{
	return _apiUrl + "/device/heartbeat";
}

string ApiUrl::getBatchHeartbeatUrl()
{
	return _apiUrl + "/device/batch/heartbeat";
}

string ApiUrl::getCloudCmdUrl()
{
	return _apiUrl+"/chk/device/data/ret/command";
}

string ApiUrl::getMQConfigUrl()
{
	return _apiUrl+"/config/mqinfo";
}

string ApiUrl::getSubDeviceOfflineUrl()
{
	return _apiUrl+"/device/offline";
}

string ApiUrl::getNoticeUserUrl()
{
	return _apiUrl+"/mq/pub2user";
}

string ApiUrl::getNoticeConfigUrl()
{
	return _apiUrl+"/config/mqinfo";
}

/* string ApiUrl::getLocationUrl()
{
    return _apiUrl+"/device/location";
} */
