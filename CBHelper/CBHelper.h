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
#include <MAUtil/String.h>
#include <MAUtil/Util.h>
#include <MAP/LonLat.h>
#include <yajl/YAJLDom.h>
#include <mastdlib.h>
#include <conprint.h>

#include "CBHttpConnection.h"
#include "Convert.h"
#include "DataCommands/CBHelperDataCommandList.h"
#include "DataCommands/CBHelperSearchCondition.h"
#include "CBHelperResponder.h"
#include "CBPayPal.h"

namespace Cloudbase {

/*! \mainpage cloudbase.io MoSync Helper Class Reference
 *
 * \section intro_sec Introduction
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation.<br/><br/>
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.<br/><br/>

 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to the Free
 * Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.<br/><br/>
 *
 * \section install_sec Getting Started
 *
 * The cloudbase.io MoSync helper class compiles to .lib MoSync library. The project
 * needs to be part of your workspace and the CBHelper directory in your path for
 * additional includes.<br/><br/>
 * This full reference is a companion to <a href="/documentation/mosync" target="_blank">
 * the tutorial on the cloudbase.io website<a/>
 */

class CBHelper {
public:
	bool isHttps;
	MAP::LonLat currentLocation;
	String sessionId;

	/**
	* Initializes the new cloudbase.io CBHelper object for the given application
	* @param appCode the application code as registered on cloudbase.io (for example test-application)
	* @param appUniq the unique application code generated by cloudbase.io (df2edf1f2444c3dccc570224915609d5)
	*/
	CBHelper(String appCode, String appUniq);

	/**
	 * Sets the password of the current application. This should be the md5 of the password as specified
	 * when creating the application on cloudbase.io
	 * @param pwd The md5 string representation of the application password on cloudbase.io
	 */
	void setPassword(String pwd);

	void logDebug(String line, String category) { writeLog(line, category, "DEBUG"); };
	void logInfo(String line, String category) { writeLog(line, category, "INFO"); };
	void logWarn(String line, String category) { writeLog(line, category, "WARNING"); };
	void logError(String line, String category) { writeLog(line, category, "ERROR"); };
	void logFatal(String line, String category) { writeLog(line, category, "FATAL"); };
	void logEvent(String line, String category) { writeLog(line, category, "EVENT"); };
	/**
	 * Writes a line to the cloudbase.io application log. This is used for debug purposes as well as to
	 * collect analytics about the application (see log EVENT in the documentation).
	 * A number of shortcut methods are also available: logDebug, logInfo, logWarn, logError, logFatal
	 * and logEvent
	 * @param line The line of text to be written to the log
	 * @param category The category of the line - this is a custom text field that can be used to keep
	 * the log output for different sections of the app segregated
	 * @param level The severity of the line. Possible values are (DEBUG/INFO/WARNING/ERROR/FATAL/EVENT)
	 */
	void writeLog(String line, String category, String level);

	/**
	 * Log the navigation to a new screen
	 * @param viewName An NSString representing the unique name for the loaded UIViewController
	 */
	void logNavigation(String screenName);

	/**
	 * Inserts the given object in a cloudbase collection. If the collection does not exist it is automatically created.
	 * Similarly if the data structure of the given object is different from documents already present in the collection
	 * the structure is automatically altered to accommodate the new object.
	 * The data object implementing the CBSerializable interface should always return a JSon array. This mean it can contain
	 * multiple objects and they will be inserted as multiple rows if needed. Otherwise simply wrap your object withing square
	 * brackets [ ]
	 * Once the call to the APIs is completed the responder is called
	 * @param collectionName The name of the collection (table) in your cloudbase database.
	 * @param data The objec to be inserted in the collection
	 * @param reponder The CBHelperResponder object to handle the response from the cloudbase.io servers
	 */
	void insertDocument(String collectionName, CBSerializable* data, CBHelperResponder* responder = NULL);
	/**
	 * Inserts the given object in a cloudbase collection. If the collection does not exist it is automatically created.
	 * Similarly if the data structure of the given object is different from documents already present in the collection
	 * the structure is automatically altered to accommodate the new object.
	 * The data object implementing the CBSerializable interface should always return a JSon array. This mean it can contain
	 * multiple objects and they will be inserted as multiple rows if needed. Otherwise simply wrap your object withing square
	 * brackets [ ]
	 * Once the call to the APIs is completed the requestCompleted method is triggered in the delegate.
	 * @param collectionName The name of the collection (table) in your cloudbase database.
	 * @param data The objec to be inserted in the collection
	 * @param atts A Vector of CBHelperAttachment objects representing the files to be attached to the request
	 * @param reponder The CBHelperResponder object to handle the response from the cloudbase.io servers
	 */
	void insertDocument(String collectionName, CBSerializable* data, MAUtil::Vector<CBHelperAttachment> atts = NULL, CBHelperResponder* responder = NULL);
	/***
	 * Runs a search over a collection with the given criteria. The documents matching the search criteria are then
	 * returned in the CBHelperResponseInfo object passed to the responder.
	 * @param collection The name of the collection to run the search over
	 * @param conditions A set of search conditions
	 * @param responder The CBHelperResponder object to handle the response from the cloudbase.io servers
	 */
	void searchDocument(String collectionName, CBHelperSearchCondition conditions, CBHelperResponder* responder = NULL);
	// TODO: Implement the update method. Issue is that at the moment we expect the cb_sarrch_key to specify
	// the conditions for the update statement - these need to be added to the document after it's been serialized

