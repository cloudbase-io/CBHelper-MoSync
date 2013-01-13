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
#include "SettingsScreen.h"
#include "DataScreen.h"
#include "LogScreen.h"
#include "NotificationScreen.h"
#include "FunctionScreen.h"

#define SETTINGS_TAB		0
#define DATA_TAB			1
#define LOG_TAB				2
#define NOTIF_TAB			3
#define FUNCTION_TAB		4

class TabbedMainScreen :
	public MainScreen,
	public TabScreenListener
{
public:
	TabbedMainScreen() : MainScreen()
	{
		mSettings = new SettingsScreen(dynamic_cast<MainScreen*>(this));
		mData = new DataScreen(dynamic_cast<MainScreen*>(this));
		mLog = new LogScreen(dynamic_cast<MainScreen*>(this));
		mNotif = new NotificationScreen(dynamic_cast<MainScreen*>(this));
		mFunction = new FunctionScreen(dynamic_cast<MainScreen*>(this));

			// Add them as tabs.
		this->addTab(mSettings);
		this->addTab(mData);
		this->addTab(mLog);
		this->addTab(mNotif);
		this->addTab(mFunction);
	}

	/**
	 * This method is called when there is an event for this widget.
	 * @param widgetEventData The data for the widget event.
	 */
	void handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		if (MAW_EVENT_TAB_CHANGED == widgetEventData->eventType)
		{
			// Here we save the current tab index.
			mCurrentTabIndex = widgetEventData->tabIndex;
		}
	}

	/**
	 * This method is called when a key is pressed.
	 */
	void handleKeyPress(int keyCode)
	{
		// The MAK_BACK key code is sent on Android when
		//  the back key is pressed.
		if (MAK_BACK == keyCode)
		{
			maExit(0);
		}
	}

	/**
	 * Handle pointer presses.
	 */
	void handlePointerPressed(MAPoint2d point)
	{

	}

	/**
	 * Handle pointer moves.
	 */
	void handlePointerMoved(MAPoint2d point)
	{

	}

	/**
	 * Handle pointer releases.
	 */
	void handlePointerReleased(MAPoint2d point)
	{

	}

    /**
     * This method is called when a tab screen has changed to a new tab.
     * @param tabScreen The tab screen object that generated the event.
     * @param tabScreenIndex The index of the new tab.
     */
    void tabScreenTabChanged(
        TabScreen* tabScreen,
        const int tabScreenIndex)
    {
		// Here we save the current tab index.
		mCurrentTabIndex = tabScreenIndex;
    }

private:
	/**
	 * Index of the currently visible tab.
	 */
	int mCurrentTabIndex;

	/**
	 * Various screens...
	 */
	SettingsScreen* mSettings;
	DataScreen* mData;
	LogScreen* mLog;
	NotificationScreen* mNotif;
	FunctionScreen* mFunction;
};

MainScreen* MainScreen::createTabbedUI()
{
	return new TabbedMainScreen();
}

/**
* Constructor.
*/
MainScreen::MainScreen(): TabScreen() {
	//this->helper = NULL;
}

/**
* Destructor.
*/
MainScreen::~MainScreen()
{
}

void MainScreen::initalizeHelper(MAUtil::String appCode, MAUtil::String appUniq, MAUtil::String pwd) {
	this->helper = new CBHelper(appCode, appUniq);
	this->helper->setPassword(pwd);
	printf("init");
}

