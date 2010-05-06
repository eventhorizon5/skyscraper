#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

//(*Headers(TextWindow)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class TextWindow: public wxDialog
{
	public:

		TextWindow(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~TextWindow();

		//(*Declarations(TextWindow)
		wxButton* bOK;
		wxTextCtrl* tMain;
		//*)

	protected:

		//(*Identifiers(TextWindow)
		static const long ID_tMain;
		static const long ID_bOK;
		//*)

	private:

		//(*Handlers(TextWindow)
		void On_bOK_Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