	/**
	 * Runs a search over a collection and applies the given list of aggregation commands to the output.
	 * @param collectionName The name of the collection to run the search over
	 * @param commands A List of CBDataAggregationCommand objects
	 * @param responder a block of code to be executed once the request is completed
	 */
	void searchDocumentAggregate(String collectionName, CBHelperDataCommandList commands, CBHelperResponder* responder = NULL);
	/**
	 * Downloads a file attached to a document in the CloudBase database using the file_id generated once the file is uploaded.
	 * More information on file attachments is available here http://cloudbase.io/documentation/rest-apis#CloudBase
	 * @param fileId The cloudbase.io id generated for the file
	 * @param responder A responder implementing the fileDownloaded(CBHelperAttachment* fileData) method.
	 */
	void downloadFile(String fileId, CBHelperResponder* responder = NULL);

	/**
	 * Subscribes the devices with the current token received from the manufacturer to a notification channel. All devices are
	 * autmatically subscribed to the channel <strong>all</strong>.
	 * @param token The token returned by the Apple notification services or C2DM.
	 * @param channel The name of the channel the device is subscribing to - if the channel doesn't exist it is
	 * automatically created
	 * @param responder The CBHelperResponder object to handle the response from the cloudbase.io servers
	 */
	void subscribeDeviceWithToken(String token, String channel, CBHelperResponder* responder = NULL);
	/**
	 * Unsubscribes a device from a notification channel.
	 * @param token The token returned by the Apple notification services or C2DM.
	 * @param channel The name of the channel the device is unsubscribing from
	 * @param fromAll Whether it should also be removed from the "all" channel
	 * @param responder The CBHelperResponder object to handle the response from the cloudbase.io servers
	 */
	void unsubscribeDeviceWithToken(String token, String channel, bool fromAll, CBHelperResponder* responder = NULL);
	/**
	 * Sends a push notification to a specific channel. Push notifications from client devices need to be enabled
	 * in the application security settings in your cloudbase.io control panel.
	 * @param channel The name of the channel to notify
	 * @param certType The Apple certiicate type for the notification (production/development)
	 * @param notification The text of the notification
	 * @param badge The ID of the badge to be used for the notification
	 * @param sound The name of the sound to be used for the notification
	 * @param responder The CBHelperResponder object to handle the response from the cloudbase.io servers
	 */
	void sendNotification(String channel, String certType, String notification, String badge, String sound, CBHelperResponder* responder = NULL);
	/**
	 * Sends an email to a recipient using a template previously created on cloudbase.io
	 * @param templateCode The code assigned to the template
	 * @param recipient The email address of the recipient
	 * @param subject The subject of the email notification
	 * @param vars A Map of variables to populate the template. The serialized String of this object should
	 * look something like this: { "first_name" : "cloud", "last_name" : "base", "title" : ".io" }
	 */
	void sendNotificationEmail(String templateCode, String recipient, String subject, CBSerializable* vars = NULL);

