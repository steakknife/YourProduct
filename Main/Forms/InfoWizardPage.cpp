/*
 Copyright (c) 2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#include "System.h"
#include "InfoWizardPage.h"

namespace YourProduct
{
	InfoWizardPage::InfoWizardPage (wxPanel *parent, const wxString &actionButtonText, shared_ptr <Functor> actionFunctor)
		: InfoWizardPageBase (parent)
	{
		if (!actionButtonText.empty())
		{
			wxButton *actionButton = new wxButton (this, wxID_ANY, actionButtonText);
			ActionFunctor = actionFunctor;
			actionButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (InfoWizardPage::OnActionButtonClick), nullptr, this);

			InfoPageSizer->Add (actionButton, 0, wxALL, 5);
		}

		InfoStaticText->SetFocus();
	}

	void InfoWizardPage::SetMaxStaticTextWidth (int width)
	{
		InfoStaticText->Wrap (width);
	}
}
