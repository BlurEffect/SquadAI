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
// threats to the entity.
//--------------------------------------------------------------------------------------
void EntityCombatManager::DetermineGreatestKnownThreat(void)
{
	if(!m_pEntity->GetKnownThreats().empty())
	{
		float shortestSquareDistance = std::numeric_limits<float>::max();
		Entity* pGreatestKnownThreat = nullptr;

		for(std::vector<Entity*>::const_iterator it = m_pEntity->GetKnownThreats().begin(); it != m_pEntity->GetKnownThreats().end(); ++it)
		{
			float squareDistance = 0.0f;
			XMVECTOR vector = XMLoadFloat2(&(*it)->GetPosition()) - XMLoadFloat2(&m_pEntity->GetPosition());
			XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));
			
			if(squareDistance < shortestSquareDistance)
			{
				pGreatestKnownThreat = (*it);
				shortestSquareDistance = squareDistance;
			}
		}

		m_pEntity->SetGreatestKnownThreat(pGreatestKnownThreat);
	}else
	{
		m_pEntity->SetGreatestKnownThreat(nullptr);
	}
}

//--------------------------------------------------------------------------------------
// Determines and sets the currently greatest suspected threat based on the distance of the
// threats to the entity.
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

			if(it->m_hasPriority && squareDistance < shortestPrioritySquareDistance)
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
	ProjectileFiredMessage projectileFiredMessage(m_pEntity->GetId(), m_pEntity->GetTeam(), m_pEntity->GetPosition(), target);
	m_pEnvironment->ReceiveMessage(&projectileFiredMessage);
	//m_pEnvironment->AddProjectile(m_pEntity->GetId(), m_pEntity->GetTeam(), m_pEntity->GetPosition(), target);
}

//--------------------------------------------------------------------------------------
// Called when the entity is hit by a hostile projectile.
// Param1: The damage that will be suffered through the projectile.
// Param2: The id of the entity that shot the projectile.
// Param3: Tells whether the entity that fired the projectile is still alive.
// Param4: The position of the entity that shot the projectile.
//--------------------------------------------------------------------------------------
void EntityCombatManager::Hit(float damage, unsigned long id, bool shooterAlive, const XMFLOAT2& position)
{
	m_pEntity->SetCurrentHealth(m_pEntity->GetCurrentHealth() - damage);

	// Only update threat state if the attacker is still alive
	if(shooterAlive)
	{
		// check if already in list of known threats -> do nothing
		std::vector<Entity*>::iterator foundItKnown = std::find_if(m_pEntity->GetKnownThreats().begin(), m_pEntity->GetKnownThreats().end(), Entity::FindEntityById(id));
		if(foundItKnown == m_pEntity->GetKnownThreats().end())
		{
			// The shooter is not an already known threat -> check the suspected threats

			// Check if the shooter is already in the list of suspected threats
			std::vector<SuspectedThreat>::iterator foundIt = std::find_if(m_pEntity->GetSuspectedThreats().begin(), m_pEntity->GetSuspectedThreats().end(), Entity::FindSuspectedThreatById(id));
			if(foundIt == m_pEntity->GetSuspectedThreats().end())
			{
				// This is a new suspected threat -> add it to the list
				m_pEntity->AddSuspectedThreat(id, position, true);
			}else
			{
				// The shooter is already a suspected threat, update the last known position
				foundIt->m_lastKnownPosition = position;
				foundIt->m_hasPriority = true;
			}
		}
	}

	
}

