/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.cpp
*  Represents a fighting entity in the test environment.
*/

#define DEBUG

// Includes
#include "Soldier.h"
#include "TestEnvironment.h"
#include "MessageDataStructures.h"

Soldier::Soldier(void) : Entity(),
						 m_fireWeaponTimer(0.0f)
{
}

Soldier::~Soldier(void)
{
}

//--------------------------------------------------------------------------------------
// Initialises the soldier.
// Param1:  An ID uniquely identifying this entity.
// Param2:  The start position of the entity.
// Param3:  The initial rotation of the entity.
// Param4:  The uniform scale of the entity.
// Param5:  The category the object belongs to.
// Param6:  The type of the collider that should be created for this entity.
// Param7:  The collider data that should be used for the creation of the collider.
// Param8:  A pointer to the test environment the soldier is part of.
// Param9:  Identifies the team that this entity belongs to.
// Param10: The bundled soldier-specific properties.
// Returns true if the soldier was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Soldier::Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, TestEnvironment* pEnvironment, EntityTeam team, const SoldierProperties& soldierProperties)
{
	if(!Entity::Initialise(id, position, rotation, uniformScale, category, colliderType, pColliderData, pEnvironment, soldierProperties.m_maxHealth, team))
	{
		return false;
	}

	m_soldierProperties  = soldierProperties;

	Reset();      

	return (m_movementManager.Initialise(this, GetTestEnvironment()) && m_combatManager.Initialise(this, GetTestEnvironment()) && m_sensors.Initialise(this, GetTestEnvironment()));
}

// Debug ->remove
float g_time = 0.0f;

//--------------------------------------------------------------------------------------
// Updates the soldier entity.
// Param1: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void Soldier::Update(float deltaTime)
{
	if(deltaTime == 0.0f)
	{
		return;
	}

	GetBehaviour()->Tick(deltaTime);

	/*
	//m_movementManager.FollowPath(GetTargetReachedRadius(), m_soldierProperties.m_maxSpeed);
	m_movementManager.AvoidCollisions(m_soldierProperties.m_maxCollisionSeeAhead, 100.0f);//m_soldierProperties.m_maxCollisionAvoidanceForce);
	//m_movementManager.Separate(m_soldierProperties.m_separationRadius, m_soldierProperties.m_maxSeparationForce);
	//m_movementManager.StayAwayFromWalls(m_soldierProperties.m_avoidWallsRadius, m_soldierProperties.m_maxAvoidWallsForce);

	if(GetTeam() == TeamRed)
	{
		//m_sensors.CheckForThreats();
		m_movementManager.Seek(XMFLOAT2(24.0f, 24.0f), 1.0f, GetMaxSpeed());
	}else
	{
		m_movementManager.Seek(XMFLOAT2(-24.0f, -24.0f), 1.0f, GetMaxSpeed());
	}
	
	g_time += deltaTime;

	if((g_time >= 2.0f) && GetTeam() == TeamRed)
	{
		XMFLOAT2 target;
		target.x = GetPosition().x + 1.0f;
		target.y = GetPosition().y;

		m_combatManager.ShootAt(target);
		g_time = 0.0f;
	}

	m_movementManager.UpdatePosition(deltaTime, m_soldierProperties.m_maxSpeed, m_soldierProperties.m_maxTotalForce);
	*/
}

