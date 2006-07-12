
// Don't modify comment 
#ifndef __VwX_loader_H__
#define __VwX_loader_H__

 #include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
//[inc]add your include files here



//[inc]end your include
 
#include <wx/dirctrl.h>
class Loader;
class LoaderEvt:public wxEvtHandler
{
public:
 LoaderEvt(Loader *parent){ptr_winPan=parent;}
protected:  
 Loader *ptr_winPan;
 DECLARE_EVENT_TABLE()     
//[evt]add your code here



//[evt]end your code
};

class Loader:public wxDialog
{
 friend class LoaderEvt;
protected:
  wxPoint m_tmppoint;
  wxSize  m_tmpsize;
  wxPoint& VwXSetwxPoint(long x,long y);
  wxSize& VwXSetwxSize(long w,long h);
public:
 Loader(wxWindow* parent, wxWindowID id = -1, const wxString& title = wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("dialogBox"));
// Pointer control
 wxGenericDirCtrl *ltf3c;
 virtual ~Loader();
 void DLoader();
 void initBefore();
 void OnPreCreate();
 void VwXinit();
 void initAfter();
protected:
// DECLARE_EVENT_TABLE()  
//[win]add your code here



 #define VwX_INIT_OBJECTS_Loader
//[win]end your code 
};
// end Loader

#endif
