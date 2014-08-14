/* 
*  Kevin Meergans, SquadAI, 2014
*  CombatManager.cpp
*  Each entity able to fight has a combat manager associated to it that
*  is responsible for controlling the attack and defense actions of the entity.
*/

// Includes
#include "EntityCombatManager.h"
#include "Entity.h"
#include "TestEnvironment.h"

EntityCombatManager::EntityCombatManager(void) : m_pEntity(nullptr),
												 m_pEnvironment(nullptr)
{
}

EntityCombatManager::~EntityCombatManager(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the combat manager.
// Param1: A pointer to the entity that the combat manager is associated to.
// Param2: A pointer to the test environment that the entity belongs to.
// Returns true if the combat component was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool EntityCombatManager::Initialise(Entity* pEntity, TestEnvironment* pTestEnvironment)
{
	if(!pEntity || !pTestEnvironment)
	{
		return false;
	}

	m_pEntity	   = pEntity;
	m_pEnvironment = pTestEnvironment;

	return true;
}

//--------------------------------------------------------------------------------------
// Resets the combat component of the entity.
//--------------------------------------------------------------------------------------
void EntityCombatManager::Reset(void)
{
	
}

//--------------------------------------------------------------------------------------
// Determines and sets the currently greatest known threat based on the distance of the
// threats to the entity. It also takes into account whether threats have successfully 
// attacked the entity to give special prioritisation to these threats.
//--------------------------------------------------------------------------------------
void EntityCombatManager::DetermineGreatestKnownThreat(void)
{
	if(!m_pEntity->GetKnownThreats().empty())
	{
		float shortestSquareDistance = std::numeric_limits<float>::max();
		float shortestPrioritySquareDistance = std::numeric_limits<float>::max();
		KnownThreat* pGreatestKnownThreat = nullptr;
		KnownThreat* pPriorityKnownThreat = nullptr;

		for(std::vector<KnownThreat>::iterator it = m_pEntity->GetKnownThreats().begin(); it != m_pEntity->GetKnownThreats().end(); ++it)
		{
			float squareDistance = 0.0f;
			XMVECTOR vector = XMLoadFloat2(&it->m_pEntity->GetPosition()) - XMLoadFloat2(&m_pEntity->GetPosition());
			XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));
			
			if(squareDistance < shortestSquareDistance)
			{
				pGreatestKnownThreat = &(*it);
				shortestSquareDistance = squareDistance;
			}
			if(it->m_hasHitEntity && squareDistance < shortestPrioritySquareDistance)
			{
				pPriorityKnownThreat = &(*it);
				shortestPrioritySquareDistance = squareDistance;
			}
		}

		if(pPriorityKnownThreat)
		{
			m_pEntity->SetGreatestKnownThreat(pPriorityKnownThreat);
		}else
		{
			m_pEntity->SetGreatestKnownThreat(pGreatestKnownThreat);
		}

	}else
	{
		m_pEntity->SetGreatestKnownThreat(nullptr);
	}
}

//--------------------------------------------------------------------------------------
// Determines and sets the currently greatest suspected threat based on the distance of the
// threats to the entity. It also takes into account whether threats have successfully 
// attacked the entity to give special prioritisation to these threats.
//--------------------------------------------------------------------------------------
void EntityCombatManager::DetermineGreatestSuspectedThreat(void)
{
	if(!m_pEntity->GetSuspectedThreats().empty())
	{
		float shortestSquareDistance = std::numeric_limits<float>::max();
		float shortestPrioritySquareDistance = std::numeric_limits<float>::max();
		SuspectedThreat* pGreatestSuspectedThreat = nullptr;
		SuspectedThreat* pPrioritySuspectedThreat = nullptr;

		for(std::vector<SuspectedThreat>::iterator it = m_pEntity->GetSuspectedThreats().begin(); it != m_pEntity->GetSuspectedThreats().end(); ++it)
		{
			float squareDistance = 0.0f;
			XMVECTOR vector = XMLoadFloat2(&it->m_lastKnownPosition) - XMLoadFloat2(&m_pEntity->GetPosition());
			XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));
			
			if(squareDistance < shortestSquareDistance)
			{
				pGreatestSuspectedThreat = &(*it);
				shortestSquareDistance = squareDistance;
			}

			if(it->m_hasHitEntity && squareDistance < shortestPrioritySquareDistance)
			{
				pPrioritySuspectedThreat = &(*it);
				shortestPrioritySquareDistance = squareDistance;
			}
		}

		if(pPrioritySuspectedThreat)
		{
			m_pEntity->SetGreatestSuspectedThreat(pPrioritySuspectedThreat);
		}else
		{
			m_pEntity->SetGreatestSuspectedThreat(pGreatestSuspectedThreat);
		}
	}else
	{
		m_pEntity->SetGreatestSuspectedThreat(nullptr);
	}
}

//--------------------------------------------------------------------------------------
// Fires a projectile towards a specified target position.
// Param1: The target position to shoot at.
//--------------------------------------------------------------------------------------
void EntityCombatManager::ShootAt(const XMFLOAT2& target)
{
	ProjectileFiredEventData data(m_pEntity->GetId(), m_pEntity->GetTeam(), m_pEntity->GetPosition(), target);
	m_pEntity->SendEvent(m_pEnvironment, ProjectileFiredEventType, &data);

	//ProjectileFiredMessage projectileFiredMessage(m_pEntity->GetId(), m_pEntity->GetTeam(), m_pEntity->GetPosition(), target);
	//m_pEnvironment->ProcessMessage(&projectileFiredMessage);
	//m_pEnvironment->AddProjectile(m_pEntity->GetId(), m_pEntity->GetTeam(), m_pEntity->GetPosition(), target);
}

