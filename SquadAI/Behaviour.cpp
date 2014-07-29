/* 
*  Kevin Meergans, SquadAI, 2014
*  Behaviour.cpp
*  Abstract base class for entity behaviours.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

// Includes
#include "Behaviour.h"

unsigned long Behaviour::s_BehaviourId = 0;

Behaviour::Behaviour(void) : m_id(++s_BehaviourId),
							 m_status(StatusInvalid)
{
}

//--------------------------------------------------------------------------------------
// Custom copy constructor to ensure that the id stays unique.
//--------------------------------------------------------------------------------------
Behaviour::Behaviour(const Behaviour& sourceBehaviour)
{
	m_id     = ++s_BehaviourId;
	m_status = sourceBehaviour.GetStatus();
}

//--------------------------------------------------------------------------------------
// Custom assignment operator to ensure that the id stays unique.
//--------------------------------------------------------------------------------------
Behaviour& Behaviour::operator= (const Behaviour& sourceBehaviour)
{
	m_id     = ++s_BehaviourId;
	m_status = sourceBehaviour.GetStatus();

	return *this;
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
// Aborts the behaviour.
//--------------------------------------------------------------------------------------
void Behaviour::Abort(void)
{
	OnTerminate(StatusAborted);
	m_status = StatusAborted;
}

//--------------------------------------------------------------------------------------
// Resets the behaviour.
//--------------------------------------------------------------------------------------
void Behaviour::Reset(void)
{
	m_status = StatusInvalid;
}

bool Behaviour::IsTerminated(void) const
{
	return (m_status == StatusSuccess || m_status == StatusFailure);
}

bool Behaviour::IsRunning(void) const
{
	return (m_status == StatusRunning);
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

unsigned long Behaviour::GetId(void) const
{

}

BehaviourStatus Behaviour::GetStatus(void) const
{
	return m_status;
}