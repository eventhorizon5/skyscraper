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
		wxButton* bAddShaftWall;
		wxButton* bAddWall;
		wxButton* bColumnWallBox;
		wxButton* bAddDoor;
		wxButton* bAddFloorIndicator;
		wxButton* bShaft;
		wxButton* bAddStairsDoor;
		wxButton* bAddShaftDoor;
		wxButton* bCreateCallButtons;
		wxButton* bLoadTexture;
		wxButton* bAddDirectionalIndicator;
		wxButton* bAddStairs;
		wxButton* bColumnWallBox2;
		wxButton* bAddFloor;
		wxButton* bAddStairsFloor;
		wxButton* bAddInterfloorFloor;
		wxButton* bFloor;
		wxButton* bElevator;
		wxButton* bAddShaftFloor;
		wxButton* bAddInterfloorWall;
		wxButton* bAddStairsWall;
		//*)

	protected:

		//(*Identifiers(CreateObject)
		static const long ID_bLoadTexture;
		static const long ID_bFloor;
		static const long ID_bAddFloor;
		static const long ID_bAddWall;
		static const long ID_bAddInterfloorFloor;
		static const long ID_bAddInterfloorWall;
		static const long ID_bAddShaftFloor;
		static const long ID_bAddShaftWall;
		static const long ID_bAddStairsFloor;
		static const long ID_bAddStairsWall;
		static const long ID_bColumnWallBox;
		static const long ID_bColumnWallBox2;
		static const long ID_bCreateCallButtons;
		static const long ID_bAddStairs;
		static const long ID_bAddDoor;
		static const long ID_bAddStairsDoor;
		static const long ID_bAddDirectionalIndicator;
		static const long ID_bAddShaftDoor;
		static const long ID_bAddFloorIndicator;
		static const long ID_bElevator;
		static const long ID_bShaft;
		//*)

	private:

		//(*Handlers(CreateObject)
		void On_bLoadTexture_Click(wxCommandEvent& event);
		void On_bFloor_Click(wxCommandEvent& event);
		void On_bElevator_Click(wxCommandEvent& event);
		void On_bShaft_Click(wxCommandEvent& event);
		void On_bAddFloor_Click(wxCommandEvent& event);
		void On_bAddWall_Click(wxCommandEvent& event);
		void On_bAddInterfloorFloorC_lick(wxCommandEvent& event);
		void On_bAddInterfloorFloor_Click(wxCommandEvent& event);
		void On_bAddInterfloorWall_Click(wxCommandEvent& event);
		void On_bAddShaftFloor_Click(wxCommandEvent& event);
		void On_bAddShaftWall_Click(wxCommandEvent& event);
		void On_bAddFloorIndicator_Click(wxCommandEvent& event);
		void On_bAddShaftDoor_Click(wxCommandEvent& event);
		void On_bAddDirectionalIndicator_Click(wxCommandEvent& event);
		void On_bAddStairsDoor_Click(wxCommandEvent& event);
		void On_bAddDoor_Click(wxCommandEvent& event);
		void On_bAddStairs_Click(wxCommandEvent& event);
		void On_bCreateCallButtons_Click(wxCommandEvent& event);
		void On_bColumnWallBox2_Click(wxCommandEvent& event);
		void On_bColumnWallBox_Click(wxCommandEvent& event);
		void On_bAddStairsWall_Click(wxCommandEvent& event);
		void On_bAddStairsFloor_Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
