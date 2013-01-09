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
#ifndef CBSERIALIZABLE_H_
#define CBSERIALIZABLE_H_

namespace Cloudbase {

/**
 * This interface should be implemented by all objects that need to be sent and stored in the CloudBase.
 * The serialize() method is called by the CBHelper object when the document needs to be sent to the
 * cloudbase.io REST APIs
 */
class CBSerializable {
public:
	/**
	 * This method should produce a String containing the JSON representation of the current object.
	 * The cloudbase.io APIs are capable of inserting multiple objects in one call and expect the structure
	 * passed to be an array, even if just one object is present.
	 * Therefore this method should always return a String beginning with "[":
	 * [ { "first_name" : "cloud", "last_name" : "base", "title" : ".io" } ]
	 * @return The JSON representation of the current object
	 */
	virtual String serialize() { return "[ {} ]"; };
};

}

#endif
