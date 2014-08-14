/* 
*  Kevin Meergans, SquadAI, 2014
*  UniversalIndividualBehaviour.cpp
*  Abstract base class for behaviours that can operate on all types of individual entities.
*/

// Includes
#include "UniversalIndividualBehaviour.h"
#include "Entity.h"


UniversalIndividualBehaviour::UniversalIndividualBehaviour(const char* name, Entity* pEntity) : Behaviour(name),
																								m_pEntity(pEntity)													
{
}

UniversalIndividualBehaviour::~UniversalIndividualBehaviour(void)
{
}

// Data access functions

Entity* UniversalIndividualBehaviour::GetEntity(void)
{
	return m_pEntity;
}
