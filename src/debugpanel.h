
// Don't modify comment 
#ifndef __VwX_debugpanel_H__
#define __VwX_debugpanel_H__

 #include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
//[inc]add your include files here



//[inc]end your include
 
class DebugPanel;
class DebugPanelEvt:public wxEvtHandler
{
public:
 DebugPanelEvt(DebugPanel *parent){ptr_winPan=parent;}
protected:  
 DebugPanel *ptr_winPan;
 DECLARE_EVENT_TABLE()     
//[evt]add your code here



//[evt]end your code
};

class DebugPanel:public wxFrame
{
 friend class DebugPanelEvt;
protected:
  wxPoint m_tmppoint;
  wxSize  m_tmpsize;
  wxPoint& VwXSetwxPoint(long x,long y);
  wxSize& VwXSetwxSize(long w,long h);
public:
 DebugPanel(wxWindow* parent, wxWindowID id, const wxString& title = wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxT("frame"));
// Pointer control
 wxButton *bListAltitudes;
 wxStaticText *st4c;
 wxStaticText *st5c;
 wxStaticText *st6c;
 wxStaticText *st7c;
 wxStaticText *st8c;
 wxStaticText *st9c;
 wxStaticText *st10c;
 wxStaticText *st11c;
 wxStaticText *t_camerafloor;
 wxStaticText *t_cameraposition;
 wxStaticText *t_elevnumber;
 wxStaticText *t_elevfloor;
 wxStaticText *t_gotofloor;
 wxStaticText *t_disttodest;
 wxStaticText *t_rate;
 wxStaticText *t_object;
 wxStaticText *st22c;
 wxStaticBox *sb24c;
 wxStaticText *st25c;
 wxStaticText *st26c;
 wxStaticText *st27c;
 wxStaticText *st28c;
 wxStaticText *st29c;
 wxCheckBox *chkCollisionDetection;
 wxCheckBox *chkFrameLimiter;
 wxCheckBox *chkMainProcessing;
 wxCheckBox *chkAutoShafts;
 wxCheckBox *chkFrameSync;
 wxButton *bMeshControl;
 wxButton *bInitRealtime;
 wxSlider *s_ElevNum;
 wxStaticText *st39c;
 wxStaticText *st40c;
 wxSlider *s_Floor;
 wxStaticText *st44c;
 wxStaticText *elevFloor;
 wxButton *bEditElevator;
 wxButton *bCallElevator;
 wxButton *bGo;
 wxButton *bOpen;
 wxButton *bOpenManual;
 wxButton *bEnqueueUp;
 wxButton *bEnqueueDown;
 wxButton *bClose;
 wxButton *bCloseManual;
 wxStaticText *st55c;
 virtual ~DebugPanel();
 void DDebugPanel();
 void initBefore();
 void OnPreCreate();
 void VwXinit();
 void initAfter();
protected:
// DECLARE_EVENT_TABLE()  
//[win]add your code here



 #define VwX_INIT_OBJECTS_DebugPanel
//[win]end your code 
};
// end DebugPanel

#endif
