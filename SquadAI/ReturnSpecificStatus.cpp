/* 
*  Kevin Meergans, SquadAI, 2014
*  ReturnSpecificStatus.cpp
*  A decorator that always returns the specific status code that was set on it
*  despite its actual state.
*/

// Includes
#include "ReturnSpecificStatus.h"


ReturnSpecificStatus::ReturnSpecificStatus(Entity* pEntity, const char* name, Behaviour* pChild, BehaviourStatus returnStatus) 
						: Decorator(pEntity, name, pChild),
						  m_returnStatus(returnStatus)
{
}

ReturnSpecificStatus::~ReturnSpecificStatus(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the repeat decorator.
//--------------------------------------------------------------------------------------
void ReturnSpecificStatus::OnInitialise(void)
{
	SetReturnStatus(m_returnStatus);
}

//--------------------------------------------------------------------------------------
// Updates the repeat decorator.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the repeat decorator.
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



