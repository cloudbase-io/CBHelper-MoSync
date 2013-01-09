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

#include <MAUtil/Map.h>
#include <MAUtil/HashMap.h>
#include <MAP/LonLat.h>
#include <conprint.h>
#include "Convert.h"
#include "CBSerializable.h"

//#include "CBHelper.h"

namespace Cloudbase {

/**
 * The possible operators for a search on the CloudBase database
 */
typedef enum CBConditionOperator {
	CBOperatorEqual,           //!< CBOperatorEqual
    CBOperatorLess,         //!< CBOperatorLess
    CBOperatorLessOrEqual,  //!< CBOperatorLessOrEqual
    CBOperatorBigger,       //!< CBOperatorBigger
    CBOperatorBiggerOrEqual,//!< CBOperatorBiggerOrEqual
    CBOperatorAll,          //!< CBOperatorAll
    CBOperatorExists,       //!< CBOperatorExists
    CBOperatorMod,          //!< CBOperatorMod
    CBOperatorNe,           //!< CBOperatorNe
    CBOperatorIn,           //!< CBOperatorIn
    CBOperatorNin,          //!< CBOperatorNin
    CBOperatorSize,         //!< CBOperatorSize
    CBOperatorType,         //!< CBOperatorType
    CBOperatorWithin,       //!< CBOperatorWithin
    CBOperatorNear          //!< CBOperatorNear
} CBConditionOperator;

/**
 * Conditions to concatenate multiple conditions
 */
typedef enum CBConditionLink {
	CBConditionLinkAnd,//!< CBConditionLinkAnd
	CBConditionLinkOr, //!< CBConditionLinkOr
	CBConditionLinkNor //!< CBConditionLinkNor
} CBConditionLink;

/**
 * This class represents a set of conditions to run a search over your CloudBase - this is the equivalent
 * representation of a WHERE clause in SQL.
 * Each condition can have a number of sub-conditions and each one is linked to the previous using a CBConditionLink
 */
class CBHelperSearchCondition : public CBSerializable {
public:
	/// String arrays to convert the condition operator and link from the struct to a String
	const static String CBConditionOperator_ToString[];
	const static String CBConditionLink_ToString[];

	/**
	 * Creates a new empty search conditions. Before being used this condition will need to set at
	 * least a field, value and operator
	 */
	CBHelperSearchCondition();
	/**
	 * Creates a new condition to run a basic search over a field.
	 * @param field The name of the field on the CloudBase collection
	 * @param value The value to look for. It is possible to use regular expressions in
	 * this field. For more information see the CloudBase documentation on the REST APIs
	 * page: http://cloudbase.io/documentation/rest-apis#CloudBase
	 * @param op The operator for the condition
	 */
	CBHelperSearchCondition(String field, String value, CBConditionOperator op);
	/**
	 * Creates a new location-based condition. This will run over the default cb_location field
	 * set on the documents when location information are sent.
	 * This performs a search within a boundary box specified with the 2 corners
	 * @param SWLat The latitude for the south-western corner of the boundary box
	 * @param SWLng The longitude for the south-western corner of the boundary box
	 * @param NELat The latitude for the north-eastern corner of the boundary box
	 * @param NELng The longitude for the north-eastern corner of the boundary box
	 */
	CBHelperSearchCondition(double SWLat, double SWLng, double NELat, double NELng);
	/**
	 * Creates a new location-based condition. This will run over the default cb_location field
	 * set on the documents when location information are sent.
	 * This performs a search around the given location.
	 * @param lat The latitude of the point we are looking for
	 * @param lng The longitude of the point to search around
	 * @param maxDistance The size in meters of the area to consider around the given point
	 */
	CBHelperSearchCondition(double lat, double lng, double maxDistance);

	/**
	 * Sets the link between the current condition and the previous one
	 * @param link The CBConditionLink element
	 */
	void setConditionLink(CBConditionLink link);
	/**
	 * Adds a condition to the list of subconditions for the current one.
	 * @param newCond The new condition to be added
	 */
	void addCondition(CBHelperSearchCondition* newCond);

	// internal methods used to serialize a CBCondition object and its sub-conditions.
	String serialize();
	String serialize(CBHelperSearchCondition* cond, bool isTop = true);

	String field_;
	String value_;
	int conditionOperator_;
	int contidionLink_;
	Vector<CBHelperSearchCondition*> conditions_;
protected:
	void baseInit();

};

}
