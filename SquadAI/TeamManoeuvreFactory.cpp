/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamManoeuvreFactory.cpp
*  Class with a static member function used to create team manoeuvres
*  for use by team AIs.
*/

// Includes
#include "TeamManoeuvreFactory.h"
#include "TeamAI.h"
#include "MultiflagCTFTeamAI.h"

//--------------------------------------------------------------------------------------
// Creates a certain team manoeuvre for use by the provided team AI.
// Param1: Determines which manoeuvre will be created from a range of available types.
// Param2: The minimal number of entities required for the execution of the manoeuvre.
// Param3: The maximal number of entities allowed to executed the manoeuvre.
// Param4: A pointer to the team AI that the manoeuvre should be created for.
// Param5: A pointer to a data structure holding additional initialisation data for the manoeuvre.
//		   Should be null if no additional data required for creation of the manoeuvre.
// Returns a pointer to the newly created team manoeuvre. The pointer is null if something failed 
// during creation or if the factory does not support the given manoeuvre type.
//--------------------------------------------------------------------------------------
TeamManoeuvre* TeamManoeuvreFactory::CreateTeamManoeuvre(TeamManoeuvreType type, unsigned int minNumberOfParticipants, unsigned int maxNumberOfParticipants, TeamAI* pTeamAI, void* pAdditionalData)
{
	switch(type)
	{
	case TestAllMoveManoeuvre:
		return new TestManoeuvre(minNumberOfParticipants, maxNumberOfParticipants, pTeamAI);
		break;
	case DefendBaseEntrancesManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		DefendBaseEntrancesInitData* pData = reinterpret_cast<DefendBaseEntrancesInitData*>(pAdditionalData);
		return new DefendBaseEntrances(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI, pData->m_switchPositionsInterval);
		break;
		}
	case RushBaseAttackManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		RushBaseAttackInitData* pData = reinterpret_cast<RushBaseAttackInitData*>(pAdditionalData);
		return new RushBaseAttack(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI, pData->m_waitForParticipantsInterval, pData->m_assemblyPointDistance);
		break;
		}
	default:
		return nullptr;
	}
}