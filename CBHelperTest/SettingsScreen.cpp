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

/**
 * Constructor.
 */
SettingsScreen::SettingsScreen(MainScreen* mainHelper) : Screen(), mScreen(mainHelper) {
	//this->helper = mainHelper;
	createUI();
	this->loadSettings();
}

/**
 * Destructor.
 */
SettingsScreen::~SettingsScreen()
{
	// TODO: Deallocate the images we create!
}

/**
 * Create the UI for the color list screen.
 */
void SettingsScreen::createUI()
{
	setTitle("Settings");

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
	mTitle->setText("application settings");
	mTitle->setTextHorizontalAlignment("center");
	// Add the label to the main layout.
	mMainLayout->addChild(mTitle);

	mAppCodeBox = new EditBox();
	mAppCodeBox->fillSpaceHorizontally();
	mAppCodeBox->wrapContentVertically();
	mAppCodeBox->setPlaceholder("application code");
	//mAppCodeBox->setInputFlag(EDIT_BOX_INPUT_FLAG_PASSWORD);
	mMainLayout->addChild(mAppCodeBox);

	mAppUniqBox = new EditBox();
	mAppUniqBox->fillSpaceHorizontally();
	mAppUniqBox->wrapContentVertically();
	mAppUniqBox->setPlaceholder("application unique");
	mMainLayout->addChild(mAppUniqBox);

	mAppPwdBox = new EditBox();
	mAppPwdBox->fillSpaceHorizontally();
	mAppPwdBox->wrapContentVertically();
	mAppPwdBox->setInputFlag(EDIT_BOX_INPUT_FLAG_PASSWORD);
	mAppPwdBox->setPlaceholder("application password");
	mMainLayout->addChild(mAppPwdBox);

	mSubmitButton = new Button();
	mSubmitButton->fillSpaceHorizontally();
	mSubmitButton->wrapContentVertically();
	mSubmitButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mSubmitButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mSubmitButton->setText("Save");
	mMainLayout->addChild(mSubmitButton);

	mSubmitButton->addButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
*/
void SettingsScreen::buttonClicked(Widget* button)
{
	printf("button clicked");
	// Hide the keyboard if it isn't already dismissed.
	mAppCodeBox->hideKeyboard();
	mAppUniqBox->hideKeyboard();
	mAppPwdBox->hideKeyboard();

	this->saveSettings();
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void SettingsScreen::editBoxReturn(EditBox* editBox) {
	if (editBox == mAppCodeBox)
		mAppCodeBox->hideKeyboard();
	if (editBox == mAppUniqBox)
		mAppUniqBox->hideKeyboard();
	if (editBox == mAppPwdBox)
		mAppPwdBox->hideKeyboard();
}

void SettingsScreen::loadSettings() {
	MAUtil::String filename = getLocalPath() + SETTINGS_FILE_NAME;

	MAHandle file = maFileOpen(filename.c_str(), MA_ACCESS_READ);
	if(file < 0) {
		printf("Error opening file %i\n", file);
		return;
	}

	// Check if the file exists.
	int res = maFileExists(file);
	MAASSERT(res >= 0);
	if(!res) {
		printf("File does not exist.\n");
		maFileClose(file);
		return;
	}

	// Get the file size.
	int size = maFileSize(file);
	printf("Size: %i\n", size);
	MAASSERT(size >= 0);

	// Read the file data.
	static char data[200];
	MAASSERT(size < (int)sizeof(data));
	res = maFileRead(file, data, size);
	MAASSERT(res == 0);

	// Close the file.
	printf("Closing...\n");
	res = maFileClose(file);
	MAASSERT(res == 0);

	printf("Done.\n");

	MAUtil::String contents = data;

	printf("Loaded settings string %s", contents.c_str());

	if (contents.findFirstOf(',', 0) <= 0)
		return;

	int commaPosition = contents.findFirstOf(',', 0);
	MAUtil::String appCode = contents.substr(0, commaPosition);
	mAppCodeBox->setText(appCode);
	printf("app code: %s", appCode.c_str());

	int prevCommaPosition = commaPosition + 1;
	commaPosition = contents.findFirstOf(',', prevCommaPosition);
	MAUtil::String appUniq = contents.substr(prevCommaPosition, commaPosition-prevCommaPosition);
	mAppUniqBox->setText(appUniq);
	printf("app uniq: %s", appUniq.c_str());

	prevCommaPosition = commaPosition + 1;
	commaPosition = contents.findFirstOf(',', prevCommaPosition);
	MAUtil::String appPwd = contents.substr(prevCommaPosition, contents.length() - prevCommaPosition);
	//mAppPwdBox->setText(appPwd);
	printf("app pwd: %s", appPwd.c_str());

	//helper = CBHelper(appCode, appUniq);
	//helper.setPassword(appPwd);
	this->mScreen->initalizeHelper(appCode, appUniq, appPwd);
}

void SettingsScreen::saveSettings() {
	//return;
	// Construct the filename.
	this->mScreen->initalizeHelper(mAppCodeBox->getText(), mAppUniqBox->getText(), Cloudbase::MD5(mAppPwdBox->getText()).hexdigest());
	return;

	MAUtil::String filename = getLocalPath() + SETTINGS_FILE_NAME;

	// Open the file handle.
	printf("Open '%s'\n", filename.c_str());
	MAHandle file = maFileOpen(filename.c_str(), MA_ACCESS_READ_WRITE);
	if(file < 0) {
		printf("Error opening file %i\n", file);
		return;
	}

	// If the file exists...
	int res = maFileExists(file);
	MAASSERT(res >= 0);
	if(res) {
		// Truncate it, deleting any old data.
		printf("Truncating file...\n");
		res = maFileTruncate(file, 0);
		MAASSERT(res == 0);
	} else {
		// Otherwise, create it.
		printf("Creating file...\n");
		res = maFileCreate(file);
		MAASSERT(res >= 0);
	}
	// In either case, we now have an empty file at our disposal.

	// Write some data.
	MAUtil::String settingsData = "";
	settingsData += mAppCodeBox->getText();
	settingsData += ",";
	settingsData += mAppUniqBox->getText();
	settingsData += ",";
	settingsData += Cloudbase::MD5(mAppPwdBox->getText()).hexdigest();

	//static const char data[] = strdup(settingsData.c_str());

	res = maFileWrite(file, settingsData.c_str(), settingsData.length());
	MAASSERT(res == 0);

	// Close the file.
	printf("Closing...\n");
	res = maFileClose(file);
	MAASSERT(res == 0);

	printf("Done.\n");
	this->loadSettings();
	//return true;
}

int SettingsScreen::getSystemProperty(const char* key, MAUtil::String& dst) {
    int size = maGetSystemProperty(key, NULL, 0);
    printf("received size: %i", size);
    if (size < 0)
        return size;
    dst.resize(size-1);
    maGetSystemProperty(key, dst.pointer(), size);
    return size;
}


MAUtil::String SettingsScreen::getLocalPath() {
    // Do this here to work around a MoRE bug.
    FileLister fl;
    fl.start("/");

    MAUtil::String path;
    // Try getting the local path.
    int result = getSystemProperty("mosync.path.local", path);
    // If it works, fine.
    if(result > 0) {
        printf("Got local path: %i\n", result);
        return path + "/";
    }
    printf("local path output %i", result);
    // Otherwise, get the first root directory.
    fl.start("");
    result = fl.next(path);
    MAASSERT(result > 0);
    return path;
}


