/* 
*  Kevin Meergans, SquadAI, 2014
*  UniversalIndividualBehaviour.h
*  Abstract base class for behaviours that can operate on all types of individual entities.
*/

#ifndef UNIVERSAL_INDIVIDUAL_BEHAVIOUR_H
#define UNIVERSAL_INDIVIDUAL_BEHAVIOUR_H

// Includes
#include "Behaviour.h"

// Forward declarations
class Entity;

class UniversalIndividualBehaviour : public Behaviour
{
public:
	UniversalIndividualBehaviour(const char* name, Entity* pEntity);
	virtual ~UniversalIndividualBehaviour(void);
	
	// Data access functions
	Entity* GetEntity(void);

private:
	Entity*         m_pEntity; // The entity that this behaviour belongs to

};

#endif // UNIVERSAL_INDIVIDUAL_BEHAVIOUR_H