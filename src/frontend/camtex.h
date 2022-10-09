#ifndef CAMTEX_H
#define CAMTEX_H

//(*Headers(CameraTextureControl)
#include <wx/dialog.h>
//*)

namespace Skyscraper {

class CameraTextureControl: public wxDialog
{
	public:

		CameraTextureControl(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~CameraTextureControl();

		//(*Declarations(CameraTextureControl)
		//*)
		void Loop();

	protected:

		//(*Identifiers(CameraTextureControl)
		//*)

	private:

		//(*Handlers(CameraTextureControl)
		//*)

		DECLARE_EVENT_TABLE()
};

}

#endif
