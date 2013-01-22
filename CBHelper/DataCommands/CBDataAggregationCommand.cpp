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
#include "CBDataAggregationCommand.h"

namespace Cloudbase {

const String CBDataAggregationCommand::CBDataAggregationCommandType_ToString[] = {
	"$project",
	"$unwind",
	"$group",
	"$match"
};

String CBDataAggregationCommand::getCommandTypeString() {
	return this->CBDataAggregationCommandType_ToString[this->commandType];
}

}



