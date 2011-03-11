#ifndef _SBS_PROFILER_H
#define _SBS_PROFILER_H

#include "LinearMath/btQuickprof.h"

#define ENABLE_PROFILING

/***************************************************************************************************
**
** Real-Time Hierarchical Profiling for Game Programming Gems 3
**
** by Greg Hjelstrom & Byon Garrabrant
**
***************************************************************************************************/

///A node in the Profile Hierarchy Tree
class SBSIMPEXP SBSProfileNode {

public:
	SBSProfileNode( const char * name, SBSProfileNode * parent );
	~SBSProfileNode( void );

	SBSProfileNode * Get_Sub_Node( const char * name );

	SBSProfileNode * Get_Parent( void )		{ return Parent; }
	SBSProfileNode * Get_Sibling( void )		{ return Sibling; }
	SBSProfileNode * Get_Child( void )			{ return Child; }

	void				CleanupMemory();
	void				Reset( void );
	void				Call( void );
	bool				Return( void );

	const char *	Get_Name( void )				{ return Name; }
	int				Get_Total_Calls( void )		{ return TotalCalls; }
	float				Get_Total_Time( void )		{ return TotalTime; }

protected:

	const char *	Name;
	int				TotalCalls;
	float				TotalTime;
	unsigned long int			StartTime;
	int				RecursionCounter;

	SBSProfileNode *	Parent;
	SBSProfileNode *	Child;
	SBSProfileNode *	Sibling;
};

///An iterator to navigate through the tree
class SBSIMPEXP SBSProfileIterator
{
public:
	// Access all the children of the current parent
	void				First(void);
	void				Next(void);
	bool				Is_Done(void);
	bool                Is_Root(void) { return (CurrentParent->Get_Parent() == 0); }

	void				Enter_Child( int index );		// Make the given child the new parent
	void				Enter_Largest_Child( void );	// Make the largest child the new parent
	void				Enter_Parent( void );			// Make the current parent's parent the new parent

	// Access the current child
	const char *	Get_Current_Name( void )			{ return CurrentChild->Get_Name(); }
	int				Get_Current_Total_Calls( void )	{ return CurrentChild->Get_Total_Calls(); }
	float				Get_Current_Total_Time( void )	{ return CurrentChild->Get_Total_Time(); }

	// Access the current parent
	const char *	Get_Current_Parent_Name( void )			{ return CurrentParent->Get_Name(); }
	int				Get_Current_Parent_Total_Calls( void )	{ return CurrentParent->Get_Total_Calls(); }
	float				Get_Current_Parent_Total_Time( void )	{ return CurrentParent->Get_Total_Time(); }

protected:

	SBSProfileNode *	CurrentParent;
	SBSProfileNode *	CurrentChild;

	SBSProfileIterator( SBSProfileNode * start );
	friend	class		SBSProfileManager;
};


///The Manager for the Profile system
class SBSIMPEXP SBSProfileManager {
public:
	static	void						Start_Profile( const char * name );
	static	void						Stop_Profile( void );

	static	void						CleanupMemory(void)
	{
		Root.CleanupMemory();
	}

	static	void						Reset( void );
	static	void						Increment_Frame_Counter( void );
	static	int						Get_Frame_Count_Since_Reset( void )		{ return FrameCounter; }
	static	float						Get_Time_Since_Reset( void );

	static	SBSProfileIterator *	Get_Iterator( void )
	{

		return new SBSProfileIterator( &Root );
	}
	static	void						Release_Iterator( SBSProfileIterator * iterator ) { delete ( iterator); }

	static void	dumpRecursive(std::string &output, SBSProfileIterator* profileIterator, int spacing);

	static void	dumpAll();

private:
	static	SBSProfileNode			Root;
	static	SBSProfileNode *			CurrentNode;
	static	int						FrameCounter;
	static	unsigned long int		ResetTime;
};


///ProfileSampleClass is a simple way to profile a function's scope
///Use the SBS_PROFILE macro at the start of scope to time
class SBSIMPEXP SBSProfileSample {
public:
	SBSProfileSample( const char * name );
	~SBSProfileSample( void );
};

#ifdef ENABLE_PROFILING
#define	SBS_PROFILE(name)			SBSProfileSample __profile(name)
#else
#define	SBS_PROFILE(name)
#endif

#endif

