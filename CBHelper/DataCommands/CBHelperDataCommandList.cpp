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
#include "CBHelperDataCommandList.h"

namespace Cloudbase {

CBHelperDataCommandList::CBHelperDataCommandList() {

}

void CBHelperDataCommandList::addCommand(CBDataAggregationCommand* command) {
	this->commands.add(command);
}

String CBHelperDataCommandList::serialize() {
	String output = " { ";

	output += "\"cb_aggregate_key\" : [ ";

	for ( int i = 0; i < this->commands.size(); i++ ) {
		CBDataAggregationCommand *curCommand = this->commands[i];
		output += " { \"" + curCommand->getCommandTypeString() + "\" : ";
		output += curCommand->serializeAggregateConditions();
		output += " } ";

		if ( i < this->commands.size() - 1 )
			output += ", ";
	}

	output += " ] } ";

	//printf("Prepared aggregate request: %s", output.c_str());

	return output;
}

}