//--------------------------------------------------------------------------------------
// Moves the soldier entity towards a given target.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::MoveToTarget(float deltaTime)
{
	if(!m_movementManager.IsPathSet())
	{
		// There is no path set, create a new one
		if(!m_movementManager.SetPathTo(GetMovementTarget()))
		{
			// No valid path exists to the target position
			return StatusFailure;
		}
	}

	if(m_movementManager.FollowPath(m_soldierProperties.m_targetReachedRadius, m_soldierProperties.m_maxSpeed))
	{
		// The target was reached
		if(GetGreatestSuspectedThreat() && GetGreatestSuspectedThreat()->m_lastKnownPosition.x == GetMovementTarget().x &&
			GetGreatestSuspectedThreat()->m_lastKnownPosition.y == GetMovementTarget().y)
		{
			int a = 4;
		}

		return StatusSuccess;
	}

	// Target not yet reached -> move the entity along the path avoiding obstacles and intersection with other soldiers
	
	// Note: Collision avoidance not really needed as pathfinding sufficient and avoidance of other entities not really
	//       important at the moment.
	// m_movementManager.AvoidCollisions(m_soldierProperties.m_maxCollisionSeeAhead, m_soldierProperties.m_maxCollisionAvoidanceForce);
	m_movementManager.Separate(m_soldierProperties.m_separationRadius, m_soldierProperties.m_maxSeparationForce);
	m_movementManager.StayAwayFromWalls(m_soldierProperties.m_avoidWallsRadius, m_soldierProperties.m_maxAvoidWallsForce);

	m_movementManager.UpdatePosition(deltaTime, m_soldierProperties.m_maxSpeed, m_soldierProperties.m_maxTotalForce);

	return StatusRunning;
}

