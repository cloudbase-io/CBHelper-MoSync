/* Copyright (C) 2012 cloudbase.io

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/
#include "CBHelper.h"

namespace Cloudbase {

CBHelper::CBHelper(String appCode, String appUniq) {
	this->baseInit();
	this->appCode = appCode;
	this->appUniq = appUniq;
}

void CBHelper::baseInit() {
	this->isHttps				= true;
	this->deviceRegistered		= false;
	this->domain				= "api.cloudbase.io";
	this->defaultLogCategory	= "DEFAULT";
	this->language				= "";
	this->deviceUniqueIdentifier= "";

	this->authUsername			= "";
	this->authPassword			= "";

	this->sessionId				= "";

	this->getSystemProperty("mosync.iso-639-1", this->language);
	this->getSystemProperty("mosync.device.UUID", this->deviceUniqueIdentifier);


	this->getSystemProperty("mosync.device.OS", this->deviceModel);
	this->getSystemProperty("mosync.device.OS.version", this->deviceVersion);

	printf("getting OS: %s", this->deviceModel.c_str());
	if (MAUtil::lowerString(this->deviceModel).find("android", 0) >= 0) {
		this->notificationNetwork = "and";
	} else if (MAUtil::lowerString(this->deviceModel).find("ios", 0) >= 0) {
		this->notificationNetwork = "ios";
	} else
		this->notificationNetwork = "winmo";

	this->country = "";
	// TODO: We should be able to find the name of the country from the locale
}

void CBHelper::registerDevice() {
	if (this->deviceRegistered)
		return;
	CBHttpConnection* con = this->createConnection("register-device");

	CBRegisterDevice* params = new CBRegisterDevice(this->notificationNetwork, this->deviceModel, this->deviceVersion, this->language, this->country);

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/register";

	CBRegisterDeviceResponder* resp = new CBRegisterDeviceResponder(this);

	con->sendRequest(url, dynamic_cast<CBSerializable*>(params), resp);
	this->deviceRegistered = true;
}

void CBHelper::writeLog(String line, String category, String level) {
	CBHttpConnection* con = this->createConnection("log");

	CBLogLine* params = new CBLogLine(category, level, line, this->deviceModel, this->deviceVersion);

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/log";

	con->sendRequest(url,  dynamic_cast<CBSerializable*>(params), NULL);
}

void CBHelper::logNavigation(String screenName) {
	if (this->sessionId == "")
		return;

	CBHttpConnection* con = this->createConnection("log-navigation");

	CBNavigation* params = new CBNavigation(this->sessionId, screenName);

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/lognavigation";

	con->sendRequest(url, dynamic_cast<CBSerializable*>(params), NULL);
}

void CBHelper::insertDocument(String collectionName, CBSerializable* data, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("data");

	printf("insert document %s ", data->serialize().c_str());

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/" + collectionName;
	url += "/insert";

	con->sendRequest(url, data, responder, true);
}
void CBHelper::insertDocument(String collectionName, CBSerializable* data, MAUtil::Vector<CBHelperAttachment> atts, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("data");
	con->atts = atts;

	printf("insert document %s ", data->serialize().c_str());

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/" + collectionName;
	url += "/insert";

	con->sendRequest(url, data, responder, true);
}
void CBHelper::searchDocument(String collectionName, CBHelperSearchCondition conditions, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("data");
	//printf("called search");
	//printf("search document %s ", conditions.serialize().c_str());
	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/" + collectionName;
	url += "/search";

	con->sendRequest(url, dynamic_cast<CBSerializable*>(&conditions), responder, true);
}
void CBHelper::downloadFile(String fileId, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("data");
	con->isDownload = true;

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/file/" + fileId;

	con->sendRequest(url, NULL, responder);
}

void CBHelper::subscribeDeviceWithToken(String token, String channel, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("notifications-register");
	String network = "ios"; // TODO: Detect device and assign ios/and/winmo
	CBPushNotification* notif = new CBPushNotification("subscribe", token, channel, network);

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/notifications-register";

	con->sendRequest(url, dynamic_cast<CBSerializable*>(notif), responder);
}
void CBHelper::unsubscribeDeviceWithToken(String token, String channel, bool fromAll, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("notifications-register");
	String network = "ios"; // TODO: Detect device and assign ios/and/winmo
	CBPushNotification* notif = new CBPushNotification("unsubscribe", token, channel, network);
	notif->fromAll = fromAll;

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/notifications-register";

	con->sendRequest(url, dynamic_cast<CBSerializable*>(notif), responder);
}
void CBHelper::sendNotification(String channel, String certType, String notification, String badge, String sound, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("notifications");

	CBPushNotificationMessage* notif = new CBPushNotificationMessage(channel, certType, notification, badge, sound);

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/notifications";

	con->sendRequest(url, dynamic_cast<CBSerializable*>(notif), responder);
}
void CBHelper::sendNotificationEmail(String templateCode, String recipient, String subject, CBSerializable* vars) {
	CBHttpConnection* con = this->createConnection("email");

	CBNotificationEmail* notif = new CBNotificationEmail(templateCode, recipient, subject, vars);

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/email";

	con->sendRequest(url, dynamic_cast<CBSerializable*>(notif), NULL);
}

void CBHelper::executeCloudFunction(String functionCode, MAUtil::Map<String, String>* params, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("cloudfunction");
	con->additionalPostParams = params;

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/cloudfunction/";
	url += functionCode;

	con->sendRequest(url, NULL, responder);
}
void CBHelper::executeApplet(String appletCode, MAUtil::Map<String, String>* params, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("applet");
	con->additionalPostParams = params;

	String url = this->generateURL();
	url += "/" + this->appCode;
	url += "/applet/";
	url += appletCode;

	con->sendRequest(url, NULL, responder);
}

void CBHelper::setPassword(String pwd) {
	this->password = pwd;
	this->registerDevice();
}

void CBHelper::setAuthentication(String username, String password) {
	this->authUsername = username;
	this->authPassword = password;
}

CBHttpConnection* CBHelper::createConnection(String function) {
	CBHttpConnection* con = new CBHttpConnection(this->appCode, this->appUniq, this->password, this->deviceUniqueIdentifier, function);

	if (this->authUsername != "") {
		con->authUsername = this->authUsername;
		con->authPassword = this->authPassword;
	}

	return con;
}

String CBHelper::generateURL() {
	return (this->isHttps?"https":"http") + (String)"://" + this->domain;
}

int CBHelper::getSystemProperty(const char* key, MAUtil::String& dst) {
    int size = maGetSystemProperty(key, NULL, 0);
    if (size < 0)
        return size;
    dst.resize(size-1);
    maGetSystemProperty(key, dst.pointer(), size);
    return size;
}

CBRegisterDeviceResponder::CBRegisterDeviceResponder(CBHelper* helperInstance) { this->helper = helperInstance; }
void CBRegisterDeviceResponder::parseResponse(CBHelperResponseInfo* resp, YAJLDom::Value* responseMessage) {
	printf("responder called. status = %s ", resp->outputString.c_str());
	printf("response message %s", responseMessage->getValueForKey("output")->toString().c_str());
	this->helper->sessionId = responseMessage->getValueForKey("sessionid")->toString();
}

}