	/**
	 * Executes a CloudFunction declared on cloudbase.io
	 * @param functionCode The code assigned to the function
	 * @param params If necessary the parameters for the function - these will be received by your code on the
	 * cloudbase.io server as additional $_POST parameters.
	 * @param responder The CBHelperResponder object to handle the response from the cloudbase.io servers which
	 * includes the output from your CloudFunction
	 */
	void executeCloudFunction(String functionCode, MAUtil::Map<String, String> params, CBHelperResponder* responder = NULL);
	/**
	 * Executes a cloudbase.io Applet
	 * @param appletCode The code for the applet
	 * @param params Additional parameters for the Applet
	 * @param responder The CBHelperResponder object to handle the response from the cloudbase.io servers which
	 * contains the output from the Applet
	 */
	void executeApplet(String appletCode, MAUtil::Map<String, String> params, CBHelperResponder* responder = NULL);

	/**
	 * Initiates a transaction with PayPal by sending the payment details and retrieving a token
	 * and an express checkout url. The url returned should be then opened in a browser window.
	 * @param purchaseDetails a populated CBPayPalBill object
	 * @param isLiveEnvironment whether we are using the production or sandbox paypal environments
	 * @param responder a responder to handle the returned PayPal token and submission url
	 */
	void preparePayPalPurchase(CBPayPalBill purchaseDetails, bool isLiveEnvironment, CBHelperResponder* responder = NULL);

	/**
	 * Once the PayPal purchase is completed this method updates the record in the cloudbase.io database.
	 * The responder can then proceed to close the payment window using the output of the call
	 * @param url The url returned by PayPal once the payment is completed
	 * @param responder The responder to complete the payment in the application
	 */
	void completePayPalPurchase(String url, CBHelperResponder* responder = NULL);

	/**
	 * If client authentication is enabled in the cloudbase.io settings then this function sets the username
	 * and password of the current user in the application. The user will be authenticated against the collection
	 * specified in the cloudbase.io security settings for the application
	 * @param username The username of the user logged onto the application
	 * @param password The password of the user - if the password are stored in encrypted format during the registration
	 *  then this string should be encrypted in exactly the same way.
	 */
	void setAuthentication(String username, String password);

protected:
	String appCode;
	String appUniq;
	String password;

	String domain;
	String defaultLogCategory;

	String country;
	String language;

	String deviceUniqueIdentifier;
	String deviceModel;
	String deviceVersion;
	String notificationNetwork;
	//String deviceLanguage;

	String authUsername;
	String authPassword;

	void registerDevice();
	CBHttpConnection* createConnection(String function);
private:
	bool deviceRegistered;

	void baseInit();
	String generateURL();
	int getSystemProperty(const char* key, MAUtil::String& dst);
};

class CBRegisterDevice : public CBSerializable {
public:
	CBRegisterDevice(String deviceType, String deviceName, String deviceModel, String language, String country) :
		deviceType_(deviceType), deviceName_(deviceName), deviceModel_(deviceModel), language_(language), country_(country) {}

	String serialize() {
		String out = "{";
		out += "\"device_type\" : \"" + deviceType_ + "\", ";
		out += "\"device_name\" : \"" + deviceName_ + "\", ";
		out += "\"device_model\" : \"" + deviceModel_ + "\", ";
		out += "\"language\" : \"" + language_ + "\", ";
		out += "\"country\" : \"" + country_ + "\" } ";

		return out;
	}
private:
	String deviceType_;
	String deviceName_;
	String deviceModel_;
	String language_;
	String country_;
};

class CBPushNotification : public CBSerializable {
public:
	CBPushNotification(String action, String token, String channel, String network) :
		action_(action), deviceToken(token), deviceChannel(channel), deviceNetwork(network), fromAll(false) {};

	String serialize() {
		String out = "{";
		out += "\"action\" : \"" + action_ + "\", ";
		out += "\"device_key\" : \"" + deviceToken + "\", ";
		out += "\"device_network\" : \"" + deviceNetwork + "\", ";
		out += "\"channel\" : \"" + deviceChannel + "\" ";

		if (fromAll) {
			//String isAll = (fromAll?"true":"false")l
			out += ", \"from_all\" : \"true\" ";
		}

		out += " } ";
		return out;
	};

