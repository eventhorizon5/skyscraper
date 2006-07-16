#include "meshcontrol.h"

BEGIN_EVENT_TABLE(MeshControl,wxFrame)
//(*EventTable(MeshControl)
//*)
END_EVENT_TABLE()

MeshControl::MeshControl(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(MeshControl)
    Create(parent,id,_("Realtime Mesh Control"),wxDefaultPosition,wxSize(43,30),0);
    //*)
}

MeshControl::~MeshControl()
{
}


void MeshControl::OnInit(wxInitDialogEvent& event)
{

}
