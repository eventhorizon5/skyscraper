#include "editelevator.h"

BEGIN_EVENT_TABLE(editelevator,wxFrame)
//(*EventTable(editelevator)
    EVT_INIT_DIALOG(editelevator::OnInit)
//*)
END_EVENT_TABLE()

editelevator::editelevator(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(editelevator)
    Create(parent,id,_("Elevator Parameters"),wxDefaultPosition,wxSize(159,134),0);
    //*)
}

editelevator::~editelevator()
{
}


void editelevator::OnInit(wxInitDialogEvent& event)
{

}
