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
		return new RushBaseAttack(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI, pData->m_waitForParticipantsInterval);
		break;
		}
	case RunTheFlagHomeManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		return new RunTheFlagHome(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI);
		break;
		}
	case ReturnDroppedFlagManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		return new ReturnDroppedFlag(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI);
		break;
		}
	case CoordinatedBaseAttackManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		CoordinatedBaseAttackInitData* pData = reinterpret_cast<CoordinatedBaseAttackInitData*>(pAdditionalData);
		return new CoordinatedBaseAttack(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI, pData->m_numberOfAttackGroups, pData->m_waitForParticipantsInterval);
		break;
		}
	case DistractionBaseAttackManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		DistractionBaseAttackInitData* pData = reinterpret_cast<DistractionBaseAttackInitData*>(pAdditionalData);
		return new DistractionBaseAttack(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI, pData->m_numberOfSneakers, pData->m_waitForParticipantsInterval);
		break;
		}
	case SimpleBaseAttackManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		return new SimpleBaseAttack(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI);
		break;
		}
	case PickUpDroppedFlagManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		return new PickUpDroppedFlag(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI);
		break;
		}
	case SimpleBaseDefenceManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		return new SimpleBaseDefence(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI);
		break;
		}
	case ActiveBaseDefenceManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		ActiveBaseDefenceInitData* pData = reinterpret_cast<ActiveBaseDefenceInitData*>(pAdditionalData);
		return new ActiveBaseDefence(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI, pData->m_patrolRadius);
		break;
		}
	case GuardedFlagCaptureManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		GuardedFlagCaptureInitData* pData = reinterpret_cast<GuardedFlagCaptureInitData*>(pAdditionalData);
		return new GuardedFlagCapture(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI, pData->m_guardRadius, pData->m_updateMovementTargetsInterval);
		break;
		}
	case InterceptFlagCarrierManoeuvre:
		{
		MultiflagCTFTeamAI* pCTFTeamAI = dynamic_cast<MultiflagCTFTeamAI*>(pTeamAI);
		if(!pTeamAI)
		{
			return nullptr;
		}
		InterceptFlagCarrierInitData* pData = reinterpret_cast<InterceptFlagCarrierInitData*>(pAdditionalData);
		return new InterceptFlagCarrier(minNumberOfParticipants, maxNumberOfParticipants, pCTFTeamAI, pData->m_searchRadius, pData->m_updateCarrierPositionInterval);
		break;
		}
	default:
		return nullptr;
	}
}