#ifndef LOADER_H
#define LOADER_H

//(*Headers(Loader)
#include <wx/dialog.h>
#include <wx/intl.h>
//*)

class Loader: public wxDialog
{
	public:

		Loader(wxWindow* parent,wxWindowID id = -1);
		virtual ~Loader();

		//(*Identifiers(Loader)
        enum Identifiers
        {
        };
        //*)

	protected:

		//(*Handlers(Loader)
		void OnInit(wxInitDialogEvent& event);
        //*)

		//(*Declarations(Loader)
        //*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
