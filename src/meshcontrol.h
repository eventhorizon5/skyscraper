#ifndef MESHCONTROL_H
#define MESHCONTROL_H

//(*Headers(MeshControl)
#include <wx/frame.h>
#include <wx/intl.h>
//*)

class MeshControl: public wxFrame
{
	public:

		MeshControl(wxWindow* parent,wxWindowID id = -1);
		virtual ~MeshControl();

		//(*Identifiers(MeshControl)
        enum Identifiers
        {
        };
        //*)

	protected:

		//(*Handlers(MeshControl)
		void OnInit(wxInitDialogEvent& event);
        //*)

		//(*Declarations(MeshControl)
        //*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
