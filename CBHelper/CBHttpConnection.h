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
#include <MAUtil/Connection.h>
#include <MAUtil/String.h>
#include <MAUtil/Util.h>
#include <MAP/LonLat.h>
#include <yajl/YAJLDom.h>
#include <mastdlib.h>
#include <conprint.h>
#include <MAUtil/DataHandler.h>

#include "Convert.h"
#include "CBHelperResponder.h"
#include "CBSerializable.h"
#include "CBHelperAttachment.h"
#include "CBHttpConnectionParameter.h"

#define CONNECTION_BUFFER_SIZE 40000
#define REQUEST_PARAMETER_BOUNDARY "---------------------------14737809831466499882746641449"
#define CBHELPER_USER_AGENT "MoSync-CBHelper-0.1a"

namespace Cloudbase {

/**
 * This class is used internally to execute calls to the cloudbase.io APIs. Each request is self-contained within a
 * CBHttpConnection object. This way the CBHelper object can execute multiple requests at the same time.
 */
class CBHttpConnection : private HttpConnectionListener {
public:
	CBHelperResponder* CBResponder; /// The responder for the response
	MAP::LonLat currentLocation; /// The current location property as per the CBHelper object. If this is set then the location info is sent
	String authUsername; /// The current username and password for the user interacting with the application - if these security settings are setup
	String authPassword;
	MAUtil::Vector<CBHelperAttachment> atts; /// An array of file attachments for the request
	MAUtil::Map<String, String> additionalPostParams; /// Additional post parameters. This is used by the CloudFunctions and Applet APIs
	bool isDownload; /// whether the object has been created to download a file attachment or call the APIs

	/**
	 * Creates a new CBHttpConnection object. Additional parameters may be set in the object after it's been initialised such as the responder,
	 * the Vector of attachments and the eventual additional post parameters
	 * @param appCode The application code
	 * @param appUniq The unique application code
	 * @param password The MD5 of the application password
	 * @param deviceUniq The unique identifier for the device
	 * @param function The cloudbase.io function being called
	 */
	CBHttpConnection(String appCode, String appUniq, String password, String deviceUniq, String function);

	void sendRequest(String url,  CBSerializable* parameters, CBHelperResponder* responder, bool isArray = false);

	void connWriteFinished (Connection *conn, int result);
	void httpFinished(MAUtil::HttpConnection *conn, int result);
	void connReadFinished (Connection *conn, int result);
	void connRecvFinished (Connection *conn, int result);
protected:
	/**
	 * Parses the response output. Extracts the message data from the JSON and creates the new CBHelperResponseInfo object.
	 * If a CBHelperResponder was given to the class then the parseResponse method is triggered from here once the response
	 * has been validated
	 * @param statusCode The http status code
	 * @param function The cloudbase.io function being called
	 * @param responder The responder object specified for the CBHttpConnection
	 */
	void parseResponseOutput(int statusCode, String function, CBHelperResponder* responder);
private:
	String appCode;
	String appUniq;
	String password;
	String function;
	String deviceUniqueIdentifier;

	String normalisedParameters;

	// temporary counter to submit the connection once everything is written
	int totWrites;
	int tmpWrites;

	HttpConnection theConnection;
	char mBuffer[CONNECTION_BUFFER_SIZE];
	MAHandle downloadFileData;
	String responseOutputString;
	MAUtil::Vector<CBHttpConnectionParameter> parameters;
	//static unsigned char fileData[65536];
	bool isConnected;
	bool fileBody;
	bool fileFinished;
	bool doneWithParameters;

	/**
	 * Writes the request with all of the standard parameters and additional attachments and post params.
	 * @param parameters The object to be sent in the post_data parameter
	 * @param isArray Whether the object is an array or not (not used at the moment)
	 */
	void prepareParameters(CBSerializable* parameters, bool isArray);
	void buildParamBody(String paramName, String paramValue);
	void buildFileBody(CBHelperAttachment att, int counter);
	int calculateRequestSize();
	int getParamBodySize(String paramName, String paramValue);
	int getFileBodySize(CBHelperAttachment att, int counter);
	void writeElement(int index);
};
}
