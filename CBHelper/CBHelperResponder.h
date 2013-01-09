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
#include "CBHelperAttachment.h"

namespace Cloudbase {

#ifndef CBRESPONSEINFO_H_
#define CBRESPONSEINFO_H_

/**
 * A container for the data returned from the cloudbase.io server. This is handed over as a representation
 * of a response to objects implementing the CBHelperResponder interface.
 */
class CBHelperResponseInfo {
public:
	String function; /// The function from cloudbase.io (data/notifications/CloudFunction/Applet etc)
	String errorMessage; /// An error message, if any, returned by cloudbase.io
	String outputString; /// The full String representation of the response from the cloudbase.io servers
	int httpStatusCode; /// The status code for the call
	bool postSuccess; /// Whether the request was successfull
};

#endif

#ifndef CBRESPONDER_H_
#define CBRESPONDER_H_

class CBHelperResponder {
public:
	/**
	* Called when a connection to the cloudbase.io APIs is completed and the data is ready.
	* @param respData A CBHelperResponseInfo object populated with the data from the API request
	* @param result \> 0 on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	virtual void parseResponse(CBHelperResponseInfo resp, YAJLDom::Value* responseMessage) { return; };

	/**
	 * This method is called once a file has been downloaded using the cloudbase.io file id.
	 * The CBHelperAttachment object will only contain the MAHandle data handle pointing to the downloaded file data
	 * @param fileData The CBHelperAttachment containing the MAHandle data handle.
	 */
	virtual void fileDownloaded(CBHelperAttachment fileData) { return; };
};

#endif

}
