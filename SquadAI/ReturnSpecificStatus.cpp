/* 
*  Kevin Meergans, SquadAI, 2014
*  ReturnSpecificStatus.cpp
*  A decorator that always returns the specific status code that was set on it
*  despite its actual state.
*/

// Includes
#include "ReturnSpecificStatus.h"


ReturnSpecificStatus::ReturnSpecificStatus(const char* name, Behaviour* pChild, BehaviourStatus returnStatus) 
						: Decorator(name, pChild),
						  m_returnStatus(returnStatus)
{
}

ReturnSpecificStatus::~ReturnSpecificStatus(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the return specific status decorator.
//--------------------------------------------------------------------------------------
void ReturnSpecificStatus::OnInitialise(void)
{
	SetReturnStatus(m_returnStatus);
}

//--------------------------------------------------------------------------------------
// Updates the return specific status decorator.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the decorator.
//--------------------------------------------------------------------------------------
BehaviourStatus ReturnSpecificStatus::Update(float deltaTime)
{
	if(!m_pChild)
	{
		// If the child was not initialised properly, indicate a problem by returning invalid
		return StatusInvalid;
	}

	BehaviourStatus status = m_pChild->Tick(deltaTime);
	return m_returnStatus;
}

void ReturnSpecificStatus::SetReturnStatus(BehaviourStatus returnStatus)
{
	m_returnStatus = returnStatus;
}



