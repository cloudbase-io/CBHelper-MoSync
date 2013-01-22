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
#include "CBDataAggregationCommandGroup.h"

namespace Cloudbase {

const String CBDataAggregationCommandGroup::CBDataAggregationGroupOperator_ToString[] = {
	"$sum",
	"$max",
	"$min",
	"$avg"
};

CBDataAggregationCommandGroup::CBDataAggregationCommandGroup() {
	this->commandType = CBDataAggregationGroup;
}

void CBDataAggregationCommandGroup::addOutputField(String fieldName) {
	//String finalName = "$" + fieldName;
	this->idFields.add(fieldName);
}

void CBDataAggregationCommandGroup::addGroupFormulaForField(String outputField, CBDataAggregationGroupOperator op, String fieldName) {
	String finalFieldName = "$" + fieldName;
	this->addGroupFormulaForValue(outputField, op, finalFieldName);
}

void CBDataAggregationCommandGroup::addGroupFormulaForValue(String outputField, CBDataAggregationGroupOperator op, String groupValue) {
	String output = "";

	output += "\"" + outputField + "\" : { ";
	output += "\"" + this->CBDataAggregationGroupOperator_ToString[op] + "\" : \"";
	output += groupValue + "\" } ";

	this->groupFields.add(output);
}

String CBDataAggregationCommandGroup::serializeAggregateConditions() {
	String output = " { ";

	output += " \"_id\" : ";
	if ( this->idFields.size() > 1 ) {
		output += " { ";
		for ( int i = 0; i < this->idFields.size(); i++ ) {
			output += " \"" + this->idFields[i] + "\" : \"$";
			output += this->idFields[i] + "\"";

			if ( i < this->idFields.size() - 1 )
				output += ",";
		}
		output += " } ";
	} else {
		if ( this->idFields.size() > 0 )
			output += "\"$" + this->idFields[0] + "\"";
	}

	output += ",";

	for ( int i = 0; i < this->groupFields.size(); i++ ) {
		output += this->groupFields[i];

		if ( i < this->groupFields.size() - 1 )
			output += ",";
	}

	output += " } ";

	return output;
}

}
