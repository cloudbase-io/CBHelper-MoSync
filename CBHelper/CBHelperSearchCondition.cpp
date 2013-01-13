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

#include "CBHelperSearchCondition.h"

namespace Cloudbase {

const String CBHelperSearchCondition::CBConditionOperator_ToString[] = {
		"",
		"$lt",
		"$lte",
		"$gt",
		"$gte",
		"$all",
		"$exists",
		"$mod",
		"$ne",
		"$in",
		"$nin",
		"$size",
		"$type",
		"$within",
		"$near"
	};

const String CBHelperSearchCondition::CBConditionLink_ToString[] = {
		"$and",
		"$or",
		"$nor"
	};


CBHelperSearchCondition::CBHelperSearchCondition() {
	this->baseInit();
}
CBHelperSearchCondition::CBHelperSearchCondition(String field, String value, CBConditionOperator op) {
	this->baseInit();

	this->field_ = field;
	this->value_ = value;
	this->conditionOperator_ = op;
}

CBHelperSearchCondition::CBHelperSearchCondition(double SWLat, double SWLng, double NELat, double NELng) {
	this->baseInit();

	this->field_ = "cb_location";
	this->value_ = "[ [ " + Convert::toString(SWLat) + ", " + Convert::toString(SWLng) + "], [ " + Convert::toString(NELat) + ", " + Convert::toString(NELng) + "] ] ";
	this->conditionOperator_ = CBOperatorWithin;
}
CBHelperSearchCondition::CBHelperSearchCondition(double lat, double lng, double maxDistance) {
	this->baseInit();

	this->field_ = "cb_location";
	this->value_ = "{ \"$near\" : [ " + Convert::toString(lat) + ", " + Convert::toString(lng) + "], \"$maxDistance\" : " + Convert::toString(maxDistance) + " }";
	this->conditionOperator_ = CBOperatorNear;

}

void CBHelperSearchCondition::setConditionLink(CBConditionLink link) {
	this->contidionLink_ = link;
}

void CBHelperSearchCondition::addCondition(CBHelperSearchCondition* newCond) {
	this->conditions_.add(newCond);
}

void CBHelperSearchCondition::baseInit() {
	//this->conditions_;
	this->field_ = "";
	this->value_ = "";
	this->limit = -1;
}

void CBHelperSearchCondition::addSortField(String fieldName, CBSortDirection dir) {
	String sortField = "{ \"";
	sortField += fieldName + "\" : ";
	sortField += dir + " }";
	this->sortFields_.add(sortField);
}

String CBHelperSearchCondition::serialize() {
	return this->serialize(this, true);
}

//String CBHelperSearchCondition::serialize(bool isTop) {
//	return this->serialize(this, isTop);
//}

String CBHelperSearchCondition::serialize(CBHelperSearchCondition* cond, bool isTop) {
	String output = "{ ";
	if (isTop) {
		if (!cond->sortFields_.empty()) {
			output += "\"cb_sort_key\" : [";
			for (int i = 0; i < cond->sortFields_.size(); i++) {
				output += cond->sortFields_[i];
				if (i < cond->sortFields_.size() - 1)
					output += ", ";
			}
			output += " ], ";
		}

		if (cond->limit > 0) {
			output += "\"cb_limit\" : " + cond->limit;
			output += ", ";
		}
		output += "\"cb_search_key\" : { ";
	}

	if (cond->field_ == "") {
		if (!cond->conditions_.empty()) {
			int prevLink = -1;
			String curCond = "";

			for (int i = 0; i < cond->conditions_.size(); i++) {
				CBHelperSearchCondition* curGroup = cond->conditions_[i];
				if (prevLink != -1 && prevLink != curGroup->contidionLink_) {
					String linkStr = cond->CBConditionLink_ToString[prevLink];
					output += "\"" + linkStr + "\" : " + curCond + ", ";
					curCond = "";
				}
				curCond = curGroup->serialize(curGroup, false);
				prevLink = curGroup->contidionLink_;
				if (i == cond->conditions_.size() - 1) {
				    //[output setValue:curObject forKey:CBConditionLink_ToString[prevLink]];
					String linkStr = cond->CBConditionLink_ToString[prevLink];
					output += "\"" + linkStr + "\" : " + curCond;
				}
			}
		}
	} else {
		String op = cond->CBConditionOperator_ToString[cond->conditionOperator_];
		switch (cond->conditionOperator_) {
		case CBOperatorEqual:
			output += "\"" + cond->field_ + "\"";
			output += " : \"" + cond->value_ + "\"";
			break;
		case CBOperatorAll:
		case CBOperatorExists:
		case CBOperatorNe:
		case CBOperatorIn:
		case CBOperatorNin:
		case CBOperatorSize:
		case CBOperatorType:
			output += "\"" + cond->field_ + "\"";
			output += " : { \"" + op + "\" : \"" + cond->value_ + "\" } ";
		                //[cond setValue:conditionsGroup.value forKey:CBConditionOperator_ToString[conditionsGroup.CBOperator]];
		                //[output setValue:cond forKey:conditionsGroup.field];
			break;
		case CBOperatorMod:
			output += "\"" + cond->field_ + "\"";
			output += " : { \"" + op + "\" : [ " + cond->value_ + ", 1 ] } ";
			break;
		case CBOperatorWithin:
			output += "\"" + cond->field_ + "\"";
			output += " : { \"" + op + "\" :  { \"$box\" : " + cond->value_ + " } } ";
			break;
		case CBOperatorNear:
			output += "\"" + cond->field_ + "\"";
			output += " : " + cond->value_;
			break;
		default:
			break;
		}
	}

	if (isTop)
		output += " } ";

	output += " }";

	return output;
}

}
