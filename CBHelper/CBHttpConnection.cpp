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
#include "CBHttpConnection.h"

namespace Cloudbase {

CBHttpConnection::CBHttpConnection(String appCode, String appUniq, String password, String deviceUniq, String function) :
		theConnection(this), isConnected(false), totWrites(0), tmpWrites(0), isDownload(false) {
	this->appCode = appCode;
	this->appUniq = appUniq;
	this->password = password;
	this->deviceUniqueIdentifier = deviceUniq;
	this->function = function;
	this->totWrites = 0;
	this->tmpWrites = 0;

	this->authPassword = "";
	this->authUsername = "";
}

void CBHttpConnection::buildParamBody(String paramName, String paramValue) {
	String output = "--";
	output += REQUEST_PARAMETER_BOUNDARY;
	output += "\r\n";
	output += "content-disposition: form-data; name=\"" + paramName;
	output += "\"\r\n\r\n";
	output += paramValue + "\r\n";
	//this->theConnection.write(output.c_str(), output.length());
	//this->parameters.add(output);
	CBHttpConnectionParameter param;
	param.name = paramName;
	param.value = output;
	param.isFile = false;

	this->parameters.add(param);
	this->totWrites++;
}

// writes a file data to the multipart request
void CBHttpConnection::buildFileBody(CBHelperAttachment att, int counter) {
	String output = "--";
	output += REQUEST_PARAMETER_BOUNDARY;
	output += "\r\n";
	output += "content-disposition: attachment; name=\"file_" + counter;
	output += "\"; filename=\"" + att.fileName;
	output += "\"\r\n\r\n";

	CBHttpConnectionParameter fileIntro;
	fileIntro.name = att.fileName;
	fileIntro.value = output;
	fileIntro.isFile = false;

	this->parameters.add(fileIntro);
	this->totWrites++;

	CBHttpConnectionParameter param;
	param.name = att.fileName;
	param.fileData = att.fileData;
	param.isFile = true;

	this->parameters.add(param);
	this->totWrites++;

	CBHttpConnectionParameter fileClose;
	fileClose.name = att.fileName;
	fileClose.value = "\r\n";
	fileClose.isFile = false;

	this->parameters.add(fileClose);
	this->totWrites++;

	/*
	this->theConnection.write(output.c_str(), output.length());
	// Write the request separator

	// write the content of the file to the request using the MAHandle from the CBHelperAttachment object.
	// this needs to be a data handler and not a file handler.
	this->theConnection.writeFromData(att.fileData, 0, maGetDataSize(att.fileData));

	String closing = "\r\n";
	this->theConnection.write(closing.c_str(), closing.length());
	*/
}

void CBHttpConnection::prepareParameters(CBSerializable* parameters, bool isArray) {
	// The spacer element and log lines have been added here because of issues with Android not syncronizing the
	// writes to the connection correctly.
	this->buildParamBody("app_uniq", this->appUniq);
	this->buildParamBody("app_pwd", this->password);
	this->buildParamBody("device_uniq", this->deviceUniqueIdentifier);

	// If we have authentication parameters write them to the request. For documentation on this see
	// the cb_auth_user and password parameters in the rest-apis documentation http://cloudbase.io/documentation/rest-apis
	if (this->authUsername != "") {
		this->buildParamBody("cb_auth_user", this->authUsername);
		this->buildParamBody("cb_auth_password", this->authPassword);
	}

	// If we have the current location set in the helper class then send the data
	if (this->currentLocation.isValid()) {
		MAUtil::String location = "";

		location += "{ \"lat\" : \"" + doubleToString(this->currentLocation.lat, 6);
		location += "\", \"lng\" : \"" + doubleToString(this->currentLocation.lon, 6);
		location += "\" }";
		this->buildParamBody("location_data", location);
	}

	// Serialize the data object and attach it to the request if not null
	if (parameters != NULL) {
		String jsonOutput = parameters->serialize().c_str();

		this->buildParamBody("post_data", jsonOutput);
	}

	// Loop over the Map of additional paramters and append them as standard request parameters
	if (this->additionalPostParams.size() > 0) {
		for(Map<String, String>::Iterator iter = this->additionalPostParams.begin();
				iter != this->additionalPostParams.end();
			    iter++) {
			this->buildParamBody(iter->first, iter->second);
		}
	}

	// Attach files
	if (this->atts.size() > 0) {
		for (int i = 0; i < this->atts.size(); i++) {
			if(this->atts[i].fileData < 0) {
				printf("Invalid file handle %i\n", this->atts[i].fileData);
			} else {
				this->buildFileBody(this->atts[i], i);
			}
		}
	}

	// close the request
	String output = "";
	output += "--";
	output += REQUEST_PARAMETER_BOUNDARY;
	output += "--\r\n";

	CBHttpConnectionParameter closeRequest;
	closeRequest.name = "closing";
	closeRequest.value = output;
	closeRequest.isFile = false;

	this->parameters.add(closeRequest);
	this->totWrites++;
}

void CBHttpConnection::writeElement(int index) {
	CBHttpConnectionParameter param = this->parameters[index];

	if (param.isFile) {
		theConnection.writeFromData(param.fileData, 0, maGetDataSize(param.fileData));
	} else {
		theConnection.write(param.value.c_str(), param.value.length());
	}
}

// This method runs the same commands as the prepareParameters but returns the size of the request
// to be set in the httpConnection header
int CBHttpConnection::calculateRequestSize() {
	int bodySize = 0;

	for (int i = 0; i < this->parameters.size(); i++) {
		CBHttpConnectionParameter par = this->parameters[i];
		if (par.isFile)
			bodySize += maGetDataSize(par.fileData);
		else
			bodySize += par.value.length();
	}

	return bodySize;
}

void CBHttpConnection::sendRequest(String url,  CBSerializable* parameters, CBHelperResponder* responder, bool isArray) {
	if (isConnected) {
		printf("already connected");
		return;
	}
	int res = theConnection.create(url.c_str(), HTTP_POST);

	if (res < 0) {
		printf("No connection");
	} else {
		theConnection.setListener(this);
		theConnection.setRequestHeader("User-Agent", CBHELPER_USER_AGENT);
		String contentType = "multipart/form-data; boundary=";
		contentType += REQUEST_PARAMETER_BOUNDARY;
		theConnection.setRequestHeader("Content-Type", contentType.c_str());

		this->prepareParameters(parameters, isArray);

		// use the accessory method to calculate the size of the request
		String length = Convert::toString(this->calculateRequestSize());
		theConnection.setRequestHeader("content-length", length.c_str());
		this->CBResponder = responder; // set the shared responder to be called once the response is fully received
		//start writing parameters
		this->writeElement(0);
	}
}

void CBHttpConnection::connWriteFinished (Connection *conn, int result) {
	this->tmpWrites++;
	//printf("tot writes: %i, current %i", this->totWrites, this->tmpWrites);
	if (this->tmpWrites == this->totWrites) // if we have written all of the parameters we needed to write then post
		theConnection.finish();
	else
		this->writeElement(this->tmpWrites);
}


void CBHttpConnection::connReadFinished (Connection *conn, int result) {
	if (this->isDownload) {
		CBHelperAttachment att;
		att.fileData = this->downloadFileData;

		if (this->CBResponder != NULL) {
			this->CBResponder->fileDownloaded(att);
		}
	} else
		this->parseResponseOutput(result, this->function, this->CBResponder); // we are done reading the response. parse it and close the connection.
	theConnection.close();
}

void CBHttpConnection::connRecvFinished(Connection* conn, int result) {
	if(result > 0) {
		if (this->isDownload) {
			conn->recvToData(downloadFileData, 0, CONNECTION_BUFFER_SIZE);
		} else {
			responseOutputString.append(mBuffer, result);
			memset(this->mBuffer, 0, CONNECTION_BUFFER_SIZE);
			theConnection.recv(mBuffer, CONNECTION_BUFFER_SIZE);
		}
	} else
		printf("Error retrieving response %i", result);
}


void CBHttpConnection::httpFinished(MAUtil::HttpConnection *conn, int result) {
	if ( result >= 0) {
		String contentLengthStr;
		theConnection.getResponseHeader("Content-Length",
			&contentLengthStr);
		int contentLength = 0;
		contentLength = atoi(contentLengthStr.c_str());
		//printf("received content length: %s", contentLengthStr.c_str());
		mBuffer[contentLength] = 0;
		if (this->isDownload) {
			this->downloadFileData = maCreatePlaceholder();
			maCreateData(this->downloadFileData,(contentLength == NULL?CONNECTION_BUFFER_SIZE:contentLength));
			theConnection.readToData(this->downloadFileData, 0, (contentLength == NULL?CONNECTION_BUFFER_SIZE:contentLength));
		} else {
			theConnection.read(mBuffer, (contentLength == NULL?CONNECTION_BUFFER_SIZE:contentLength));
		}
	} else {
		theConnection.close();
	}
}

void CBHttpConnection::parseResponseOutput(int statusCode, String function, CBHelperResponder* responder) {
	CBHelperResponseInfo resp;

	resp.httpStatusCode = statusCode;
	resp.function = function;
	resp.outputString = mBuffer;
	//printf("buffer size: %i", sizeof(mBuffer));
	YAJLDom::Value* root = YAJLDom::parse(
	        (const unsigned char*)mBuffer,
	        sizeof(mBuffer));

	if (NULL == root || YAJLDom::Value::NUL == root->getType()) {
		printf("no tree");
		resp.errorMessage = "Error while parsing JSON response";
	} else {
		YAJLDom::Value* functionData = root->getValueForKey(function);

		//printf("got the tree for the function");
		if (NULL == functionData || YAJLDom::Value::MAP != functionData->getType()) {
			printf("error getting function data");
			resp.errorMessage = "Could not find function data in response";
		} else {
			//printf("total child %i", functionData->getNumChildValues());
			//printf(functionData->toString().c_str());
			resp.postSuccess = (functionData->getValueForKey("status")->toString() == "OK");
			resp.errorMessage = functionData->getValueForKey("error")->toString();
			if (NULL != responder) {
				//printf("have responder");
				responder->parseResponse(resp, functionData->getValueForKey("message"));
			}
		}
		// TODO: we should probably clean up the YAJLDom object here - Unfortunately there is an issue
		// when the responder tries to use it as it would have been already closed
	}
}

}



