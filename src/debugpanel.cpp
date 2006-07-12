
// Don't modify comment 
#include "debugpanel.h"
//[inc]add your include files here


//[inc]end your include
 

DebugPanel::DebugPanel(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style,const wxString& name)
 VwX_INIT_OBJECTS_DebugPanel
{
 OnPreCreate();
 Create(parent,id,title,pos,size,style,name);

 if((pos==wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(0,0,475,435);
 }

 if((pos!=wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(475,435);
 }
 initBefore();
 VwXinit();initAfter();
}
DebugPanel::~DebugPanel()
{
  DDebugPanel();
}
wxPoint& DebugPanel::VwXSetwxPoint(long x,long y){
  m_tmppoint.x=x;
  m_tmppoint.y=y;
  return m_tmppoint;
}

wxSize& DebugPanel::VwXSetwxSize(long w,long h){
  m_tmpsize.SetWidth(w);
  m_tmpsize.SetHeight(h);
  return m_tmpsize;
}

void DebugPanel::VwXinit()
{
 SetTitle(wxT("Simulator Control Panel"));
 Show(true);
 bListAltitudes=new wxButton(this,-1,wxT(""),VwXSetwxPoint(350,185),VwXSetwxSize(85,25));
   bListAltitudes->SetLabel(wxT("List Altitudes"));
 st4c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(15,10),VwXSetwxSize(74,13),wxST_NO_AUTORESIZE);
   st4c->SetLabel(wxT("Camera Floor:"));
 st5c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(15,30),VwXSetwxSize(89,13),wxST_NO_AUTORESIZE);
   st5c->SetLabel(wxT("Camera Position:"));
 st6c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(15,50),VwXSetwxSize(89,13),wxST_NO_AUTORESIZE);
   st6c->SetLabel(wxT("Elevator Number:"));
 st7c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(15,70),VwXSetwxSize(79,13),wxST_NO_AUTORESIZE);
   st7c->SetLabel(wxT("Elevator Floor:"));
 st8c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(15,90),VwXSetwxSize(64,13),wxST_NO_AUTORESIZE);
   st8c->SetLabel(wxT("Goto Floor:"));
 st9c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(15,110),VwXSetwxSize(89,13),wxST_NO_AUTORESIZE);
   st9c->SetLabel(wxT("Distance To Dest:"));
 st10c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(15,130),VwXSetwxSize(34,13),wxST_NO_AUTORESIZE);
   st10c->SetLabel(wxT("Rate:"));
 st11c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(15,150),VwXSetwxSize(89,13),wxST_NO_AUTORESIZE);
   st11c->SetLabel(wxT("Selected Object:"));
 t_camerafloor=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(105,10),VwXSetwxSize(109,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 t_cameraposition=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(105,30),VwXSetwxSize(109,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 t_elevnumber=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(105,50),VwXSetwxSize(109,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 t_elevfloor=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(105,70),VwXSetwxSize(109,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 t_gotofloor=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(105,90),VwXSetwxSize(109,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 t_disttodest=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(105,110),VwXSetwxSize(109,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 t_rate=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(105,130),VwXSetwxSize(109,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 t_object=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(105,150),VwXSetwxSize(109,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 st22c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(295,10),VwXSetwxSize(119,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
   st22c->SetLabel(wxT("Simulator Options"));
 sb24c=new wxStaticBox(this,-1,wxT(""),VwXSetwxPoint(10,175),VwXSetwxSize(298,216),wxSUNKEN_BORDER);
   sb24c->SetTitle(wxT("Elevator"));
 st25c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(315,35),VwXSetwxSize(99,13),wxST_NO_AUTORESIZE);
   st25c->SetLabel(wxT("Collision Detection"));
 st26c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(315,50),VwXSetwxSize(89,13),wxST_NO_AUTORESIZE);
   st26c->SetLabel(wxT("Frame Limiter"));
 st27c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(315,65),VwXSetwxSize(104,13),wxST_NO_AUTORESIZE);
   st27c->SetLabel(wxT("Main Sim Processing"));
 st28c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(315,80),VwXSetwxSize(99,13),wxST_NO_AUTORESIZE);
   st28c->SetLabel(wxT("Automatic Shafts"));
 st29c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(315,95),VwXSetwxSize(89,13),wxST_NO_AUTORESIZE);
   st29c->SetLabel(wxT("Framerate Sync"));
 chkCollisionDetection=new wxCheckBox(this,-1,wxT(""),VwXSetwxPoint(295,35),VwXSetwxSize(13,13));
 chkFrameLimiter=new wxCheckBox(this,-1,wxT(""),VwXSetwxPoint(295,50),VwXSetwxSize(13,13));
 chkMainProcessing=new wxCheckBox(this,-1,wxT(""),VwXSetwxPoint(295,65),VwXSetwxSize(13,13));
 chkAutoShafts=new wxCheckBox(this,-1,wxT(""),VwXSetwxPoint(295,80),VwXSetwxSize(13,13));
 chkFrameSync=new wxCheckBox(this,-1,wxT(""),VwXSetwxPoint(295,95),VwXSetwxSize(13,13));
 bMeshControl=new wxButton(this,-1,wxT(""),VwXSetwxPoint(325,215),VwXSetwxSize(135,25));
   bMeshControl->SetLabel(wxT("Realtime Mesh Control"));
 bInitRealtime=new wxButton(this,-1,wxT(""),VwXSetwxPoint(350,245),VwXSetwxSize(85,25));
   bInitRealtime->SetLabel(wxT("Init Realtime"));
 s_ElevNum=new wxSlider(this,-1,0,0,100,VwXSetwxPoint(30,210),VwXSetwxSize(250,24));
 st39c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(130,190),VwXSetwxSize(49,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
   st39c->SetLabel(wxT("Number"));
 st40c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(130,235),VwXSetwxSize(49,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
   st40c->SetLabel(wxT("Floor"));
 s_Floor=new wxSlider(this,-1,0,0,100,VwXSetwxPoint(30,255),VwXSetwxSize(250,24));
 st44c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(35,290),VwXSetwxSize(54,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
   st44c->SetLabel(wxT("On Floor"));
 elevFloor=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(35,305),VwXSetwxSize(54,18),wxSUNKEN_BORDER|wxST_NO_AUTORESIZE);
 bEditElevator=new wxButton(this,-1,wxT(""),VwXSetwxPoint(25,340),VwXSetwxSize(75,25));
   bEditElevator->SetLabel(wxT("Edit Elevator"));
 bCallElevator=new wxButton(this,-1,wxT(""),VwXSetwxPoint(115,290),VwXSetwxSize(85,20));
   bCallElevator->SetLabel(wxT("Call Elevator"));
 bGo=new wxButton(this,-1,wxT(""),VwXSetwxPoint(115,310),VwXSetwxSize(85,20));
   bGo->SetLabel(wxT("Go"));
 bOpen=new wxButton(this,-1,wxT(""),VwXSetwxPoint(115,335),VwXSetwxSize(85,20));
   bOpen->SetLabel(wxT("Open"));
 bOpenManual=new wxButton(this,-1,wxT(""),VwXSetwxPoint(115,355),VwXSetwxSize(85,20));
   bOpenManual->SetLabel(wxT("Open Manual"));
 bEnqueueUp=new wxButton(this,-1,wxT(""),VwXSetwxPoint(210,290),VwXSetwxSize(80,20));
   bEnqueueUp->SetLabel(wxT("Enqueue Up"));
 bEnqueueDown=new wxButton(this,-1,wxT(""),VwXSetwxPoint(210,310),VwXSetwxSize(80,20));
   bEnqueueDown->SetLabel(wxT("Enqueue Down"));
 bClose=new wxButton(this,-1,wxT(""),VwXSetwxPoint(210,335),VwXSetwxSize(80,20));
   bClose->SetLabel(wxT("Close"));
 bCloseManual=new wxButton(this,-1,wxT(""),VwXSetwxPoint(210,355),VwXSetwxSize(80,20));
   bCloseManual->SetLabel(wxT("Close Manual"));
 st55c=new wxStaticText(this,-1,wxT(""),VwXSetwxPoint(235,125),VwXSetwxSize(94,33),wxSUNKEN_BORDER|wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
   st55c->SetLabel(wxT("Measurements\r\nare in feet"));
 Refresh();
}
 
BEGIN_EVENT_TABLE(DebugPanelEvt,wxEvtHandler)
//[evtEvt]add your code here


//[evtEvt]end your code
END_EVENT_TABLE()
 
//BEGIN_EVENT_TABLE( DebugPanel,wxFrame)
//[evtwin]add your code here


//[evtwin]end your code
//END_EVENT_TABLE()

//[evtFunc]add your code here

void DebugPanel::OnPreCreate(){
 //add your code here

}

void DebugPanel::initBefore(){
	//add your code here

}

void DebugPanel::initAfter(){
 //add your code here

}

void DebugPanel::DDebugPanel(){
 //add your code here

}

//[evtFunc]end your code
