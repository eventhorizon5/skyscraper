
// Don't modify comment 
#include "cameracontrol.h"
//[inc]add your include files here


//[inc]end your include
 

CameraControl::CameraControl(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style,const wxString& name)
 VwX_INIT_OBJECTS_CameraControl
{
 OnPreCreate();
 Create(parent,id,title,pos,size,style,name);

 if((pos==wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(0,0,415,350);
 }

 if((pos!=wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(415,350);
 }
 initBefore();
 VwXinit();initAfter();
}
CameraControl::~CameraControl()
{
  DCameraControl();
}
wxPoint& CameraControl::VwXSetwxPoint(long x,long y){
  m_tmppoint.x=x;
  m_tmppoint.y=y;
  return m_tmppoint;
}

wxSize& CameraControl::VwXSetwxSize(long w,long h){
  m_tmpsize.SetWidth(w);
  m_tmpsize.SetHeight(h);
  return m_tmpsize;
}

void CameraControl::VwXinit()
{
 SetTitle(wxT("Camera Control"));
 Show(true);
 Refresh();
}
 
BEGIN_EVENT_TABLE(CameraControlEvt,wxEvtHandler)
//[evtEvt]add your code here


//[evtEvt]end your code
END_EVENT_TABLE()
 
//BEGIN_EVENT_TABLE( CameraControl,wxFrame)
//[evtwin]add your code here


//[evtwin]end your code
//END_EVENT_TABLE()

//[evtFunc]add your code here

void CameraControl::OnPreCreate(){
 //add your code here

}

void CameraControl::initBefore(){
 //add your code here

}

void CameraControl::initAfter(){
 //add your code here

}

void CameraControl::DCameraControl(){
 //add your code here

}

//[evtFunc]end your code
