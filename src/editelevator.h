#ifndef EDITELEVATOR_H
#define EDITELEVATOR_H

//(*Headers(editelevator)
#include <wx/frame.h>
#include <wx/intl.h>
//*)

class editelevator: public wxFrame
{
	public:

		editelevator(wxWindow* parent,wxWindowID id = -1);
		virtual ~editelevator();

		//(*Identifiers(editelevator)
        enum Identifiers
        {
        };
        //*)

	protected:

		//(*Handlers(editelevator)
		void OnInit(wxInitDialogEvent& event);
        //*)

		//(*Declarations(editelevator)
        //*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
