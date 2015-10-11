#include <OgreBulletDynamicsWorld.h>
#include "globals.h"
#include "sbs.h"
#include "profiler.h"
#include "unix.h"

extern SBS::SBS *sbs; //external pointer to the SBS engine
static oClock gProfileClock;

namespace SBS {

/*

***************************************************************************************************
**
** profile.cpp
**
** Real-Time Hierarchical Profiling for Game Programming Gems 3
**
** by Greg Hjelstrom & Byon Garrabrant
**
***************************************************************************************************/

inline void Profile_Get_Ticks(unsigned long int * ticks)
{
	*ticks = gProfileClock.getTimeMicroseconds();
}

inline float Profile_Get_Tick_Rate(void)
{
//	return 1000000.f;
	return 1000.f;

}

/***************************************************************************************************
**
** SBSProfileNode
**
***************************************************************************************************/

/***********************************************************************************************
 * INPUT:                                                                                      *
 * name - pointer to a static string which is the name of this profile node                    *
 * parent - parent pointer                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * The name is assumed to be a static pointer, only the pointer is stored and compared for     *
 * efficiency reasons.                                                                         *
 *=============================================================================================*/
SBSProfileNode::SBSProfileNode( const char * name, SBSProfileNode * parent ) :
	Name( name ),
	TotalCalls( 0 ),
	TotalTime( 0 ),
	StartTime( 0 ),
	RecursionCounter( 0 ),
	Parent( parent ),
	Child( NULL ),
	Sibling( NULL )
{
	Reset();
}


void	SBSProfileNode::CleanupMemory()
{
	delete ( Child);
	Child = NULL;
	delete ( Sibling);
	Sibling = NULL;
}

SBSProfileNode::~SBSProfileNode( void )
{
	delete ( Child);
	delete ( Sibling);
}


/***********************************************************************************************
 * INPUT:                                                                                      *
 * name - static string pointer to the name of the node we are searching for                   *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * All profile names are assumed to be static strings so this function uses pointer compares   *
 * to find the named node.                                                                     *
 *=============================================================================================*/
SBSProfileNode * SBSProfileNode::Get_Sub_Node( const char * name )
{
	// Try to find this sub node
	SBSProfileNode * child = Child;
	while ( child ) {
		if ( child->Name == name ) {
			return child;
		}
		child = child->Sibling;
	}

	// We didn't find it, so add it

	SBSProfileNode * node = new SBSProfileNode( name, this );
	node->Sibling = Child;
	Child = node;
	return node;
}


void	SBSProfileNode::Reset( void )
{
	TotalCalls = 0;
	TotalTime = 0.0f;


	if ( Child ) {
		Child->Reset();
	}
	if ( Sibling ) {
		Sibling->Reset();
	}
}


void	SBSProfileNode::Call( void )
{
	TotalCalls++;
	if (RecursionCounter++ == 0) {
		Profile_Get_Ticks(&StartTime);
	}
}


bool	SBSProfileNode::Return( void )
{
	if ( --RecursionCounter == 0 && TotalCalls != 0 ) {
		unsigned long int time;
		Profile_Get_Ticks(&time);
		time -= StartTime;
		TotalTime += (float)time / Profile_Get_Tick_Rate();
	}
	return ( RecursionCounter == 0 );
}


/***************************************************************************************************
**
** SBSProfileIterator
**
***************************************************************************************************/
SBSProfileIterator::SBSProfileIterator( SBSProfileNode * start )
{
	CurrentParent = start;
	CurrentChild = CurrentParent->Get_Child();
}


void	SBSProfileIterator::First(void)
{
	CurrentChild = CurrentParent->Get_Child();
}


void	SBSProfileIterator::Next(void)
{
	CurrentChild = CurrentChild->Get_Sibling();
}


bool	SBSProfileIterator::Is_Done(void)
{
	return CurrentChild == NULL;
}


void	SBSProfileIterator::Enter_Child( int index )
{
	CurrentChild = CurrentParent->Get_Child();
	while ( (CurrentChild != NULL) && (index != 0) ) {
		index--;
		CurrentChild = CurrentChild->Get_Sibling();
	}

	if ( CurrentChild != NULL ) {
		CurrentParent = CurrentChild;
		CurrentChild = CurrentParent->Get_Child();
	}
}


void	SBSProfileIterator::Enter_Parent( void )
{
	if ( CurrentParent->Get_Parent() != NULL ) {
		CurrentParent = CurrentParent->Get_Parent();
	}
	CurrentChild = CurrentParent->Get_Child();
}


/***************************************************************************************************
**
** SBSProfileManager
**
***************************************************************************************************/

SBSProfileNode	SBSProfileManager::Root( "Root", NULL );
SBSProfileNode *	SBSProfileManager::CurrentNode = &SBSProfileManager::Root;
int				SBSProfileManager::FrameCounter = 0;
unsigned long int			SBSProfileManager::ResetTime = 0;


/***********************************************************************************************
 * SBSProfileManager::Start_Profile -- Begin a named profile                                    *
 *                                                                                             *
 * Steps one level deeper into the tree, if a child already exists with the specified name     *
 * then it accumulates the profiling; otherwise a new child node is added to the profile tree. *
 *                                                                                             *
 * INPUT:                                                                                      *
 * name - name of this profiling record                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * The string used is assumed to be a static string; pointer compares are used throughout      *
 * the profiling code for efficiency.                                                          *
 *=============================================================================================*/
void	SBSProfileManager::Start_Profile( const char * name )
{
	if (!sbs)
		return;
	if (sbs->enable_profiling == false)
		return;

	if (name != CurrentNode->Get_Name()) {
		CurrentNode = CurrentNode->Get_Sub_Node( name );
	}

	CurrentNode->Call();
}


/***********************************************************************************************
 * SBSProfileManager::Stop_Profile -- Stop timing and record the results.                       *
 *=============================================================================================*/
void	SBSProfileManager::Stop_Profile( void )
{
	if (!sbs)
		return;
	if (sbs->enable_profiling == false)
		return;

	// Return will indicate whether we should back up to our parent (we may
	// be profiling a recursive function)
	if (CurrentNode->Return()) {
		CurrentNode = CurrentNode->Get_Parent();
	}
}


/***********************************************************************************************
 * SBSProfileManager::Reset -- Reset the contents of the profiling system                       *
 *                                                                                             *
 *    This resets everything except for the tree structure.  All of the timing data is reset.  *
 *=============================================================================================*/
void	SBSProfileManager::Reset( void )
{
	if (!sbs)
		return;
	if (sbs->enable_profiling == false)
		return;

	gProfileClock.reset();
	Root.Reset();
	Root.Call();
	FrameCounter = 0;
	Profile_Get_Ticks(&ResetTime);
}


/***********************************************************************************************
 * SBSProfileManager::Increment_Frame_Counter -- Increment the frame counter                    *
 *=============================================================================================*/
void SBSProfileManager::Increment_Frame_Counter( void )
{
	if (!sbs)
		return;
	if (sbs->enable_profiling == false)
		return;

	FrameCounter++;
}


/***********************************************************************************************
 * SBSProfileManager::Get_Time_Since_Reset -- returns the elapsed time since last reset         *
 *=============================================================================================*/
float SBSProfileManager::Get_Time_Since_Reset( void )
{
	unsigned long int time;
	Profile_Get_Ticks(&time);
	time -= ResetTime;
	return (float)time / Profile_Get_Tick_Rate();
}

#include <stdio.h>

void SBSProfileManager::dumpRecursive(std::string &output, SBSProfileIterator* profileIterator, int spacing)
{
	if (!sbs)
		return;
	if (sbs->enable_profiling == false)
		return;

	profileIterator->First();
	if (profileIterator->Is_Done())
		return;

	float accumulated_time=0, parent_time = profileIterator->Is_Root() ? SBSProfileManager::Get_Time_Since_Reset() : profileIterator->Get_Current_Parent_Total_Time();
	int frames_since_reset = SBSProfileManager::Get_Frame_Count_Since_Reset();
	for (int i = 0; i < spacing; i++)
		output.append(".");
	output.append("----------------------------------\n");
	for (int i = 0; i < spacing; i++)
		output.append(".");
	char buffer[1000];
	_snprintf(buffer, 1000, "Profiling: %s (total running time: %.3f ms) ---\n",	profileIterator->Get_Current_Parent_Name(), parent_time);
	output.append(buffer);
	float totalTime = 0.f;


	int numChildren = 0;

	for (int i = 0; !profileIterator->Is_Done(); i++, profileIterator->Next())
	{
		numChildren++;
		float current_total_time = profileIterator->Get_Current_Total_Time();
		accumulated_time += current_total_time;
		float fraction = parent_time > SIMD_EPSILON ? (current_total_time / parent_time) * 100 : 0.f;
		for (int j = 0; j < spacing; j++)
			output.append(".");
		_snprintf(buffer, 1000, "%d -- %s (%.2f %%) :: %.3f ms / frame (%d calls)\n", i, profileIterator->Get_Current_Name(), fraction,(current_total_time / (double)frames_since_reset), profileIterator->Get_Current_Total_Calls());
		output.append(buffer);
		totalTime += current_total_time;
		//recurse into children
	}

	if (parent_time < accumulated_time)
	{
		output.append("what's wrong\n");
	}
	for (int i = 0; i < spacing; i++)
		output.append(".");
	_snprintf(buffer, 1000, "%s (%.3f %%) :: %.3f ms\n", "Unaccounted:", parent_time > SIMD_EPSILON ? ((parent_time - accumulated_time) / parent_time) * 100 : 0.f, parent_time - accumulated_time);
	output.append(buffer);

	for (int i = 0; i < numChildren; i++)
	{
		profileIterator->Enter_Child(i);
		dumpRecursive(output, profileIterator, spacing + 3);
		profileIterator->Enter_Parent();
	}
}

void SBSProfileManager::dumpAll()
{
	SBSProfileIterator* profileIterator = 0;
	profileIterator = SBSProfileManager::Get_Iterator();

	std::string output;
	dumpRecursive(output, profileIterator, 0);
	printf("%s\n", output.c_str());

	SBSProfileManager::Release_Iterator(profileIterator);
}

SBSProfileSample::SBSProfileSample( const char * name, bool advanced )
{
	is_advanced = advanced;
	if (!sbs)
		return;
	if (is_advanced == true && sbs->enable_advanced_profiling == false)
		return;
	SBSProfileManager::Start_Profile( name );
}

SBSProfileSample::~SBSProfileSample( void )
{
	if (!sbs)
		return;
	if (is_advanced == true && sbs->enable_advanced_profiling == false)
		return;
	SBSProfileManager::Stop_Profile();
}

}
