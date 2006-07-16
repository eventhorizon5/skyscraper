#include "cameracontrol.h"

BEGIN_EVENT_TABLE(CameraControl,wxFrame)
//(*EventTable(CameraControl)
//*)
END_EVENT_TABLE()

CameraControl::CameraControl(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(CameraControl)
    Create(parent,id,_("Camera Control"),wxDefaultPosition,wxDefaultSize,0);
    //*)
}

CameraControl::~CameraControl()
{
}


void CameraControl::OnInit(wxInitDialogEvent& event)
{

}
