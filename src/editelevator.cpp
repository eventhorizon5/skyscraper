
// Don't modify comment 
#include "editelevator.h"
//[inc]add your include files here


//[inc]end your include
 

editelevator::editelevator(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style,const wxString& name)
 VwX_INIT_OBJECTS_editelevator
{
 OnPreCreate();
 Create(parent,id,title,pos,size,style,name);

 if((pos==wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(0,0,455,340);
 }

 if((pos!=wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(455,340);
 }
 initBefore();
 VwXinit();initAfter();
}
editelevator::~editelevator()
{
  Deditelevator();
}
wxPoint& editelevator::VwXSetwxPoint(long x,long y){
  m_tmppoint.x=x;
  m_tmppoint.y=y;
  return m_tmppoint;
}

wxSize& editelevator::VwXSetwxSize(long w,long h){
  m_tmpsize.SetWidth(w);
  m_tmpsize.SetHeight(h);
  return m_tmpsize;
}

void editelevator::VwXinit()
{
 SetTitle(wxT("Elevator Parameters"));
 Show(true);
 Refresh();
}
 
BEGIN_EVENT_TABLE(editelevatorEvt,wxEvtHandler)
//[evtEvt]add your code here


//[evtEvt]end your code
END_EVENT_TABLE()
 
//BEGIN_EVENT_TABLE( editelevator,wxFrame)
//[evtwin]add your code here


//[evtwin]end your code
//END_EVENT_TABLE()

//[evtFunc]add your code here

void editelevator::OnPreCreate(){
 //add your code here

}

void editelevator::initBefore(){
 //add your code here

}

void editelevator::initAfter(){
 //add your code here

}

void editelevator::Deditelevator(){
 //add your code here

}

//[evtFunc]end your code
