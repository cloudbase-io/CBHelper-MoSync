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
#include "CBPayPal.h"
#include "snprintf.h"

namespace Cloudbase {

CBPayPalBill::CBPayPalBill() {
	this->currency = "USD";
	this->paymentCompletedFunction = "";
	this->paymentCancelledFunction = "";
	this->paymentCompletedUrl = "";
	this->paymentCancelledUrl = "";
}

void CBPayPalBill::addItem(CBPayPalBillItem newItem) {
	this->items.add(newItem);
}

String CBPayPalBill::serialize() {
	String output = "";
	String itemsOutput = "";
	double fullAmount = 0.0f;

	for ( int i = 0; i < this->items.size(); i++ ) {
		CBPayPalBillItem curItem = this->items[i];

		itemsOutput += curItem.serialize();

		if ( i < this->items.size() - 1 ) {
			itemsOutput += ", ";
		}

		fullAmount = fullAmount + curItem.amount + (curItem.tax < 0 ? 0 : curItem.tax );
	}

	output += " { ";
	output += " \"name\" : \"" + this->name + "\", ";
	output += " \"description\" : \"" + this->description + "\", ";
	output += " \"amount\" : ";
	output += doubleToString(fullAmount);
	output += ", ";
	output += " \"invoice_number\" : \"" + this->invoiceNumber + "\", ";
	output += " \"items\" : [ " + itemsOutput + " ] } ";

	return output;
}

CBPayPalBillItem::CBPayPalBillItem() {
	this->tax = -1;
}

String CBPayPalBillItem::serialize() {
	String output = "";

	output += " { \"item_name\" : \"" + this->name + "\", ";
	output += " \"item_description\" : \"" + this->description + "\", ";
	output += " \"item_amount\" : ";
	output += doubleToString(this->amount);
	output += ", ";
	if ( this->tax > 0 ) {
		output += " \"item_tax\" : ";
		output += doubleToString(this->tax);
		output += ", ";
	}
	output += " \"item_quantity\" : ";
	output += integerToString(this->quantity);
	output += " } ";

	return output;
}

}
