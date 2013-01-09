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
#include "DataScreen.h"
#include <MAUtil/DataHandler.h>

class DownloadResponder : public CBHelperResponder {
public:
	DownloadResponder() { fileName = "/sdcard/temp.jpg"; }

	void fileDownloaded(CBHelperAttachment fileData) {
		printf("called file download");
		MAHandle file = maFileOpen(fileName.c_str(), MA_ACCESS_READ_WRITE);
		if(file < 0) {
			printf("Error opening file %i\n", file);
			return;
		}

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

		res = maFileWriteFromData(file, fileData.fileData, 0, maGetDataSize(fileData.fileData));
		MAASSERT(res == 0);

		// Close the file.
		printf("Closing...\n");
		res = maFileClose(file);
		MAASSERT(res == 0);
	};
private:
	String fileName;
};

class TestData : public CBSerializable {
public:
	TestData(String firstName, String lastName, String post) :
			first_name_(firstName), last_name_(lastName), post_(post) {}


	String serialize() {
		String out = "[ {";
		out += "\"first_name\" : \"" + first_name_ + "\", ";
		out += "\"last_name\" : \"" + last_name_ + "\", ";
		out += "\"post_\" : \"" + post_ + "\" } ]";

		return out;
	}
private:
	String first_name_;
	String last_name_;
	String post_;
};

/**
 * Constructor.
 */
DataScreen::DataScreen(MainScreen* mainHelper) : Screen(), mScreen(mainHelper)
{
	//helper = mainHelper;
	createUI();
	Environment::getEnvironment().addCustomEventListener(this);
}

/**
 * Destructor.
 */
DataScreen::~DataScreen()
{
	// TODO: Deallocate the images we create!
}

/**
 * Create the UI for the color list screen.
 */
void DataScreen::createUI()
{
	setTitle("Data");

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
	mTitle->setText("data");
	mTitle->setTextHorizontalAlignment("center");
	// Add the label to the main layout.
	mMainLayout->addChild(mTitle);

	mInsertButton = new Button();
	mInsertButton->fillSpaceHorizontally();
	mInsertButton->wrapContentVertically();
	mInsertButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mInsertButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mInsertButton->setText("Insert Object");
	mMainLayout->addChild(mInsertButton);

	mInsertButton->addButtonListener(this);

	mInsertFileButton = new Button();
	mInsertFileButton->fillSpaceHorizontally();
	mInsertFileButton->wrapContentVertically();
	mInsertFileButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mInsertFileButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mInsertFileButton->setText("Insert Object with File");
	mMainLayout->addChild(mInsertFileButton);

	mInsertFileButton->addButtonListener(this);

	mSearchButton = new Button();
	mSearchButton->fillSpaceHorizontally();
	mSearchButton->wrapContentVertically();
	mSearchButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mSearchButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mSearchButton->setText("Search Objects");
	mMainLayout->addChild(mSearchButton);

	mSearchButton->addButtonListener(this);

	mFileIdBox = new EditBox();
	mFileIdBox->fillSpaceHorizontally();
	mFileIdBox->wrapContentVertically();
	mFileIdBox->setPlaceholder("file id");
	mMainLayout->addChild(mFileIdBox);

	mDownloadButton = new Button();
	mDownloadButton->fillSpaceHorizontally();
	mDownloadButton->wrapContentVertically();
	mDownloadButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mDownloadButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mDownloadButton->setText("Download file");
	mMainLayout->addChild(mDownloadButton);

	mDownloadButton->addButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
*/
void DataScreen::buttonClicked(Widget* button)
{
	if (button == mInsertButton) {
		TestData doc("Stefano", "Buliani", "method");
		mScreen->helper->insertDocument("users_data_tab", dynamic_cast<CBSerializable*>(&doc), NULL, NULL);
	}

	if (button == mInsertFileButton) {
		maImagePickerOpen();
	}

	if (button == mSearchButton) {
		CBSearchResponder* resp = new CBSearchResponder();
		this->mScreen->helper->searchDocument("users", CBHelperSearchCondition("first_name", "Stefano", CBOperatorEqual), (CBHelperResponder*)resp);
	}

	if (button == mDownloadButton) {
		DownloadResponder* resp = new DownloadResponder();
		this->mScreen->helper->downloadFile(mFileIdBox->getText(), resp);
		//this->mScreen->helper->downloadFile("7aec7e7c6d0d418472b2f02467f95a46", resp);
	}
}

void DataScreen::customEvent(const MAEvent& event) {
	if (event.type == EVENT_TYPE_IMAGE_PICKER) {
		if (event.imagePickerState == 1) {
			String imagePath = "/sdcard/DCIM/Camera/IMG_20121026_114908.jpg";
			mImageFile = maCreatePlaceholder();
			mImageFile = maFileOpen(imagePath.c_str(), MA_ACCESS_READ);

			if(mImageFile < 0){
				printf("Error opening image file %i\n", mImageFile);
				return;
			}
			MAHandle mEncodedImage = maCreatePlaceholder();

			maCreateData(mEncodedImage,maFileSize(mImageFile));

			int success = maFileReadToData(mImageFile, mEncodedImage, 0, maFileSize(mImageFile));
			CBHelperAttachment att;
			att.fileName = "testimage.png";
			att.filePath = imagePath;
			att.fileData = mEncodedImage;
			att.fileSize = maFileSize(mImageFile);

			MAUtil::Vector<CBHelperAttachment> atts;
			atts.add(att);
			TestData doc("Stefano", "Buliani", "method");
			this->mScreen->helper->insertDocument("test_attachment", dynamic_cast<CBSerializable*>(&doc), atts, NULL);

		}
	}
}

int DataScreen::getSystemProperty(const char* key, MAUtil::String& dst) {
    int size = maGetSystemProperty(key, NULL, 0);
    printf("received size: %i", size);
    if (size < 0)
        return size;
    dst.resize(size-1);
    maGetSystemProperty(key, dst.pointer(), size);
    return size;
}


MAUtil::String DataScreen::getLocalPath() {
    // Do this here to work around a MoRE bug.
    FileLister fl;
    fl.start("/");

    MAUtil::String path;
    // Try getting the local path.
    int result = getSystemProperty("mosync.path.local", path);
    // If it works, fine.
    if(result > 0) {
        printf("Got local path: %i\n", result);
        return path;
    }
    printf("local path output %i", result);
    // Otherwise, get the first root directory.
    fl.start("");
    result = fl.next(path);
    MAASSERT(result > 0);
    return path;
}


