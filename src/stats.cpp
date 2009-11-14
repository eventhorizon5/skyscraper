#include "stats.h"

//(*InternalHeaders(Stats)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(Stats)
//*)

BEGIN_EVENT_TABLE(Stats,wxDialog)
	//(*EventTable(Stats)
	//*)
END_EVENT_TABLE()

Stats::Stats(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(Stats)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	//*)
}

Stats::~Stats()
{
	//(*Destroy(Stats)
	//*)
}

