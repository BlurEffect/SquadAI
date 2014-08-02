/* 
*  Kevin Meergans, SquadAI, 2014
*  ReturnSpecificStatus.h
*  A decorator that always returns the specific status code that was set on it
*  despite its actual state.
*/

#ifndef RETURN_SPECIFIC_STATUS_H
#define RETURN_SPECIFIC_STATUS_H

// Includes
#include "Decorator.h"

//--------------------------------------------------------------------------------------
// Contains all additional data required for the creation of a parallel behaviour.
//--------------------------------------------------------------------------------------
struct ReturnSpecificStatusInitData
{
	ReturnSpecificStatusInitData(Behaviour* pChild, BehaviourStatus returnStatus) : m_pChild(pChild),
																	                m_returnStatus(returnStatus)
	{}

	Behaviour*      m_pChild;
	BehaviourStatus m_returnStatus;
};

class ReturnSpecificStatus : public Decorator
{
public:
	ReturnSpecificStatus(Entity* pEntity, const char* name, Behaviour* pChild, BehaviourStatus returnStatus);
	~ReturnSpecificStatus(void);

	void SetReturnStatus(BehaviourStatus returnStatus);

private:
	void OnInitialise(void);
	BehaviourStatus Update(float deltaTime);

	BehaviourStatus m_returnStatus;
};

#endif // RETURN_SPECIFIC_STATUS_H