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

using namespace MAUtil;

namespace Cloudbase {

typedef enum {
	CBDataAggregationGroupSum,
	CBDataAggregationGroupMax,
	CBDataAggregationGroupMin,
	CBDataAggregationGroupAvg
} CBDataAggregationGroupOperator;

/**
 * The group aggregation command. This works exaclty in the same way a GROUP BY
 * command would work in SQL.
 * The outputField array contains the number of fields for the output to be
 * "grouped by".
 * There's a number of operators to apply to the grouped field defined as
 * CBDataAggregationGroupOperator
 */
class CBDataAggregationCommandGroup : public CBDataAggregationCommand {
private:
	Vector<String> idFields;
	Vector<String> groupFields;
public:
	const static String CBDataAggregationCommandGroup::CBDataAggregationGroupOperator_ToString[];

	CBDataAggregationCommandGroup();

	/**
	 * Adds a field to the list of fields the output should be
	 * grouped by
	 * @param fieldName An NSString with the name of the field
	 */
	void addOutputField(String fieldName);

	/**
	 * Adds a calculated field to the output of this group clause using a static value
	 * @param outputField The name of the output field
	 * @param op The operator to apply to the selected variable field
	 * @param groupValue A value to be used with the operator
	 */
	void addGroupFormulaForValue(String outputField, CBDataAggregationGroupOperator op, String groupValue);
	/**
	 * Adds a calculated field to the output of this group clause using the value of another field
	 * @param outputField The name of the output field
	 * @param op The operator to apply to the selected variable field
	 * @param fieldName The name of the variable field to be used with the operator
	 */
	void addGroupFormulaForField(String outputField, CBDataAggregationGroupOperator op, String fieldName);

	String serializeAggregateConditions();
};
}
