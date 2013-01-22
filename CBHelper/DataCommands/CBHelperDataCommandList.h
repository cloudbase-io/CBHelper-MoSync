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
#include <conprint.h>
#include "../CBSerializable.h"
#include "../Convert.h"
#include "CBDataAggregationCommand.h"

using namespace MAUtil;

namespace Cloudbase {

/**
 * This object represents an ordered list of CBDataAggregationCommand
 * objects. It is used for the commands to be serialized and handed
 * to a request to the cloudbase.io APIs
 */
class CBHelperDataCommandList : public CBSerializable {
private:
	Vector<CBDataAggregationCommand*> commands;
public:
	/**
	 * Creates a new instance of the list
	 */
	CBHelperDataCommandList();

	/**
	 * Adds a command to the list. This is an ordered list and commands
	 * should be added exactly in the order they are meant to be executed
	 * in.
	 * @param command The new command to be added to the list
	 */
	void addCommand(CBDataAggregationCommand* command);

	String serialize();
};

}
