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

Behaviour::Behaviour(const char* name) : m_id(++s_BehaviourId),
										 m_name(name),
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
// Param1: The time in seconds passed since the last frame.
// Returns the current status of the behaviour.
//--------------------------------------------------------------------------------------
BehaviourStatus Behaviour::Tick(float deltaTime)
{

	// When the behaviour is ticked the first time, make sure it is
	// initialised properly.
	if(m_status == StatusInvalid)
	{
		OnInitialise();
	}

	// Get the current state of the behaviour.
	m_status = Update(deltaTime);

	// When the behaviour is completed (fail or success), terminate
	// it properly.

	BehaviourStatus returnStatus = m_status;

	if(m_status != StatusRunning)
	{
		OnTerminate(m_status);
	}

	return returnStatus;
}

//--------------------------------------------------------------------------------------
// Aborts the behaviour.
//--------------------------------------------------------------------------------------
void Behaviour::Abort(void)
{
	OnTerminate(StatusAborted);
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
	// Default implementation, do nothing.
}

//--------------------------------------------------------------------------------------
// Terminates the behaviour.
// Param1: The status of the behaviour on termination.
//--------------------------------------------------------------------------------------
void Behaviour::OnTerminate(BehaviourStatus status)
{
	Reset();
}

// Data access functions

unsigned long Behaviour::GetId(void) const
{
	return m_id;
}

const char* Behaviour::GetName(void) const
{
	return m_name;
}

BehaviourStatus Behaviour::GetStatus(void) const
{
	return m_status;
}