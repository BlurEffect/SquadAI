/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.cpp
*  Represents a fighting entity in the test environment.
*/

#define DEBUG

// Includes
#include "Soldier.h"
#include "TestEnvironment.h"
#include "Message.h"

Soldier::Soldier(void) : Entity(),
						 m_fireWeaponTimer(0.0f),
						 m_changeObservationTargetTimer(0.0f),
						 m_resumePathNode(0)
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

//--------------------------------------------------------------------------------------
// Moves the soldier entity towards a given target.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::MoveToTarget(float deltaTime)
{
	/*
	
	if(!m_movementManager.IsPathSet())
	{
		// There is no path set, create a new one
		if(!m_movementManager.SetPathTo(GetMovementTarget()))
		{
			// No valid path exists to the target position
			return StatusFailure;
		}
	}
	*/
	if(m_movementManager.FollowPath(GetPath(), m_soldierProperties.m_targetReachedRadius, m_soldierProperties.m_maxSpeed))
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

	if(GetCurrentOrder() && GetCurrentOrder()->GetOrderType() == MoveToPositionOrder && GetCurrentOrder()->GetOrderPriority() != LowPriority)
	{
		// Remember the current path node in case the soldier gets interrupted by enemies. Soldier will be able to resume movement afterwards.
		// Does only work though if soldier doesn't move away (only fighting known threats)
		SetResumePathNode(m_movementManager.GetCurrentNode());
	}

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
		SetAttackTarget(GetGreatestKnownThreat()->m_pEntity->GetPosition());
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
BehaviourStatus Soldier::DetermineMovementTarget(float deltaTime)
{
	// if defend order and already at position -> false, if not there yet -> true
	// if movement order -> target

	// Reset movement target
	SetMovementTarget(XMFLOAT2(0.0f, 0.0f));
	SetMovementTargetSet(false);

	if(GetCurrentOrder())
	{
		if(GetCurrentOrder()->GetOrderType() == DefendPositionOrder)
		{
			if(IsAtTarget(reinterpret_cast<DefendOrder*>(GetCurrentOrder())->GetDefendPosition()))
			{
				SetMovementTargetSet(false);
			}else
			{
				SetMovementTarget(reinterpret_cast<DefendOrder*>(GetCurrentOrder())->GetDefendPosition());
				SetMovementTargetSet(true);
			}

			return StatusSuccess;
		}

		if(GetCurrentOrder()->GetOrderType() == MoveToPositionOrder)
		{
			SetMovementTarget(reinterpret_cast<MoveOrder*>(GetCurrentOrder())->GetTargetPosition());
			SetMovementTargetSet(true);

			return StatusSuccess;
		}
	}

	// If there is no order, just pick a random target position within the test environment to patrol
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
	// Reset greatest known threat
	SetGreatestKnownThreat(nullptr);

	if(GetCurrentOrder())
	{
		// Check if there is a current order that requires to attack a certain enemy.
		if(GetCurrentOrder()->GetOrderType() == AttackEnemyOrder && GetCurrentOrder()->GetOrderPriority() == HighPriority)
		{
			// If the target enemy is in a known threat of this soldier -> set him as greatest threat to make sure the soldier
			// attacks that target.
			unsigned long enemyId = reinterpret_cast<AttackOrder*>(GetCurrentOrder())->GetEnemyId();
			if(IsKnownThreat(enemyId))
			{
				SetGreatestKnownThreat(GetKnownThreat(enemyId));
			}else
			{
				// The required enemy is not visible to the soldier but due to the high priority of the attack order, the
				// soldier is not allowed to attack other threats. Moving to the position of the require enemy is now his
				// main concern. Thus, no greatest known threat is set in order to bail out of the attack behaviour.
				SetGreatestKnownThreat(nullptr);
			}

			return StatusSuccess;
		}

		if((GetCurrentOrder()->GetOrderType() == MoveToPositionOrder && GetCurrentOrder()->GetOrderPriority() == HighPriority) ||
			((GetCurrentOrder()->GetOrderType() == DefendPositionOrder && GetCurrentOrder()->GetOrderPriority() == HighPriority) 
			&& !IsAtTarget(reinterpret_cast<DefendOrder*>(GetCurrentOrder())->GetDefendPosition())))
		{
			// The soldier has a high priority move order or has to move to a defend position asap. This means the soldier has 
			// to ignore all enemies and try to reach his target. Thus, no greatest known threat is being set.
			SetGreatestKnownThreat(nullptr);
			return StatusSuccess;
		}
	}

	if((GetCurrentOrder() && GetCurrentOrder()->GetOrderType() == DefendPositionOrder && GetCurrentOrder()->GetOrderPriority() == HighPriority))
	{
		if(!GetKnownThreats().empty())
		{
			int a = 2;
		}
		
		
	}

	if(GetCurrentOrder() && GetCurrentOrder()->GetOrderType() == DefendPositionOrder)
	{
		int a = 2;
	}

	// No orders given that require special measures, just choose the greatest known threat as perceived by the soldier.
	SetGreatestKnownThreat(m_combatManager.DetermineGreatestKnownThreat());

	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Determines and sets the currently greatest suspected threat to the Soldier.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::DetermineGreatestSuspectedThreat(float deltaTime)
{
	if(GetCurrentOrder())
	{
		if(GetCurrentOrder()->GetOrderPriority() == HighPriority || GetCurrentOrder()->GetOrderPriority() == MediumPriority)
		{
			if(GetCurrentOrder()->GetOrderPriority() == HighPriority && GetCurrentOrder()->GetOrderType() == AttackEnemyOrder)
			{
				// Only move to the requested enemy

				SetGreatestSuspectedThreat(GetSuspectedThreat(reinterpret_cast<AttackOrder*>(GetCurrentOrder())->GetEnemyId()));
				return StatusSuccess;
			}

			// For any other orders of priority high and medium, soliders should not investigate any suspected threats (that also
			// means not pursue any enemies) as there are other objectives of higher priority.
			SetGreatestSuspectedThreat(nullptr);
			return StatusSuccess;
		}
	}

	// No orders given that require special measures, just choose the greatest suspected threat as perceived by the soldier.
	SetGreatestSuspectedThreat(m_combatManager.DetermineGreatestSuspectedThreat());

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
// Determines/Calculates a path to the current movement target and sets it as the active path.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::DeterminePathToTarget(float deltaTime)
{
	if(GetCurrentOrder() && GetCurrentOrder()->GetOrderType() == MoveToPositionOrder)
	{
		if(!reinterpret_cast<MoveOrder*>(GetCurrentOrder())->GetPath()->empty())
		{
			if(GetCurrentOrder()->GetOrderPriority() == MediumPriority || GetCurrentOrder()->GetOrderPriority() == HighPriority)
			{
				// Use the path provided by the team AI
				SetPath(reinterpret_cast<MoveOrder*>(GetCurrentOrder())->GetPath());
				// If the path was started before, resume it.
				m_movementManager.SetCurrentNode(GetResumePathNode());
			}else
			{
				// Let the soldier find a path himself.
				SetPath(m_movementManager.CreatePathTo(GetMovementTarget()));
				m_movementManager.SetCurrentNode(0);
			}
		}else
		{
			// Let the soldier find a path himself.
			SetPath(m_movementManager.CreatePathTo(GetMovementTarget()));
			m_movementManager.SetCurrentNode(0);
		}
	}else
	{
		// Let the soldier find a path himself.
		SetPath(m_movementManager.CreatePathTo(GetMovementTarget()));
		m_movementManager.SetCurrentNode(0);
	}

	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Determines a point to observe for the soldier and sets it as the active observation point.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::DetermineObservationTarget(float deltaTime)
{

	if(GetCurrentOrder() && GetCurrentOrder()->GetOrderType() == DefendPositionOrder)
	{
		const XMFLOAT2& viewDirection = reinterpret_cast<DefendOrder*>(GetCurrentOrder())->GetViewDirection();
		
		if(viewDirection.x != 0.0f || viewDirection.y != 0.0f)
		{
			// There is a defend order and a specific view direction that the entity should guard has been provided
			SetObservationTarget(XMFLOAT2(GetPosition().x + viewDirection.x, GetPosition().y + viewDirection.y));
			SetObservationTargetSet(true);

			return StatusSuccess;
		}
	}

	// Otherwise determine a random look at target and change it in a fix interval

	// Reset observation target
	SetObservationTarget(XMFLOAT2(0.0f, 0.0f));
	SetObservationTargetSet(false);
	
	// Update the timer
	m_changeObservationTargetTimer += deltaTime;

	// Check if the soldier should change his observation target
	if(m_changeObservationTargetTimer >= m_soldierProperties.m_lookAroundInterval)
	{
		// Determine a random lookAt position

		SetObservationTarget(XMFLOAT2(GetPosition().x + (rand() % 128 - 64), GetPosition().y + (rand() % 128 - 64)));
		SetObservationTargetSet(true);
		m_changeObservationTargetTimer = 0.0f;
	}
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Makes the entity look at the currently set observation target.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::LookAtTarget(float deltaTime)
{
	m_movementManager.LookAt(GetObservationTarget());
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Finalise the movement of the entity when it reaches its objective.
// Param1: The time in seconds passed since the last frame.
// Returns the current state of the action.
//--------------------------------------------------------------------------------------
BehaviourStatus Soldier::FinaliseMovement(float deltaTime)
{
	SetResumePathNode(0);

	if(GetCurrentOrder() && GetCurrentOrder()->GetOrderType() == MoveToPositionOrder)
	{
		UpdateOrderStateMessageData data(GetId(), SucceededOrderState);
		SendMessage(GetTeamAI(), UpdateOrderStateMessageType, &data);
	}
	
	return StatusSuccess;
}

//--------------------------------------------------------------------------------------
// Tells whether the soldier is currently moving towards the greatest suspected threat in
// order to investigate it or whether there now is a greater suspected threat that the
// entity should tend to. It also factors in whether there are orders from the team AI based
// on which the soldier should stop pursuing a target and tend to other objectives instead.
// Returns true if the suspected threat being investigated is still the greatest suspected threat,
// false otherwise and also if there is no suspected threat at all.
//--------------------------------------------------------------------------------------
bool Soldier::IsInvestigatingGreatestSuspectedThreat(void)
{
	if(GetCurrentOrder())
	{
		if((GetCurrentOrder()->GetOrderType() == MoveToPositionOrder || GetCurrentOrder()->GetOrderType() == DefendPositionOrder) &&
			(GetCurrentOrder()->GetOrderPriority() != LowPriority))
		{
			// A higher level move or defend order has become active -> abort the pursuit of enemies.
			return false;
		}
	}

	// No specific orders, just check if there is a greater suspected threat (also valid for attack orders)
	return Entity::IsInvestigatingGreatestSuspectedThreat();
}

//--------------------------------------------------------------------------------------
// Tells whether the entity is currently moving towards the highest priority movement target 
// or whether there now is a higher priority target that the entity should move towards instead.
// Returns true if the target being moved towards is still the highest priority target,
// false otherwise and also if there is no movement target set at all.
//--------------------------------------------------------------------------------------
bool Soldier::IsMovingToHighestPriorityTarget(void)
{
	if(!IsMovementTargetSet())
	{
		return false;
	}

	// if defense order, might have to bail out if not moving to defend position
	// if movement order comes in and not currently moving to that taregt -> bail out

	if(GetCurrentOrder() && GetCurrentOrder()->GetOrderType() == MoveToPositionOrder)
	{
		const XMFLOAT2& orderTarget = reinterpret_cast<MoveOrder*>(GetCurrentOrder())->GetTargetPosition();

		if(GetMovementTarget().x != orderTarget.x || GetMovementTarget().y != orderTarget.y)
		{
			// Soldier should stop moving to current target and follow order instead
			return false;
		}
	}

	if(GetCurrentOrder() && GetCurrentOrder()->GetOrderType() == DefendPositionOrder)
	{
		const XMFLOAT2& orderTarget = reinterpret_cast<DefendOrder*>(GetCurrentOrder())->GetDefendPosition();

		if(GetMovementTarget().x != orderTarget.x || GetMovementTarget().y != orderTarget.y)
		{
			// Soldier should stop moving to current target and follow order instead
			return false;
		}
	}

	return true;
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

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void Soldier::ProcessEvent(EventType type, void* pEventData)
{
	// Forward all calls to the base class
	Entity::ProcessEvent(type, pEventData);
}

//--------------------------------------------------------------------------------------
// Tells whether a soldier is at a certain target or not. Accounts for the target reached
// radius used by the soldier.
// Param1: The target position, for which to check if the soldier has reached it.
// Returns true if the soldier has reached the target, false otherwise. 
//--------------------------------------------------------------------------------------
bool Soldier::IsAtTarget(const XMFLOAT2& target)
{
	float distance(0.0f);
	XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&target) - XMLoadFloat2(&GetPosition())));

	return (distance <= m_soldierProperties.m_targetReachedRadius);
}

//--------------------------------------------------------------------------------------
// Processes a given message. Default implementation.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void Soldier::ProcessMessage(Message* pMessage)
{
	// Forward all calls to the base class
	Entity::ProcessMessage(pMessage);
}


// Data access functions

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

unsigned int Soldier::GetResumePathNode(void) const
{
	return m_resumePathNode;
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

void Soldier::SetResumePathNode(unsigned int node)
{
	m_resumePathNode = node;
}
