#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

//(*Headers(CameraControl)
#include <wx/frame.h>
#include <wx/intl.h>
//*)

class CameraControl: public wxFrame
{
	public:

		CameraControl(wxWindow* parent,wxWindowID id = -1);
		virtual ~CameraControl();

		//(*Identifiers(CameraControl)
        enum Identifiers
        {
        };
        //*)

	protected:

		//(*Handlers(CameraControl)
		void OnInit(wxInitDialogEvent& event);
        //*)

		//(*Declarations(CameraControl)
        //*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
