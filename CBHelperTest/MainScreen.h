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
#include <NativeUI/Widgets.h>
#include "HelperInclude.h"

using namespace NativeUI;
using namespace Cloudbase;

class CBSearchResponder : public CBHelperResponder {
public:
	CBSearchResponder() {};
	void parseResponse(CBHelperResponseInfo* resp, YAJLDom::Value* responseMessage) {
		printf("total child %i", responseMessage->getNumChildValues());
	}
};

class MainScreen : public TabScreen
{
public:
	String notificationToken;

	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	virtual ~MainScreen();

	/**
	 * Create UI with three tabs
	 */
	static MainScreen* createTabbedUI();

	CBHelper* helper;

	void initalizeHelper(MAUtil::String appCode, MAUtil::String appUniq, MAUtil::String pwd);
private:
};


//#endif /* MAINSCREEN_H_ */
