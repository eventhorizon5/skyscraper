
// Don't modify comment 
#include "meshcontrol.h"
//[inc]add your include files here


//[inc]end your include
 

MeshControl::MeshControl(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style,const wxString& name)
 VwX_INIT_OBJECTS_MeshControl
{
 OnPreCreate();
 Create(parent,id,title,pos,size,style,name);

 if((pos==wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(0,0,440,345);
 }

 if((pos!=wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(440,345);
 }
 initBefore();
 VwXinit();initAfter();
}
MeshControl::~MeshControl()
{
  DMeshControl();
}
wxPoint& MeshControl::VwXSetwxPoint(long x,long y){
  m_tmppoint.x=x;
  m_tmppoint.y=y;
  return m_tmppoint;
}

wxSize& MeshControl::VwXSetwxSize(long w,long h){
  m_tmpsize.SetWidth(w);
  m_tmpsize.SetHeight(h);
  return m_tmpsize;
}

void MeshControl::VwXinit()
{
 SetTitle(wxT("Realtime Mesh Control"));
 Show(true);
 Refresh();
}
 
BEGIN_EVENT_TABLE(MeshControlEvt,wxEvtHandler)
//[evtEvt]add your code here


//[evtEvt]end your code
END_EVENT_TABLE()
 
//BEGIN_EVENT_TABLE( MeshControl,wxFrame)
//[evtwin]add your code here


//[evtwin]end your code
//END_EVENT_TABLE()

//[evtFunc]add your code here

void MeshControl::OnPreCreate(){
 //add your code here

}

void MeshControl::initBefore(){
 //add your code here

}

void MeshControl::initAfter(){
 //add your code here

}

void MeshControl::DMeshControl(){
 //add your code here

}

//[evtFunc]end your code
