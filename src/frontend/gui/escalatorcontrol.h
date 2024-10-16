#ifndef ESCALATORCONTROL_H
#define ESCALATORCONTROL_H

//(*Headers(EscalatorControl)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class EscalatorControl: public wxDialog
{
    public:

        EscalatorControl(wxWindow* parent,wxWindowID id=wxID_ANY);
        virtual ~EscalatorControl();

        //(*Declarations(EscalatorControl)
        wxButton* bOK;
        wxCheckBox* chkRun;
        wxListBox* ListBox1;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxTextCtrl* txtName;
        wxTextCtrl* txtParent;
        //*)

    protected:

        //(*Identifiers(EscalatorControl)
        static const long ID_LISTBOX1;
        static const long ID_STATICTEXT1;
        static const long ID_txtName;
        static const long ID_STATICTEXT2;
        static const long ID_txtParent;
        static const long ID_chkRun;
        static const long ID_bOK;
        //*)

    private:

        //(*Handlers(EscalatorControl)
        void On_chkRun_Click(wxCommandEvent& event);
        void On_bOK_Click(wxCommandEvent& event);
        //*)

        DECLARE_EVENT_TABLE()
};

#endif
