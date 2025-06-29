#include <OgreBulletDynamicsWorld.h>
#include <OgrePlatform.h>
#include "globals.h"
#include "sbs.h"
#include "profiler.h"

static oClock gProfileClock;

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
	#define _snprintf snprintf
#endif

namespace SBS {

bool SBSIMPEXP enable_profiling; //enable general profiling
bool SBSIMPEXP enable_advanced_profiling;

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
** ProfileNode
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
ProfileNode::ProfileNode( const char * name, ProfileNode * parent ) :
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


void	ProfileNode::CleanupMemory()
{
	delete ( Child);
	Child = NULL;
	delete ( Sibling);
	Sibling = NULL;
}

ProfileNode::~ProfileNode( void )
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
ProfileNode * ProfileNode::Get_Sub_Node( const char * name )
{
	// Try to find this sub node
	ProfileNode * child = Child;
	while ( child ) {
		if ( child->Name == name ) {
			return child;
		}
		child = child->Sibling;
	}

	// We didn't find it, so add it

	ProfileNode * node = new ProfileNode( name, this );
	node->Sibling = Child;
	Child = node;
	return node;
}


void	ProfileNode::Reset( void )
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


void	ProfileNode::Call( void )
{
	TotalCalls++;
	if (RecursionCounter++ == 0) {
		Profile_Get_Ticks(&StartTime);
	}
}


bool	ProfileNode::Return( void )
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
** ProfileIterator
**
***************************************************************************************************/
ProfileIterator::ProfileIterator( ProfileNode * start )
{
	CurrentParent = start;
	CurrentChild = CurrentParent->Get_Child();
}


void	ProfileIterator::First(void)
{
	CurrentChild = CurrentParent->Get_Child();
}


void	ProfileIterator::Next(void)
{
	CurrentChild = CurrentChild->Get_Sibling();
}


bool	ProfileIterator::Is_Done(void)
{
	return CurrentChild == NULL;
}


void	ProfileIterator::Enter_Child( int index )
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


void	ProfileIterator::Enter_Parent( void )
{
	if ( CurrentParent->Get_Parent() != NULL ) {
		CurrentParent = CurrentParent->Get_Parent();
	}
	CurrentChild = CurrentParent->Get_Child();
}


/***************************************************************************************************
**
** ProfileManager
**
***************************************************************************************************/

ProfileNode	ProfileManager::Root( "Root", NULL );
ProfileNode *	ProfileManager::CurrentNode = &ProfileManager::Root;
int				ProfileManager::FrameCounter = 0;
unsigned long int			ProfileManager::ResetTime = 0;


/***********************************************************************************************
 * ProfileManager::Start_Profile -- Begin a named profile                                    *
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
void	ProfileManager::Start_Profile( const char * name )
{
	if (enable_profiling == false)
		return;

	if (name != CurrentNode->Get_Name()) {
		CurrentNode = CurrentNode->Get_Sub_Node( name );
	}

	CurrentNode->Call();
}


/***********************************************************************************************
 * ProfileManager::Stop_Profile -- Stop timing and record the results.                       *
 *=============================================================================================*/
void	ProfileManager::Stop_Profile( void )
{
	if (enable_profiling == false)
		return;

	// Return will indicate whether we should back up to our parent (we may
	// be profiling a recursive function)
	if (CurrentNode->Return()) {
		CurrentNode = CurrentNode->Get_Parent();
	}
}


/***********************************************************************************************
 * ProfileManager::Reset -- Reset the contents of the profiling system                       *
 *                                                                                             *
 *    This resets everything except for the tree structure.  All of the timing data is reset.  *
 *=============================================================================================*/
void	ProfileManager::Reset( void )
{
	if (enable_profiling == false)
		return;

	gProfileClock.reset();
	Root.Reset();
	Root.Call();
	FrameCounter = 0;
	Profile_Get_Ticks(&ResetTime);
}


/***********************************************************************************************
 * ProfileManager::Increment_Frame_Counter -- Increment the frame counter                    *
 *=============================================================================================*/
void ProfileManager::Increment_Frame_Counter( void )
{
	if (enable_profiling == false)
		return;

	FrameCounter++;
}


/***********************************************************************************************
 * ProfileManager::Get_Time_Since_Reset -- returns the elapsed time since last reset         *
 *=============================================================================================*/
float ProfileManager::Get_Time_Since_Reset( void )
{
	unsigned long int time;
	Profile_Get_Ticks(&time);
	time -= ResetTime;
	return (float)time / Profile_Get_Tick_Rate();
}

#include <stdio.h>

void ProfileManager::dumpRecursive(std::string &output, ProfileIterator* profileIterator, int spacing)
{
	if (enable_profiling == false)
		return;

	profileIterator->First();
	if (profileIterator->Is_Done())
		return;

	float accumulated_time=0, parent_time = profileIterator->Is_Root() ? ProfileManager::Get_Time_Since_Reset() : profileIterator->Get_Current_Parent_Total_Time();
	int frames_since_reset = ProfileManager::Get_Frame_Count_Since_Reset();
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

void ProfileManager::dumpAll(std::string &output)
{
	ProfileIterator* profileIterator = 0;
	profileIterator = ProfileManager::Get_Iterator();

	dumpRecursive(output, profileIterator, 0);
	//printf("%s\n", output.c_str());

	ProfileManager::Release_Iterator(profileIterator);
}

ProfileSample::ProfileSample( const char * name, bool advanced )
{
	is_advanced = advanced;
	if (is_advanced == true && enable_advanced_profiling == false)
		return;
	ProfileManager::Start_Profile( name );
}

ProfileSample::~ProfileSample( void )
{
	if (is_advanced == true && enable_advanced_profiling == false)
		return;
	ProfileManager::Stop_Profile();
}

}
