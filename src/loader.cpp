
// Don't modify comment 
#include "loader.h"
//[inc]add your include files here


//[inc]end your include
 

Loader::Loader(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style,const wxString& name)
 VwX_INIT_OBJECTS_Loader
{
 OnPreCreate();
 Create(parent,id,title,pos,size,style,name);

 if((pos==wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(0,0,325,260);
 }

 if((pos!=wxDefaultPosition)&&(size==wxDefaultSize)){
     SetSize(325,260);
 }
 initBefore();
 VwXinit();initAfter();
}
Loader::~Loader()
{
  DLoader();
}
wxPoint& Loader::VwXSetwxPoint(long x,long y){
  m_tmppoint.x=x;
  m_tmppoint.y=y;
  return m_tmppoint;
}

wxSize& Loader::VwXSetwxSize(long w,long h){
  m_tmpsize.SetWidth(w);
  m_tmpsize.SetHeight(h);
  return m_tmpsize;
}

void Loader::VwXinit()
{
 SetTitle(wxT("Building Loader"));
 ltf3c=new wxGenericDirCtrl(this,-1,wxDirDialogDefaultFolderStr,VwXSetwxPoint(15,15),VwXSetwxSize(195,170));
 Refresh();
}
 
BEGIN_EVENT_TABLE(LoaderEvt,wxEvtHandler)
//[evtEvt]add your code here


//[evtEvt]end your code
END_EVENT_TABLE()
 
//BEGIN_EVENT_TABLE( Loader,wxDialog)
//[evtwin]add your code here


//[evtwin]end your code
//END_EVENT_TABLE()

//[evtFunc]add your code here

void Loader::OnPreCreate(){
 //add your code here

}

void Loader::initBefore(){
 //add your code here

}

void Loader::initAfter(){
 //add your code here

}

void Loader::DLoader(){
 //add your code here

}

//[evtFunc]end your code
