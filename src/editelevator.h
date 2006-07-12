
// Don't modify comment 
#ifndef __VwX_editelevator_H__
#define __VwX_editelevator_H__

 #include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
//[inc]add your include files here



//[inc]end your include
 
class editelevator;
class editelevatorEvt:public wxEvtHandler
{
public:
 editelevatorEvt(editelevator *parent){ptr_winPan=parent;}
protected:  
 editelevator *ptr_winPan;
 DECLARE_EVENT_TABLE()     
//[evt]add your code here



//[evt]end your code
};

class editelevator:public wxFrame
{
 friend class editelevatorEvt;
protected:
  wxPoint m_tmppoint;
  wxSize  m_tmpsize;
  wxPoint& VwXSetwxPoint(long x,long y);
  wxSize& VwXSetwxSize(long w,long h);
public:
 editelevator(wxWindow* parent, wxWindowID id, const wxString& title = wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxT("frame"));
// Pointer control
 virtual ~editelevator();
 void Deditelevator();
 void initBefore();
 void OnPreCreate();
 void VwXinit();
 void initAfter();
protected:
// DECLARE_EVENT_TABLE()  
//[win]add your code here



 #define VwX_INIT_OBJECTS_editelevator
//[win]end your code 
};
// end editelevator

#endif
