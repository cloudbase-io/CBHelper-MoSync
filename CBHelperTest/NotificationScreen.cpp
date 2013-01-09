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
#include "NotificationScreen.h"

/**
 * Constructor.
 */
NotificationScreen::NotificationScreen(MainScreen* mainHelper) : Screen(), mScreen(mainHelper) {
	//this->helper = mainHelper;
	createUI();
}

/**
 * Destructor.
 */
NotificationScreen::~NotificationScreen()
{
	// TODO: Deallocate the images we create!
}

/**
 * Create the UI for the color list screen.
 */
void NotificationScreen::createUI()
{
	setTitle("Notification");

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
	mTitle->setText("Notification APIs");
	mTitle->setTextHorizontalAlignment("center");
	// Add the label to the main layout.
	mMainLayout->addChild(mTitle);


	mSubscribeButton = new Button();
	mSubscribeButton->fillSpaceHorizontally();
	mSubscribeButton->wrapContentVertically();
	mSubscribeButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mSubscribeButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mSubscribeButton->setText("Subscribe");
	mMainLayout->addChild(mSubscribeButton);

	mSubscribeButton->addButtonListener(this);

	mNotifBox = new EditBox();
	mNotifBox->fillSpaceHorizontally();
	mNotifBox->wrapContentVertically();
	mNotifBox->setPlaceholder("Notification text");
	mMainLayout->addChild(mNotifBox);

	mPushButton = new Button();
	mPushButton->fillSpaceHorizontally();
	mPushButton->wrapContentVertically();
	mPushButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mPushButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mPushButton->setText("Push Notification");
	mMainLayout->addChild(mPushButton);

	mPushButton->addButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
*/
void NotificationScreen::buttonClicked(Widget* button)
{
	mNotifBox->hideKeyboard();

	if (button == mSubscribeButton) {
		this->mScreen->helper->subscribeDeviceWithToken(this->mScreen->notificationToken, NOTIF_CHANNEL, NULL);
	}

	if (button == mPushButton) {
		this->mScreen->helper->sendNotification(NOTIF_CHANNEL, "development", mNotifBox->getText(), "-1", "-1", NULL);
	}

	//this->mScreen->helper->logDebug(mLogLineBox->getText(), mLogCategoryBox->getText());
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void NotificationScreen::editBoxReturn(EditBox* editBox) {
	mNotifBox->hideKeyboard();
}

