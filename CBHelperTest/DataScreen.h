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

#include <MAUtil/FileLister.h>
#include <NativeUI/Widgets.h>
#include "DataCommands/CBDataAggregationCommandGroup.h"
#include "DataCommands/CBDataAggregationCommandProject.h"
#include "DataCommands/CBDataAggregationCommandUnwind.h"
//#include "MainScreen.h"

//#include "CBHelper/CBHelper.h"
//#include "CBHelper/MD5.h"

using namespace NativeUI;
using namespace Cloudbase;

class DataScreen : public Screen,
	public ButtonListener, public CustomEventListener
{
public:
		/**
		 * Constructor.
		 */
		DataScreen(MainScreen* mainHelper);

		/**
		 * Destructor.
		 */
		virtual ~DataScreen();

		/**
		 * Create the UI for the color list screen.
		 */
		void createUI();

		virtual void customEvent(const MAEvent& event);
private:
		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(Widget* button);
		int getSystemProperty(const char* key, MAUtil::String& dst);
		MAUtil::String getLocalPath();
private:
		Button* mInsertButton;
		Button* mInsertFileButton;
		Button* mSearchButton;
		Button* mSearchAggregateButton;
		EditBox* mFileIdBox;
		Button* mDownloadButton;
		MainScreen* mScreen;

		CBHelperResponder* resp;

		Image* mImage;
		MAUtil::String mImagePath;
		MAHandle mImageFile;
		MAHandle mEncodedImage;
};

