//Skyscraper StackWalker interface for reporting information on unhandled exceptions

// For more info about "PreventSetUnhandledExceptionFilter" see:
// "SetUnhandledExceptionFilter" and VC8
// http://blog.kalmbachnet.de/?postid=75
// and
// Unhandled exceptions in VC8 and above� for x86 and x64
// http://blog.kalmbach-software.de/2008/04/02/unhandled-exceptions-in-vc8-and-above-for-x86-and-x64/

#include <windows.h>
#include "stackwalker.h"
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "skyscraper.h"
#include "enginecontext.h"
#include "scriptproc.h"
#include "hal.h"
#include "textwindow.h"

namespace Skyscraper {

TextWindow *twindow = 0;
Skyscraper *skyscraper = 0;

class StackWalkerToConsole : public StackWalker
{
protected:
	virtual void OnOutput(LPCSTR szText)
	{
		twindow->tMain->WriteText(szText);
		//printf("%s", szText);
	}
};

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI
	MyDummySetUnhandledExceptionFilter(
	LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	return NULL;
}

BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	if (hKernel32 == NULL)
		return FALSE;
	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if(pOrgEntry == NULL)
		return FALSE;

	DWORD dwOldProtect = 0;
	SIZE_T jmpSize = 5;
#ifdef _M_X64
	jmpSize = 13;
#endif
	BOOL bProt = VirtualProtect(pOrgEntry, jmpSize,	PAGE_EXECUTE_READWRITE, &dwOldProtect);
	BYTE newJump[20];
	void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
#ifdef _M_IX86
	DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;
	dwOrgEntryAddr += jmpSize; // add 5 for 5 op-codes for jmp rel32
	DWORD dwNewEntryAddr = (DWORD) pNewFunc;
	DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;
	// JMP rel32: Jump near, relative, displacement relative to next instruction.
	newJump[0] = 0xE9;  // JMP rel32
	memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
#elif _M_X64
	newJump[0] = 0x49;  // MOV R15, ...
	newJump[1] = 0xBF;  // ...
	memcpy(&newJump[2], &pNewFunc, sizeof(pNewFunc));
	//pCur += sizeof (ULONG_PTR);
	newJump[10] = 0x41;  // JMP R15, ...
	newJump[11] = 0xFF;  // ...
	newJump[12] = 0xE7;  // ...
#endif
	SIZE_T bytesWritten;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, newJump, jmpSize, &bytesWritten);

	if (bProt != FALSE)
	{
		DWORD dwBuf;
		VirtualProtect(pOrgEntry, jmpSize, dwOldProtect, &dwBuf);
	}
	return bRet;
}

static TCHAR s_szExceptionLogFileName[_MAX_PATH] = _T("\\exceptions.log");  // default
static BOOL s_bUnhandledExeptionFilterSet = FALSE;
static LONG __stdcall CrashHandlerExceptionFilter(EXCEPTION_POINTERS* pExPtrs)
{
#ifdef _M_IX86
	if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		static char MyStack[1024*128];  // be sure that we have enought space...
		// it assumes that DS and SS are the same!!! (this is the case for Win32)
		// change the stack only if the selectors are the same (this is the case for Win32)
		//__asm push offset MyStack[1024*128];
		//__asm pop esp;
		__asm mov eax,offset MyStack[1024*128];
		__asm mov esp,eax;
	}
#endif

	//create text window
	twindow = new TextWindow(NULL, -1);
	twindow->SetTitle(wxT("Simulator Crash Report"));
	twindow->Show(true);

	//stop simulator
	if (skyscraper)
		skyscraper->GetVM()->Pause = true;

	//print exception notice
	CHAR lString[500];
	sprintf_s(lString,
		"*** Unhandled Exception\n"
		"   ExpCode: 0x%8.8X\n"
		"   ExpFlags: %d\n"
		"   ExpAddress: 0x%8.8X\n\n",
		pExPtrs->ExceptionRecord->ExceptionCode,
		pExPtrs->ExceptionRecord->ExceptionFlags,
		pExPtrs->ExceptionRecord->ExceptionAddress);
	twindow->tMain->WriteText(lString);

	//print out stack trace
	StackWalkerToConsole sw;  // output to console
	sw.ShowCallstack(GetCurrentThread(), pExPtrs->ContextRecord);

	if (skyscraper)
	{
		twindow->tMain->WriteText(wxT("\nSimulator State Dump\n--------------------------\n"));
		twindow->tMain->WriteText(wxString("Platform: " + skyscraper->GetVM()->Platform + "\n"));
		twindow->tMain->WriteText(wxString("Renderer: " + skyscraper->GetVM()->GetHAL()->Renderer + "\n"));
		twindow->tMain->WriteText(wxString("Frontend version: " + skyscraper->GetVM()->version_frontend + "\n"));

		//print out simulator state
		EngineContext *context = skyscraper->GetVM()->GetActiveEngine();
		if (context)
		{
			//print out engine state
			twindow->tMain->WriteText(wxString(context->GetSystem()->DumpState()));

			//print out script processor state, if loading
			if (context->IsLoading() == true && context->IsLoadingFinished() == false)
				twindow->tMain->WriteText(wxString(context->GetScriptProcessor()->DumpState()));
		}
	}

	//print instructions
	twindow->tMain->WriteText(wxT("\n--------------------------\n"));
	twindow->tMain->WriteText(wxString("Copy and paste the above information (select it and press CTRL-C)\nand either post it to the Skyscraper forum at http://forum.skyscrapersim.net\nor email to ryan@skyscrapersim.net\n\nPress OK to close Skyscraper\n"));
	twindow->ShowModal();

	if (twindow)
		delete twindow;
	twindow = 0;

 	FatalExit(0);

	return EXCEPTION_CONTINUE_SEARCH;
}

static void InitUnhandledExceptionFilter()
{
	TCHAR szModName[_MAX_PATH];
	if (GetModuleFileName(NULL, szModName, sizeof(szModName)/sizeof(TCHAR)) != 0)
	{
		_tcscpy_s(s_szExceptionLogFileName, szModName);
		_tcscat_s(s_szExceptionLogFileName, _T(".exp.log"));
	}
	if (s_bUnhandledExeptionFilterSet == FALSE)
	{
		// set global exception handler (for handling all unhandled exceptions)
		SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);
		PreventSetUnhandledExceptionFilter();
		s_bUnhandledExeptionFilterSet = TRUE;
	}
}

static void UnhandledExceptionSetRoot(Skyscraper *root)
{
	skyscraper = root;
}

}
