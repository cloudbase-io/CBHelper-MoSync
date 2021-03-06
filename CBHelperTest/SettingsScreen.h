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
#define SETTINGS_FILE_NAME "cb_settings.txt"

#include <MAUtil/FileLister.h>
#include <NativeUI/Widgets.h>

using namespace NativeUI;
using namespace Cloudbase;

class SettingsScreen : public Screen,
	public ButtonListener,
	public EditBoxListener
{
public:
		/**
		 * Constructor.
		 */
		SettingsScreen(MainScreen* mainHelper);

		/**
		 * Destructor.
		 */
		virtual ~SettingsScreen();

		/**
		 * Create the UI for the color list screen.
		 */
		void createUI();
private:
		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(Widget* button);

		/**
		 * This method is called when the return button was pressed.
		 * On iphone platform the virtual keyboard is not hidden after
		 * receiving this event.
		 * @param editBox The edit box object that generated the event.
		 */
		virtual void editBoxReturn(EditBox* editBox);

		void saveSettings();
		void loadSettings();
		int getSystemProperty(const char* key, MAUtil::String& dst);
		MAUtil::String getLocalPath();

private:
		EditBox* mAppCodeBox;
		EditBox* mAppUniqBox;
		EditBox* mAppPwdBox;
		Button* mSubmitButton;
		MainScreen* mScreen;
};
