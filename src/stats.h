#ifndef STATS_H
#define STATS_H

//(*Headers(Stats)
#include <wx/dialog.h>
//*)

class Stats: public wxDialog
{
	public:

		Stats(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~Stats();

		//(*Declarations(Stats)
		//*)

	protected:

		//(*Identifiers(Stats)
		//*)

	private:

		//(*Handlers(Stats)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
