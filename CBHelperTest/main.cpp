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

#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <NativeUI/Widgets.h>
#include <ma.h>

#include <Notification/NotificationManager.h>
#include <Notification/PushNotification.h>
#include <Notification/PushNotificationListener.h>

#include <conprint.h>
#include "MainScreen.h"

#define CD2M_CREDENTIALS "Your@C2DMCredentials.io"

using namespace MAUtil;
using namespace Notification;
using namespace NativeUI;

/**
 * A Moblet is a high-level class that defines the
 * behaviour of a MoSync program.
 */
class MyMoblet : public Moblet, public PushNotificationListener
{
public:
	CBHelper* helper; //the CBHelper class
	MainScreen* mMainScreen;

	/**
	 * Initialize the application in the constructor.
	 */
	MyMoblet()
	{
		mMainScreen = MainScreen::createTabbedUI();

		/* NotificationManager::getInstance()->registerPushNotification(
		            PUSH_NOTIFICATION_TYPE_BADGE |
		            PUSH_NOTIFICATION_TYPE_ALERT |
		            PUSH_NOTIFICATION_TYPE_SOUND,
		            CD2M_CREDENTIALS);
		    NotificationManager::getInstance()->addPushNotificationListener(this); */


		// Show the screen.
		mMainScreen->show();
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}

		// Print the key character.
		//printf("You typed: %c\n", keyCode);
	}

	/**
	 * Called when a key is released.
	 */
	void keyReleaseEvent(int keyCode, int nativeCode)
	{
	}

	/**
	 * Called when the screen is touched.
	 */
	void pointerPressEvent(MAPoint2d point)
	{
		// Print the x and y coordinate.
		//printf("You touched: %i %i\n", point.x, point.y);
		//printf("session id retrieved: %s", helper->sessionId.c_str());
	}

	virtual void didReceivePushNotification(PushNotification& pushNotification) {
		maMessageBox("New push notification", pushNotification.getMessage()->c_str());
	}

	virtual void didApplicationRegistered(String& token) {
		maMessageBox("Application has registered", token.c_str());
		this->mMainScreen->notificationToken = token;
	}

	virtual void didFaildToRegister(String& error) {
	maMessageBox("Application cannot register", error.c_str());
	}

	//For the Android platform only (on iOS you will never get the callback):
	virtual void didApplicationUnregister() {
		maMessageBox("Done", "Application has unregistered");
	}

};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