	bool fromAll;
private:
	String action_;
	String deviceToken;
	String deviceChannel;
	String deviceNetwork;
};

class CBRegisterDeviceResponder : public CBHelperResponder {
public:
	CBRegisterDeviceResponder(CBHelper* helperInstance);
	void parseResponse(CBHelperResponseInfo* resp, YAJLDom::Value* responseMessage);
private:
	CBHelper* helper;
};

class CBLogLine : public CBSerializable {
public:
	CBLogLine(String category, String level, String logLine, String deviceName, String deviceModel) :
		category_(category), level_(level), logLine_(logLine), deviceName_(deviceName), deviceModel_(deviceModel) {};

	String serialize() {
		String out = "{";
		out += "\"category\" : \"" + category_ + "\", ";
		out += "\"level\" : \"" + level_ + "\", ";
		out += "\"log_line\" : \"" + logLine_ + "\", ";
		out += "\"device_name\" : \"" + deviceName_ + "\", ";
		out += "\"device_model\" : \"" + deviceModel_ + "\" } ";

		return out;
	}
private:
	String category_;
	String level_;
	String logLine_;
	String deviceName_;
	String deviceModel_;
};

class CBPushNotificationMessage : public CBSerializable {
public:
	CBPushNotificationMessage(String channel, String certType, String notification, String badge, String sound) :
		channel_(channel), certificateType_(certType), notification_(notification), badge_(badge), sound_(sound) {};

	String serialize() {
		String out = "{";
		out += "\"channel\" : \"" + channel_ + "\", ";
		out += "\"cert_type\" : \"" + certificateType_ + "\", ";
		out += "\"alert\" : \"" + notification_ + "\", ";
		out += "\"badge\" : \"" + badge_ + "\", ";
		out += "\"sound\" : \"" + sound_ + "\" } ";

		return out;
	}
private:
	String channel_;
	String certificateType_;
	String notification_;
	String badge_;
	String sound_;
};

class CBNotificationEmail : public CBSerializable {
public:
	CBNotificationEmail(String templateCode, String recipient, String subject, CBSerializable* vars) :
		template_(templateCode), recipient_(recipient), subject_(subject), vars_(vars) {};

	String serialize() {
		String out = "{";
		out += "\"template_code\" : \"" + template_ + "\", ";
		out += "\"recipient\" : \"" + recipient_ + "\", ";
		out += "\"subject\" : \"" + subject_ + "\", ";
		out += "\"variables\" : \"" + vars_->serialize() + "\" } ";

		return out;
	}
private:
	String template_;
	String recipient_;
	String subject_;
	CBSerializable* vars_;
};

class CBNavigation : public CBSerializable {
public:
	CBNavigation(String sessionId, String screenName) :
		sessionId_(sessionId), screenName_(screenName) {};

	String serialize() {
		String out = "{";
		out += "\"session_id\" : \"" + sessionId_ + "\", ";
		out += "\"screen_name\" : \"" + screenName_ + "\" } ";

		return out;
	}
private:
	String sessionId_;
	String screenName_;
};

class CBPayPalPurchase : public CBSerializable {
public:
	CBPayPalPurchase(CBPayPalBill bill, bool live, String type) :
		bill_(bill), liveEnv_(live), currency_(bill.currency), type_(type),
		completedF_(bill.paymentCompletedFunction), cancelledF_(bill.paymentCancelledFunction),
		completedUrl_(bill.paymentCompletedUrl), cancelledUrl_(bill.paymentCancelledUrl) {};

	String serialize() {
		String out = "{";
		out += "\"environment\" : \"";
		out += (liveEnv_?"live":"sandbox");
		out += "\", ";
		out += "\"currency\" : \"" + currency_ + "\", ";
		out += "\"type\" : \"" + type_ + "\", ";
		out += "\"completed_cloudfunction\" : \"" + completedF_ + "\", ";
		out += "\"cancelled_cloudfunction\" : \"" + cancelledF_ + "\", ";
		out += "\"payment_completed_url\" : \"" + completedUrl_ + "\", ";
		out += "\"payment_cancelled_url\" : \"" + cancelledUrl_ + "\", ";

		out += "\"purchase_details\" : " + bill_.serialize();
		out += " } ";

		return out;
	}
private:
	CBPayPalBill bill_;
	bool liveEnv_;
	String currency_;
	String type_;
	String completedF_;
	String cancelledF_;
	String completedUrl_;
	String cancelledUrl_;
};

}
