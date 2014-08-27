/* 
*  Kevin Meergans, SquadAI, 2014
*  Entity.cpp
*  This is the abstract base class for AI controlled entities that are able
*  to move and fight.
*/
	
// Includes
#include "Entity.h"
#include "TestEnvironment.h"
#include "Message.h"
#include "TeamAI.h"
#include "TeamAIToEntityMessages.h"

Entity::Entity(void) : CollidableObject(),
					   m_pBehaviour(nullptr),
					   m_pEnvironment(nullptr),
					   m_team(EntityTeam(None)),
					   m_pTeamAI(nullptr),
					   m_pCurrentOrder(nullptr),
					   m_readyForAttack(false),
					   m_movementTargetSet(false),
					   m_movementTarget(0.0f, 0.0f),
					   m_currentHealth(0.0f),
					   m_maximalHealth(0.0f),
					   m_reportInterval(0.0f),
					   m_reportTimer(0.0f),
					   m_doReport(false)
{
}

Entity::~Entity(void)
{
	if(m_pBehaviour)
	{
		delete m_pBehaviour;
	}
}

//--------------------------------------------------------------------------------------
// Initialises the entity.
// Param1:  A unique identifier for the object.
// Param2:  The start position of the entity.
// Param3:  The initial rotation of the entity.
// Param4:  The uniform scale of the entity.
// Param5:  The category the object belongs to.
// Param6:  The type of the collider that should be created for this entity.
// Param7:  The collider data that should be used for the creation of the collider.
// Param8:  A pointer to the test environment that the entity is part of.
// Param9:  The maximal health for this entity.
// Param10: Identifies the team that this entity belongs to.
// Param11: Determines at what interval the entity will report updates to the associated team AI
// Returns true if the entity was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, TestEnvironment* pEnvironment, float maxHealth, EntityTeam team, float reportInterval)
{
	if(!CollidableObject::Initialise(id, position, rotation, uniformScale, category, colliderType, pColliderData))
	{
		return false;
	}

	// Check for invalid values
	if((team == None) || (!pEnvironment))
	{
		return false;
	}

	if(m_pBehaviour)
	{
		delete m_pBehaviour;
		m_pBehaviour = nullptr;
	}

	if(!m_pBehaviour)
	{
		m_pBehaviour = BehaviourFactory::CreateBehaviourTree(ModifiedSimpleIndividualCombatTree, this);
		if(!m_pBehaviour)
		{
			return false;
		}
	}

	m_pEnvironment = pEnvironment;
	m_team = team;
	m_maximalHealth = maxHealth;
	m_currentHealth = m_maximalHealth;
	m_reportInterval = reportInterval;

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void Entity::Update(float deltaTime)
{
	if(deltaTime == 0.0f)
	{
		// No need to traverse the tree.
		return;
	}

	m_reportTimer += deltaTime;
	if(m_reportTimer >= m_reportInterval)
	{
		m_doReport = true;
		m_reportTimer = 0.0f;
	}else
	{
		m_doReport = false;
	}
	
	m_pBehaviour->Tick(deltaTime);

	
}
	
//--------------------------------------------------------------------------------------
// Activates the entity at the start of the simulation.
//--------------------------------------------------------------------------------------
void Entity::Activate(void)
{
	// Do nothing
}

//--------------------------------------------------------------------------------------
// Resets the entity
//--------------------------------------------------------------------------------------
void Entity::Reset(void)
{
	m_knownThreats.clear();
	m_suspectedThreats.clear();
	m_pGreatestKnownThreat     = nullptr;
	m_pGreatestSuspectedThreat = nullptr;
	m_readyForAttack		   = false;
	m_movementTargetSet		   = false;
	m_movementTarget		   = XMFLOAT2(0.0f, 0.0f);
	m_attackTargetSet          = false;
	m_attackTarget		       = XMFLOAT2(0.0f, 0.0f);
	m_observationTargetSet     = false;
	m_observationTarget		   = XMFLOAT2(0.0f, 0.0f);
	m_currentHealth			   = m_maximalHealth;

	m_pCurrentOrder = nullptr;

	ResetCommunication();

	m_pBehaviour->Reset();
}
/*
//--------------------------------------------------------------------------------------
// Receives and processes a given message sent by the test environment.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void Entity::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case HitMessageType:
		{
			// Make sure the entity is actually alive (cannot be hit when dead but in the rare case that the
			// entity is hit by two projectiles during the same frame this if-statement will prevent all hits
			// after the fatal one to be processed)
			if(IsAlive())
			{
				HitMessage* pHitMessage = reinterpret_cast<HitMessage*>(pMessage);
				ProcessHit(pHitMessage->GetDamage(), pHitMessage->GetShooterId(), pHitMessage->IsShooterAlive(), pHitMessage->GetPosition());
			}
			break;
		}
	case EntityKilledMessageType:
		{
			EntityKilledMessage* pEntityKilledMessage = reinterpret_cast<EntityKilledMessage*>(pMessage);
			if(pEntityKilledMessage->GetTeam() != GetTeam())
			{
				ProcessEnemyKilled(pEntityKilledMessage->GetTeam(), pEntityKilledMessage->GetId());
			}
			break;
		}
	case ReadyToRespawnMessageType:
		{
			ReadyToRespawnMessage* pReadyToRespawnMessage = reinterpret_cast<ReadyToRespawnMessage*>(pMessage);
			ProcessRespawn(pReadyToRespawnMessage->GetRespawnPosition());
			break;
		}
	}
}
*/
/*
//--------------------------------------------------------------------------------------
// Respawns the entity at a given position.
// Param1: The position of the respawn point, where the entity will reenter the game.
//--------------------------------------------------------------------------------------
void Entity::Respawn(const XMFLOAT2& respawnPosition)
{
	Reset();
	SetPosition(respawnPosition);
	UpdateColliderPosition(respawnPosition);
	SetRotation(static_cast<float>(rand() % 360));
}
*/

//--------------------------------------------------------------------------------------
// Adds a new threat to the entity's list of known threats.
// Param1: A pointer to the hostile entity that has become a known threat.
// Param2: Tells whether the hostile enemy has successfully attacked the entity.
//--------------------------------------------------------------------------------------
void Entity::AddKnownThreat(Entity* pThreat, bool hasHitEntity)
{
	if(pThreat)
	{
		m_knownThreats.push_back(KnownThreat(pThreat, hasHitEntity));
	}
}

//--------------------------------------------------------------------------------------
// Adds a new threat to the entity's list of known threats.
// Param1: The id of the entity that is no longer a known threat to this entity.
//--------------------------------------------------------------------------------------
void Entity::RemoveKnownThreat(unsigned long id)
{
	std::vector<KnownThreat>::iterator foundIt = std::find_if(m_knownThreats.begin(), m_knownThreats.end(), Entity::FindKnownThreatById(id));
		
	if(foundIt != m_knownThreats.end())
	{
		m_knownThreats.erase(foundIt);
	}
}

//--------------------------------------------------------------------------------------
// Removes all known threats.
//--------------------------------------------------------------------------------------
void Entity::ClearKnownThreats(void)
{
	m_knownThreats.clear();
}

//--------------------------------------------------------------------------------------
// Tells whether the entity associated to the passed in ID is a known threat to this entity.
// Param1: The id of the entity that should be checked whether it is a known threat.
// Returns true if the entity associated to the passed in ID is a known threat to this entity,
// false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::IsKnownThreat(unsigned long id)
{
	std::vector<KnownThreat>::iterator foundIt = std::find_if(m_knownThreats.begin(), m_knownThreats.end(), Entity::FindKnownThreatById(id));
		
	return (foundIt != m_knownThreats.end());
}

//--------------------------------------------------------------------------------------
// Returns the known threat associated to the hostile entity with the given id.
// Param1: The id of the entity, whose associated known threat object should be returned.
// Returns a pointer to the known threat, null if the entity doesn't have a known
// threat associated to the provided id.
//--------------------------------------------------------------------------------------
KnownThreat* Entity::GetKnownThreat(unsigned long id)
{
	std::vector<KnownThreat>::iterator foundIt = std::find_if(m_knownThreats.begin(), m_knownThreats.end(), Entity::FindKnownThreatById(id));

	if(foundIt != m_knownThreats.end())
	{
		return &(*foundIt);
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------
// Adds a new threat to the entity's list of known threats.
// Param1: The id of the entity that is no longer a known threat to this entity.
// Param2: The last known position of the threat.
// Param3: Tells whether this suspected threat has special priority.
//--------------------------------------------------------------------------------------
void Entity::AddSuspectedThreat(unsigned long id, const XMFLOAT2& lastKnownPosition, bool hasPriority)
{
	m_suspectedThreats.push_back(SuspectedThreat(id, lastKnownPosition, hasPriority));
}

//--------------------------------------------------------------------------------------
// Adds a new threat to the entity's list of known threats.
// Param1: The id of the entity that is no longer a known threat to this entity.
//--------------------------------------------------------------------------------------
void Entity::RemoveSuspectedThreat(unsigned long id)
{
	std::vector<SuspectedThreat>::iterator foundIt = std::find_if(m_suspectedThreats.begin(), m_suspectedThreats.end(), Entity::FindSuspectedThreatById(id));
		
	if(foundIt != m_suspectedThreats.end())
	{
		m_suspectedThreats.erase(foundIt);
	}
}

//--------------------------------------------------------------------------------------
// Removes all suspected threats.
//--------------------------------------------------------------------------------------
void Entity::ClearSuspectedThreats(void)
{
	m_suspectedThreats.clear();
}

//--------------------------------------------------------------------------------------
// Tells whether the entity associated to the passed in ID is a suspected threat to this entity.
// Param1: The id of the entity that should be checked whether it is a suspected threat.
// Returns true if the entity associated to the passed in ID is a suspected threat to this entity,
// false otherwise.
//--------------------------------------------------------------------------------------
bool Entity::IsSuspectedThreat(unsigned long id)
{
	std::vector<SuspectedThreat>::iterator foundIt = std::find_if(m_suspectedThreats.begin(), m_suspectedThreats.end(), Entity::FindSuspectedThreatById(id));
		
	return (foundIt != m_suspectedThreats.end());
}

//--------------------------------------------------------------------------------------
// Returns the suspected threat associated to the hostile entity with the given id.
// Param1: The id of the entity, whose associated suspected threat object should be returned.
// Returns a pointer to the suspected threat, null if the entity doesn't have a suspected
// threat associated to the provided id.
//--------------------------------------------------------------------------------------
SuspectedThreat* Entity::GetSuspectedThreat(unsigned long id)
{
	std::vector<SuspectedThreat>::iterator foundIt = std::find_if(m_suspectedThreats.begin(), m_suspectedThreats.end(), Entity::FindSuspectedThreatById(id));

	if(foundIt != m_suspectedThreats.end())
	{
		return &(*foundIt);
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------
// Tells whether the entity is currently moving towards the greatest suspected threat in
// order to investigate it or whether there now is a greater suspected threat that the
// entity should tend to.
// Returns true if the suspected threat being investigated is still the greatest suspected threat,
// false otherwise and also if there is no suspected threat at all.
//--------------------------------------------------------------------------------------
bool Entity::IsInvestigatingGreatestSuspectedThreat(void)
{
	if(!GetGreatestSuspectedThreat())
	{
		return false;
	}

	if(GetGreatestSuspectedThreat()->m_lastKnownPosition.x == GetMovementTarget().x &&
	   GetGreatestSuspectedThreat()->m_lastKnownPosition.y == GetMovementTarget().y)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Tells whether the entity is currently moving towards the highest priority movement target 
// or whether there now is a higher priority target that the entity should move towards instead.
// Returns true if the target being moved towards is still the highest priority target,
// false otherwise and also if there is no movement target set at all.
//--------------------------------------------------------------------------------------
bool Entity::IsMovingToHighestPriorityTarget(void)
{
	if(!IsMovementTargetSet())
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Tells whether an entity is at a certain target or not
// Param1: The target position, for which to check if the entity has reached it.
// Returns true if the entity has reached the target, false otherwise. 
//--------------------------------------------------------------------------------------
bool Entity::IsAtTarget(const XMFLOAT2& target)
{
	float distance(0.0f);
	XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&target) - XMLoadFloat2(&GetPosition())));

	return (distance == 0.0f);
}

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void Entity::ProcessEvent(EventType type, void* pEventData)
{
	switch(type)
	{
	case EntityHitEventType:
		{
		// Make sure the entity is actually alive (cannot be hit when dead but in the rare case that the
		// entity is hit by two projectiles during the same frame this if-statement will prevent all hits
		// after the fatal one to be processed)
		if(IsAlive())
		{
			EntityHitEventData* pHitData = reinterpret_cast<EntityHitEventData*>(pEventData);
			ProcessHit(pHitData->m_damage, pHitData->m_id, pHitData->m_shooterAlive, pHitData->m_position);
		}
		break;
		}
	
	case RespawnEventType:
		{
		RespawnEventData* pRespawnData = reinterpret_cast<RespawnEventData*>(pEventData);
		ProcessRespawn(pRespawnData->m_respawnPosition);
		break;
		}
	default:
		Communicator::ProcessEvent(type, pEventData);
	}
}

//--------------------------------------------------------------------------------------
// Processes a given message.
// Param1: A pointer to the message to process.
// Returns true if this was the final communicator to process the message, false if the
// message was forwarded to another one.
//--------------------------------------------------------------------------------------
bool Entity::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case EntityKilledMessageType:
		{
		EntityKilledMessage* pEntityKilledMessage = reinterpret_cast<EntityKilledMessage*>(pMessage);
		if(pEntityKilledMessage->GetData().m_team != GetTeam())
		{
			ProcessEnemyKilled(pEntityKilledMessage->GetData().m_team, pEntityKilledMessage->GetData().m_id);
		}
		return true;
		break;
		}
	case FollowOrderMessageType:
		{
		FollowOrderMessage* pFollowOrderMessage = reinterpret_cast<FollowOrderMessage*>(pMessage);
		// Orders are processed separately
		ProcessOrder(pFollowOrderMessage->GetData().m_pOrder);
		return true;
		break;
		}
	case CancelOrderMessageType:
		{
		// Cancel the currently active order
		m_pCurrentOrder = nullptr;
		return true;
		break;
		}
	default:
		return Communicator::ProcessMessage(pMessage);
	}
}

//--------------------------------------------------------------------------------------
// Processes a given order.
// Param1: A pointer to the order to process.
//--------------------------------------------------------------------------------------
void Entity::ProcessOrder(Order* pOrder)
{
	m_pCurrentOrder = pOrder;

	switch(pOrder->GetOrderType())
	{
	case AttackEnemyOrder:
		ProcessAttackOrder(reinterpret_cast<AttackOrder*>(pOrder));
		break;
	case MoveToPositionOrder:
		ProcessMoveOrder(reinterpret_cast<MoveOrder*>(pOrder));
		break;
	case DefendPositionOrder:
		ProcessDefendOrder(reinterpret_cast<DefendOrder*>(pOrder));
		break;
	default:
		;
		// Notify team AI that order couldn't be processed?
	}
}

//--------------------------------------------------------------------------------------
// Processes a given attack order.
// Param1: A pointer to the attack order to process.
//--------------------------------------------------------------------------------------
void Entity::ProcessAttackOrder(AttackOrder* pOrder)
{
	switch(pOrder->GetOrderPriority())
	{
	case LowPriority:
		break;
	case MediumPriority:
		break;
	case HighPriority:
		break;
	}
}

//--------------------------------------------------------------------------------------
// Processes a given move order.
// Param1: A pointer to the move order to process.
//--------------------------------------------------------------------------------------
void Entity::ProcessMoveOrder(MoveOrder* pOrder)
{
	switch(pOrder->GetOrderPriority())
	{
	case LowPriority:
		break;
	case MediumPriority:
		break;
	case HighPriority:
		break;
	}
}

//--------------------------------------------------------------------------------------
// Processes a given defend order.
// Param1: A pointer to the defend order to process.
//--------------------------------------------------------------------------------------
void Entity::ProcessDefendOrder(DefendOrder* pOrder)
{
	switch(pOrder->GetOrderPriority())
	{
	case LowPriority:
		break;
	case MediumPriority:
		break;
	case HighPriority:
		break;
	}
}

// The following functions provide default implementations for the different events and
// messages that the entity can receive from the test environment.

//--------------------------------------------------------------------------------------
// Processes a hit the entity suffered from a projectile. Default implementation.
// Param1: The damage that the projectile will deal.
// Param2: The id of the shooting entity.
// Param3: Tells whether the entity that shot the projectile is still alive or dead.
// Param4: The position, from which the projectile was originally shot.
//--------------------------------------------------------------------------------------
void Entity::ProcessHit(float damage, unsigned long id, bool shooterAlive, const XMFLOAT2& position)
{
	// Update the health of the entity.
	SetCurrentHealth(GetCurrentHealth() - damage);

	// Notify team AI
	AttackedByEnemyMessageData data(GetId(), id, position);
	SendMessage(GetTeamAI(), AttackedByEnemyMessageType, &data);

	// Only update threat state if the attacker is still alive.
	if(shooterAlive)
	{
		// Check if the attacker is already in the list of known threats. If so, do nothing.
		std::vector<KnownThreat>::iterator foundItKnown = std::find_if(GetKnownThreats().begin(), GetKnownThreats().end(), Entity::FindKnownThreatById(id));
		if(foundItKnown == GetKnownThreats().end())
		{
			// The shooter is not an already known threat -> check the suspected threats.

			// Check if the shooter is already in the list of suspected threats
			std::vector<SuspectedThreat>::iterator foundIt = std::find_if(GetSuspectedThreats().begin(), GetSuspectedThreats().end(), Entity::FindSuspectedThreatById(id));
			if(foundIt == GetSuspectedThreats().end())
			{
				// The attacker is not in the list of suspected threats -> Add him to the list and set the priority
				// flag to ensure the entity checks this one out first as soon as all known threats are gone.
				AddSuspectedThreat(id, position, true);
			}else
			{
				// The shooter is already a suspected threat, update the last known position and set the 
				// priority flag.
				foundIt->m_lastKnownPosition = position;
				foundIt->m_hasHitEntity = true;
			}
		}else
		{
			// Mark the known threat as having successfully hit the entity.
			foundItKnown->m_hasHitEntity = true;
		}
	}

	if(!IsAlive())
	{
		// The entity just died, send an event to the test environment
		EntityDiedEventData data(GetTeam(), GetId());
		SendEvent(GetTestEnvironment(),EntityDiedEventType, &data);
	}
}

//--------------------------------------------------------------------------------------
// Processes the death of a hostile entity. Default implementation.
// Param1: The team that the killed entity belongs to.
// Param2: The id of the entity that was killed.
//--------------------------------------------------------------------------------------
void Entity::ProcessEnemyKilled(EntityTeam team, unsigned long id)
{
	// Remove the killed enemy from the threat lists

	if(GetGreatestKnownThreat() && GetGreatestKnownThreat()->m_pEntity->GetId() == id)
	{
		SetGreatestKnownThreat(nullptr);
	}else if(GetGreatestSuspectedThreat() && GetGreatestSuspectedThreat()->m_enemyId == id)
	{
		SetGreatestSuspectedThreat(nullptr);
	}

	RemoveKnownThreat(id);
	RemoveSuspectedThreat(id);
}

//--------------------------------------------------------------------------------------
// Processes the respawn message and will reset the entity to reenter the game. Default
// implementation.
// Param1: The position, where the entity should respawn.
//--------------------------------------------------------------------------------------
void Entity::ProcessRespawn(const XMFLOAT2& respawnPosition)
{
	m_knownThreats.clear();
	m_suspectedThreats.clear();
	m_pGreatestKnownThreat     = nullptr;
	m_pGreatestSuspectedThreat = nullptr;
	m_readyForAttack		   = false;
	m_movementTargetSet		   = false;
	m_movementTarget		   = XMFLOAT2(0.0f, 0.0f);
	m_attackTargetSet          = false;
	m_attackTarget		       = XMFLOAT2(0.0f, 0.0f);
	m_observationTargetSet     = false;
	m_observationTarget		   = XMFLOAT2(0.0f, 0.0f);
	m_currentHealth			   = m_maximalHealth;

	SetPosition(respawnPosition);
	UpdateColliderPosition(respawnPosition);
	SetRotation(static_cast<float>(rand() % 360));
}

// Data access functions

Behaviour* Entity::GetBehaviour(void)
{
	return m_pBehaviour;
}

TestEnvironment* Entity::GetTestEnvironment(void)
{
	return m_pEnvironment;
}

EntityTeam Entity::GetTeam(void) const
{
	return m_team;
}

TeamAI* Entity::GetTeamAI(void)
{
	return m_pTeamAI;
}

bool Entity::IsAlive(void) const
{
	return m_currentHealth > 0.0f;
}

std::vector<KnownThreat>& Entity::GetKnownThreats(void)
{
	return m_knownThreats;
}

std::vector<SuspectedThreat>& Entity::GetSuspectedThreats(void)
{
	return m_suspectedThreats;
}

const KnownThreat* Entity::GetGreatestKnownThreat(void) const
{
	return m_pGreatestKnownThreat;
}

const SuspectedThreat* Entity::GetGreatestSuspectedThreat(void) const
{
	return m_pGreatestSuspectedThreat;
}

bool Entity::IsReadyForAttack(void) const
{
	return m_readyForAttack;
}

bool Entity::IsAttackTargetSet(void) const
{
	return m_attackTargetSet;
}

const XMFLOAT2& Entity::GetAttackTarget(void) const
{
	return m_attackTarget;
}

bool Entity::IsMovementTargetSet(void) const
{
	return m_movementTargetSet;
}

const XMFLOAT2& Entity::GetMovementTarget(void) const
{
	return m_movementTarget;
}

float Entity::GetCurrentHealth(void) const
{
	return m_currentHealth;
}

float Entity::GetMaximalHealth(void) const
{
	return m_maximalHealth;
}

Order* Entity::GetCurrentOrder(void)
{
	return m_pCurrentOrder;
}

bool Entity::IsObservationTargetSet(void) const
{
	return m_observationTargetSet;
}

const XMFLOAT2& Entity::GetObservationTarget(void) const
{
	return m_observationTarget;
}

bool Entity::IsPathSet(void) const
{
	return m_pPath != nullptr;
}

std::vector<XMFLOAT2>* Entity::GetPath(void)
{
	return m_pPath;
}

float Entity::GetReportInterval(void) const
{
	return m_reportInterval;
}

bool Entity::DoUpdate(void) const
{
	return m_doReport;
}

void Entity::SetTestEnvironment(TestEnvironment* pEnvironment)
{
	if(pEnvironment)
	{
		m_pEnvironment = pEnvironment;
	}
}

void Entity::SetTeam(EntityTeam team)
{
	m_team = team;
}

void Entity::SetTeamAI(TeamAI* pTeamAI)
{
	m_pTeamAI = pTeamAI;
}

void Entity::SetGreatestKnownThreat(KnownThreat* pThreat)
{
	m_pGreatestKnownThreat = pThreat;
}

void Entity::SetGreatestSuspectedThreat(SuspectedThreat* pThreat)
{
	m_pGreatestSuspectedThreat = pThreat;
}

void Entity::SetReadyForAttack(bool readyForAttack)
{
	m_readyForAttack = readyForAttack;
}

void Entity::SetAttackTargetSet(bool targetSet)
{
	m_attackTargetSet = targetSet;
}

void Entity::SetAttackTarget(const XMFLOAT2& target)
{
	m_attackTarget = target;
}

void Entity::SetMovementTargetSet(bool targetSet)
{
	m_movementTargetSet = targetSet;
}

void Entity::SetMovementTarget(const XMFLOAT2& target)
{
	m_movementTarget = target;
}

void Entity::SetCurrentHealth(float health)
{
	m_currentHealth = health;
}

void Entity::SetMaximalHealth(float maxHealth)
{
	m_maximalHealth = maxHealth;
}

void Entity::SetObservationTargetSet(bool targetSet)
{
	m_observationTargetSet = targetSet;
}

void Entity::SetObservationTarget(const XMFLOAT2& target)
{
	m_observationTarget = target;
}

void Entity::SetPath(std::vector<XMFLOAT2>* pPath)
{
	m_pPath = pPath;
}

void Entity::SetReportInterval(float reportInterval)
{
	m_reportInterval = reportInterval;
}
