/* 
*  Kevin Meergans, SquadAI, 2014
*  Behaviour.cpp
*  Abstract base class for entity behaviours.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*/

// Includes
#include "Behaviour.h"

Behaviour::Behaviour(void) : m_status(StatusInvalid)
{
}

Behaviour::~Behaviour(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the behaviour and ensures it is properly intialised and terminated.
// Returns the current status of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus Behaviour::Tick(void)
{
	// When the behaviour is ticked the first time, make sure it is
	// initialised properly.
	if(m_status == StatusInvalid)
	{
		OnInitialise();
	}

	// Get the current state of the behaviour.
	m_status = Update();

	// When the behaviour is completed (fail or success), terminate
	// it properly.
	if(m_status != StatusRunning)
	{
		OnTerminate(m_status);
	}

	return m_status;
}

//--------------------------------------------------------------------------------------
// Initialises the behaviour.
//--------------------------------------------------------------------------------------
void Behaviour::OnInitialise(void)
{

}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
//--------------------------------------------------------------------------------------
void Behaviour::OnTerminate(BehaviourStatus status)
{

}