//--------------------------------------------------------------------------------------
// Makes the soldier attack a target at the given position.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::Attack(float deltaTime)
{
	m_combatManager.ShootAt(GetAttackTarget());
	// Wait until the weapon is ready again for the next shot
	SetReadyForAttack(false);
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Makes the soldier aim at a given position.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::AimAt(float deltaTime)
{
	m_movementManager.LookAt(GetAttackTarget());
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Determines the attack target for the soldier based on current threats and other 
// factors.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::DetermineAttackTargetPosition(float deltaTime)
{
	if(GetGreatestKnownThreat())
	{
		SetAttackTarget(GetGreatestKnownThreat()->GetPosition());
		SetAttackTargetSet(true);
	}else
	{
		SetAttackTargetSet(false);
	}

	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Makes the soldier wait at the position he is currently at.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::Idle(float deltaTime)
{
	// Do nothing
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Determines and sets a patrol target position for the soldier.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::DeterminePatrolTarget(float deltaTime)
{
	// Get a new random target position within the test environment
	XMFLOAT2 patrolTarget(0.0f, 0.0f);
	if(GetTestEnvironment()->GetRandomUnblockedTarget(patrolTarget))
	{
		m_movementManager.Reset();

		SetMovementTarget(patrolTarget);
		SetMovementTargetSet(true);
	}else
	{
		SetMovementTargetSet(false);
	}

	// Always succeeds
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Determines and sets a target position for the soldier to move to in order to approach threats.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::DetermineApproachThreatTarget(float deltaTime)
{
	// Approach the greatest suspected threat

	if(GetGreatestSuspectedThreat())
	{
		m_movementManager.Reset();

		SetMovementTarget(GetGreatestSuspectedThreat()->m_lastKnownPosition);
		SetMovementTargetSet(true);
	}else
	{
		SetMovementTargetSet(false);
	}

	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Updates the known and suspected threats for the soldier.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::UpdateThreats(float deltaTime)
{
	m_sensors.CheckForThreats(GetViewDirection(), m_soldierProperties.m_viewingDistance, m_soldierProperties.m_fieldOfView);
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Determines and sets the currently greatest known threat to the Soldier.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::DetermineGreatestKnownThreat(float deltaTime)
{
	m_combatManager.DetermineGreatestKnownThreat();
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Determines and sets the currently greatest suspected threat to the Soldier.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::DetermineGreatestSuspectedThreat(float deltaTime)
{
	m_combatManager.DetermineGreatestSuspectedThreat();
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Updates the attack readiness of the soldier.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::UpdateAttackReadiness(float deltaTime)
{
	if(!IsReadyForAttack())
	{
		m_fireWeaponTimer += deltaTime;

		if(m_fireWeaponTimer >= m_soldierProperties.m_fireWeaponInterval)
		{
			SetReadyForAttack(true);
			m_fireWeaponTimer = 0.0f;
		}
	}

	return StatusSuccess;
}


//--------------------------------------------------------------------------------------
// Receives and processes a given message.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void Soldier::ReceiveMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case HitMessageType:
		if(IsAlive())
		{
			HitMessage* pHitMessage = reinterpret_cast<HitMessage*>(pMessage);
			m_combatManager.Hit(pHitMessage->GetDamage(), pHitMessage->GetShooterId(), pHitMessage->IsShooterAlive(), pHitMessage->GetPosition());

			if(!IsAlive())
			{
				// The entity just died, send a message to the test environment
				EntityKilledMessage entityKilledMessage(GetTeam(), GetId());
				GetTestEnvironment()->ReceiveMessage(&entityKilledMessage);
			}
		}
		break;
	case EntityKilledMessageType:
		EntityKilledMessage* pEntityKilledMessage = reinterpret_cast<EntityKilledMessage*>(pMessage);
		if(pEntityKilledMessage->GetTeam() != GetTeam())
		{
			if(GetGreatestKnownThreat() && GetGreatestKnownThreat()->GetId() == pEntityKilledMessage->GetId())
			{
				SetGreatestKnownThreat(nullptr);
			}else if(GetGreatestSuspectedThreat() && GetGreatestSuspectedThreat()->m_enemyId == pEntityKilledMessage->GetId())
			{
				SetGreatestSuspectedThreat(nullptr);
			}

			RemoveKnownThreat(pEntityKilledMessage->GetId());
			RemoveSuspectedThreat(pEntityKilledMessage->GetId());
		}
		break;
	}
}



//--------------------------------------------------------------------------------------
// Process all messages that the entity received since the last processing.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::ProcessMessages(float deltaTime)
{
	// Used to prevent the death notice from being sent multiple times if the
	// entity was hit by several projectiles during the same frame.
	bool notifiedDeath = false;

	//Soldier* pSoldier = this;

	while(!GetActiveMessages().empty())
	{
		switch(GetActiveMessages().front()->GetType())
		{
		case HitMessageType:
			{
			HitMessage* pMsg = reinterpret_cast<HitMessage*>(GetActiveMessages().front());
			//m_combatManager.Hit(pMsg->GetDamage(), pMsg->GetShooterId(), pMsg->GetPosition());

#ifdef DEBUG
			unsigned long shooterId = pMsg->GetShooterId();
			GetTestEnvironment()->RecordEvent(EntityHitEvent, this, &shooterId);
#endif

			if(!IsAlive() && !notifiedDeath)
			{
				//GetTestEnvironment()->AddDeadEntity(this);
				notifiedDeath = true;
#ifdef DEBUG
			unsigned long shooterId = pMsg->GetShooterId();
			GetTestEnvironment()->RecordEvent(EntityKilledEvent, this, &shooterId);
#endif
			}
			break;
			}
		case EntityKilledMessageType:
			{
			EntityKilledMessage* pMsg = reinterpret_cast<EntityKilledMessage*>(GetActiveMessages().front());
			if(pMsg->GetTeam() != GetTeam())
			{
				if(GetGreatestKnownThreat() && GetGreatestKnownThreat()->GetId() == pMsg->GetId())
				{
					SetGreatestKnownThreat(nullptr);
				}else if(GetGreatestSuspectedThreat() && GetGreatestSuspectedThreat()->m_enemyId == pMsg->GetId())
				{
					SetGreatestSuspectedThreat(nullptr);
				}

				RemoveKnownThreat(pMsg->GetId());
				RemoveSuspectedThreat(pMsg->GetId());
			}
			break;
			}
		}
		
		delete GetActiveMessages().front();
		GetActiveMessages().pop();
	}

	return StatusSuccess;
}



//--------------------------------------------------------------------------------------
// Removes the currently active suspected threat from the list of suspected threats.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::ResolveSuspectedThreat(float deltaTime)
{
	if(GetGreatestSuspectedThreat())
	{
		RemoveSuspectedThreat(GetGreatestSuspectedThreat()->m_enemyId);
	}
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Activates the soldier entity.
//--------------------------------------------------------------------------------------
void Soldier::Activate(void)
{
	//m_movementManager.SetPathTo(XMFLOAT2(20.0f, 20.0f));

	if(GetTeam() == TeamRed)
	{
		//m_movementManager.SetPathTo(XMFLOAT2(20.0f, 20.0f));
	}else
	{
		//m_movementManager.SetPathTo(XMFLOAT2(-20.0f, -20.0f));
	}
}

//--------------------------------------------------------------------------------------
// Resets the soldier entity.
//--------------------------------------------------------------------------------------
void Soldier::Reset(void)
{
	m_movementManager.Reset();
	m_combatManager.Reset();

	

	Entity::Reset();
}

// Data access functions

// Movement related

const XMFLOAT2& Soldier::GetVelocity(void) const
{
	return m_movementManager.GetVelocity();
}

float Soldier::GetMaxSpeed(void) const
{
	return m_soldierProperties.m_maxSpeed;
}

float Soldier::GetMaxTotalForce(void) const
{
	return m_soldierProperties.m_maxTotalForce;
}

float Soldier::GetMaxCollisionSeeAhead(void) const
{
	return m_soldierProperties.m_maxCollisionSeeAhead;
}

float Soldier::GetMaxCollisionAvoidanceForce(void) const
{
	return m_soldierProperties.m_maxCollisionAvoidanceForce;
}

float Soldier::GetMaxAvoidWallsForce(void) const
{
	return m_soldierProperties.m_maxAvoidWallsForce;
}

float Soldier::GetMaxSeparationForce(void) const
{
	return m_soldierProperties.m_maxSeparationForce;
}

float Soldier::GetTargetReachedRadius(void) const
{
	return m_soldierProperties.m_targetReachedRadius;
}

float Soldier::GetAvoidWallsRadius(void) const
{
	return m_soldierProperties.m_maxAvoidWallsForce;
}

float Soldier::GetSeparationRadius(void) const
{
	return m_soldierProperties.m_separationRadius;
}

float Soldier::GetFireWeaponInterval(void) const
{
	return m_soldierProperties.m_fireWeaponInterval;
}

const XMFLOAT2& Soldier::GetViewDirection(void) const
{
	return m_movementManager.GetViewDirection();
}

float Soldier::GetFieldOfView(void) const
{
	return m_soldierProperties.m_fieldOfView;
}

float Soldier::GetViewingDistance(void) const
{
	return m_soldierProperties.m_viewingDistance;
}

void Soldier::SetMaxSpeed(float maxSpeed)
{
	m_soldierProperties.m_maxSpeed = maxSpeed;
}

void Soldier::SetMaxTotalForce(float maxForce)
{
	m_soldierProperties.m_maxTotalForce = maxForce;
}

void Soldier::SetMaxCollisionSeeAhead(float maxSeeAhead)
{
	m_soldierProperties.m_maxCollisionSeeAhead = maxSeeAhead;
}

void Soldier::SetMaxCollisionAvoidanceForce(float maxCollisionAvoidanceForce)
{
	m_soldierProperties.m_maxCollisionAvoidanceForce = maxCollisionAvoidanceForce;
}

void Soldier::SetMaxAvoidWallsForce(float maxAvoidWallsForce)
{
	m_soldierProperties.m_maxAvoidWallsForce = maxAvoidWallsForce;
}

void Soldier::SetMaxSeparationForce(float maxSeparationForce)
{
	m_soldierProperties.m_maxSeparationForce = maxSeparationForce;
}

void Soldier::SetTargetReachedRadius(float targetReachedRadius)
{
	m_soldierProperties.m_targetReachedRadius = targetReachedRadius;
}

void Soldier::SetAvoidWallsRadius(float avoidWallsRadius)
{
	m_soldierProperties.m_avoidWallsRadius = avoidWallsRadius;
}

void Soldier::SetSeparationRadius(float separationRadius)
{
	m_soldierProperties.m_separationRadius = separationRadius;
}

void Soldier::SetFireWeaponInterval(float fireWeaponInterval)
{
	m_soldierProperties.m_fireWeaponInterval = fireWeaponInterval;
}

void Soldier::SetFieldOfView(float fieldOfView)
{
	m_soldierProperties.m_fieldOfView = fieldOfView;
}

void Soldier::SetViewingDistance(float viewingDistance)
{
	m_soldierProperties.m_viewingDistance = viewingDistance;
}
