#include "camtex.h"

//(*InternalHeaders(CameraTextureControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(CameraTextureControl)
//*)

namespace Skyscraper {

BEGIN_EVENT_TABLE(CameraTextureControl,wxDialog)
	//(*EventTable(CameraTextureControl)
	//*)
END_EVENT_TABLE()

CameraTextureControl::CameraTextureControl(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(CameraTextureControl)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	//*)
}

CameraTextureControl::~CameraTextureControl()
{
	//(*Destroy(CameraTextureControl)
	//*)
}

void CameraTextureControl::Loop()
{
}

}
