#ifndef CREATEOBJECT_H
#define CREATEOBJECT_H

//(*Headers(CreateObject)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
//*)

class CreateObject: public wxDialog
{
	public:

		CreateObject(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CreateObject();

		//(*Declarations(CreateObject)
		wxButton* bShaft;
		wxButton* bLoadTexture;
		wxButton* bFloor;
		wxButton* bElevator;
		//*)

	protected:

		//(*Identifiers(CreateObject)
		static const long ID_bLoadTexture;
		static const long ID_bFloor;
		static const long ID_bElevator;
		static const long ID_bShaft;
		//*)

	private:

		//(*Handlers(CreateObject)
		void On_bLoadTexture_Click(wxCommandEvent& event);
		void On_bFloor_Click(wxCommandEvent& event);
		void On_bElevator_Click(wxCommandEvent& event);
		void On_bShaft_Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
