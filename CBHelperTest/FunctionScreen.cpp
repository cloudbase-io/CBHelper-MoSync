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

#include "MainScreen.h"
#include "FunctionScreen.h"

using namespace Cloudbase;

/**
 * Constructor.
 */
FunctionScreen::FunctionScreen(MainScreen* mainHelper) : Screen(), mScreen(mainHelper) {
	//this->helper = mainHelper;
	createUI();
}

/**
 * Destructor.
 */
FunctionScreen::~FunctionScreen()
{
	// TODO: Deallocate the images we create!
}

/**
 * Create the UI for the color list screen.
 */
void FunctionScreen::createUI()
{
	setTitle("CloudFunction");

	// Create the screen's main layout widget.
	VerticalLayout* mMainLayout = new VerticalLayout();
	// Make the layout fill the entire screen.
	mMainLayout->fillSpaceHorizontally();
	mMainLayout->fillSpaceVertically();
	// Add the layout as the root of the screen's widget tree.
	Screen::setMainWidget(mMainLayout);

	Label* mTitle = new Label();
	// Make the label fill the width of the parent layout and
	// adjust its height to "shrink wrap" the size of the text.
	mTitle->fillSpaceHorizontally();
	mTitle->wrapContentVertically();
	// Set the label text.
	mTitle->setText("CloudFunction APIs");
	mTitle->setTextHorizontalAlignment("center");
	// Add the label to the main layout.
	mMainLayout->addChild(mTitle);

	mFunctionCode = new EditBox();
	mFunctionCode->fillSpaceHorizontally();
	mFunctionCode->wrapContentVertically();
	mFunctionCode->setPlaceholder("CloudFunction code");
	mMainLayout->addChild(mFunctionCode);

	mExecuteFunction = new Button();
	mExecuteFunction->fillSpaceHorizontally();
	mExecuteFunction->wrapContentVertically();
	mExecuteFunction->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mExecuteFunction->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mExecuteFunction->setText("Execute CloudFunction");
	mMainLayout->addChild(mExecuteFunction);

	mExecuteFunction->addButtonListener(this);

	mExecuteApplet = new Button();
	mExecuteApplet->fillSpaceHorizontally();
	mExecuteApplet->wrapContentVertically();
	mExecuteApplet->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mExecuteApplet->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mExecuteApplet->setText("Execute Applet");
	mMainLayout->addChild(mExecuteApplet);

	mExecuteApplet->addButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
*/
void FunctionScreen::buttonClicked(Widget* button)
{
	mFunctionCode->hideKeyboard();

	if (button == mExecuteFunction) {
		CBSearchResponder* resp = new CBSearchResponder();
		this->mScreen->helper->executeCloudFunction(mFunctionCode->getText(), NULL, resp);
	}

	if (button == mExecuteApplet) {
		MAUtil::Map<String, String>* params;
		params->insert("search", "#mosync");
		CBSearchResponder* resp = new CBSearchResponder();
		this->mScreen->helper->executeApplet("cb_twitter_search", params, resp);
	}
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void FunctionScreen::editBoxReturn(EditBox* editBox) {
	mFunctionCode->hideKeyboard();
}

