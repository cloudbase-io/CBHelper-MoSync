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
#include "LogScreen.h"

/**
 * Constructor.
 */
LogScreen::LogScreen(MainScreen* mainHelper) : Screen(), mScreen(mainHelper) {
	//this->helper = mainHelper;
	createUI();
}

/**
 * Destructor.
 */
LogScreen::~LogScreen()
{
	// TODO: Deallocate the images we create!
}

/**
 * Create the UI for the color list screen.
 */
void LogScreen::createUI()
{
	setTitle("Log");

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
	mTitle->setText("log APIs");
	mTitle->setTextHorizontalAlignment("center");
	// Add the label to the main layout.
	mMainLayout->addChild(mTitle);

	mLogLineBox = new EditBox();
	mLogLineBox->fillSpaceHorizontally();
	mLogLineBox->wrapContentVertically();
	mLogLineBox->setPlaceholder("Log line");
	mMainLayout->addChild(mLogLineBox);

	mLogCategoryBox = new EditBox();
	mLogCategoryBox->fillSpaceHorizontally();
	mLogCategoryBox->wrapContentVertically();
	mLogCategoryBox->setPlaceholder("Category");
	mLogCategoryBox->setText("DEFAULT");
	mMainLayout->addChild(mLogCategoryBox);

	mSubmitButton = new Button();
	mSubmitButton->fillSpaceHorizontally();
	mSubmitButton->wrapContentVertically();
	mSubmitButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mSubmitButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mSubmitButton->setText("Log");
	mMainLayout->addChild(mSubmitButton);

	mSubmitButton->addButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
*/
void LogScreen::buttonClicked(Widget* button)
{
	printf("button clicked");
	// Hide the keyboard if it isn't already dismissed.
	mLogLineBox->hideKeyboard();
	mLogCategoryBox->hideKeyboard();

	this->mScreen->helper->logDebug(mLogLineBox->getText(), mLogCategoryBox->getText());
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void LogScreen::editBoxReturn(EditBox* editBox) {
	if (editBox == mLogLineBox)
		mLogLineBox->hideKeyboard();
	if (editBox == mLogCategoryBox)
		mLogCategoryBox->hideKeyboard();
}

