#ifndef CAMTEX_H
#define CAMTEX_H

//(*Headers(CameraTextureControl)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class CameraTextureControl: public wxDialog
{
	public:

		CameraTextureControl(DebugPanel* parent,wxWindowID id=wxID_ANY);
		virtual ~CameraTextureControl();

		//(*Declarations(CameraTextureControl)
		wxButton* bOK;
		wxButton* bSetFOV;
		wxButton* bSetLookAt;
		wxButton* bSetPosition;
		wxButton* bSetRotation;
		wxCheckBox* chkEnabled;
		wxListBox* CameraList;
		wxStaticText* lblFOV;
		wxStaticText* lblPositionX;
		wxStaticText* lblPositionY;
		wxStaticText* lblPositionZ;
		wxStaticText* lblRotationX;
		wxStaticText* lblRotationY;
		wxStaticText* lblRotationZ;
		wxTextCtrl* txtFOV;
		wxTextCtrl* txtPositionX;
		wxTextCtrl* txtPositionY;
		wxTextCtrl* txtPositionZ;
		wxTextCtrl* txtRotationX;
		wxTextCtrl* txtRotationY;
		wxTextCtrl* txtRotationZ;
		//*)
		void Loop();

	protected:

		//(*Identifiers(CameraTextureControl)
		static const long ID_CameraList;
		static const long ID_chkEnabled;
		static const long ID_lblPositionX;
		static const long ID_lblPositionY;
		static const long ID_lblPositionZ;
		static const long ID_txtPositionX;
		static const long ID_txtPositionY;
		static const long ID_txtPositionZ;
		static const long ID_bSetPosition;
		static const long ID_lblRotationX;
		static const long ID_lblRotationY;
		static const long ID_lblRotationZ;
		static const long ID_txtRotationX;
		static const long ID_txtRotationY;
		static const long ID_txtRotationZ;
		static const long ID_bSetRotation;
		static const long ID_bSetLookAt;
		static const long ID_lblFOV;
		static const long ID_txtFOV;
		static const long ID_bSetFOV;
		static const long ID_bOK;
		//*)

	private:

		//(*Handlers(CameraTextureControl)
		void On_bOK_Click(wxCommandEvent& event);
		void On_chkEnabled_Click(wxCommandEvent& event);
		void On_bSetPosition_Click(wxCommandEvent& event);
		void On_bSetRotation_Click(wxCommandEvent& event);
		void On_bSetFOV_Click(wxCommandEvent& event);
		void On_bSetLookAt_Click(wxCommandEvent& event);
		//*)
		void BuildList(bool restore_selection = false);
		SBS::SBS *Simcore;
		DebugPanel *panel;
		int lastcount;
		SBS::CameraTexture *camera;

		DECLARE_EVENT_TABLE()
};

}

#endif
