/*
 Copyright (c) 2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Main_Forms_AboutDialog
#define TC_HEADER_Main_Forms_AboutDialog

#include "Forms.h"

namespace YourProduct
{
	class AboutDialog : public AboutDialogBase
	{
	public:
		AboutDialog (wxWindow* parent);
		
		void OnWebsiteHyperlinkClick (wxHyperlinkEvent& event) { Gui->OpenHomepageLink (this, L"main"); }
	};
}

#endif // TC_HEADER_Main_Forms_AboutDialog
