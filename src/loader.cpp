#include "loader.h"

BEGIN_EVENT_TABLE(Loader,wxDialog)
//(*EventTable(Loader)
//*)
END_EVENT_TABLE()

Loader::Loader(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(Loader)
    Create(parent,id,_("Building Loader"),wxDefaultPosition,wxDefaultSize,0);
    //*)
}

Loader::~Loader()
{
}


void Loader::OnInit(wxInitDialogEvent& event)
{

}
