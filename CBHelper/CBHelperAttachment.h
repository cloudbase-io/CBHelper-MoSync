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
#ifndef CBHELPERATTACHMENT_H_
#define CBHELPERATTACHMENT_H_

namespace Cloudbase {

/**
 * This defines a file attachment for an insert/update request in the helper class.
 */
class CBHelperAttachment {
public:
	String fileName; /// The name of the file attached "example.jpg"
	String filePath; /// The full path of the file attached (this is currently not used by the helper class so can be NULL)
	MAHandle fileData; /// A data handler for the file contents (not a file handler! the file needs to be read into a data handler)
	int fileSize; /// The size of the file attached
};

}


#endif /* CBHELPERATTACHMENT_H_ */
