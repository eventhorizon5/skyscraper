/* $Id$ */
#ifndef _SBS_PROFILER_H
#define _SBS_PROFILER_H

namespace SBS {

#define ENABLE_PROFILING

/***************************************************************************************************
**
** Real-Time Hierarchical Profiling for Game Programming Gems 3
**
** by Greg Hjelstrom & Byon Garrabrant
**
***************************************************************************************************/

///A node in the Profile Hierarchy Tree
class SBSIMPEXP ProfileNode {

public:
	ProfileNode( const char * name, ProfileNode * parent );
	~ProfileNode( void );

	ProfileNode * Get_Sub_Node( const char * name );

	ProfileNode * Get_Parent( void )		{ return Parent; }
	ProfileNode * Get_Sibling( void )		{ return Sibling; }
	ProfileNode * Get_Child( void )			{ return Child; }

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

	ProfileNode *	Parent;
	ProfileNode *	Child;
	ProfileNode *	Sibling;
};

///An iterator to navigate through the tree
class SBSIMPEXP ProfileIterator
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

	ProfileNode *	CurrentParent;
	ProfileNode *	CurrentChild;

	ProfileIterator( ProfileNode * start );
	friend	class		ProfileManager;
};


///The Manager for the Profile system
class SBSIMPEXP ProfileManager {
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

	static	ProfileIterator *	Get_Iterator( void )
	{

		return new ProfileIterator( &Root );
	}
	static	void						Release_Iterator( ProfileIterator * iterator ) { delete ( iterator); }

	static void	dumpRecursive(std::string &output, ProfileIterator* profileIterator, int spacing);

	static void	dumpAll(std::string &output);

private:
	static	ProfileNode			Root;
	static	ProfileNode *			CurrentNode;
	static	int						FrameCounter;
	static	unsigned long int		ResetTime;
};


///ProfileSampleClass is a simple way to profile a function's scope
///Use the SBS_PROFILE macro at the start of scope to time
class SBSIMPEXP ProfileSample {
public:
	ProfileSample( const char * name, bool advanced = true );
	~ProfileSample( void );
private:
	bool is_advanced;
};

#ifdef ENABLE_PROFILING
#define	SBS_PROFILE(name)			ProfileSample __profile(name, true)
#define	SBS_PROFILE_MAIN(name)			ProfileSample __profile(name, false)
#else
#define	SBS_PROFILE(name)
#endif

}

#endif

