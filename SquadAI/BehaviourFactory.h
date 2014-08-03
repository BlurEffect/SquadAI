/* 
*  Kevin Meergans, SquadAI, 2014
*  BehaviourFactory.h
*  Class with a couple of static member function used to create behaviours
*  and to assemble them into behaviour trees.
*/

#ifndef BEHAVIOUR_FACTORY_H
#define BEHAVIOUR_FACTORY_H

// Includes

// Parent Nodes and Composites
#include "Parallel.h"
#include "Monitor.h"
#include "Repeat.h"
#include "Selector.h"
#include "ActiveSelector.h"
#include "Sequence.h"
#include "ReturnSpecificStatus.h"
// Conditions
#include "MovementTargetSet.h"
#include "ReadyToAttack.h"
#include "AttackTargetSet.h"
#include "GreatestKnownThreatSet.h"
#include "GreatestSuspectedThreatSet.h"
// Actions
#include "DeterminePatrolTarget.h"
#include "DetermineApproachThreatPosition.h"
#include "DetermineAttackTarget.h"
#include "DetermineGreatestThreats.h"
#include "AttackTarget.h"
#include "AimAtTarget.h"
#include "UpdateThreats.h"
#include "UpdateAttackReadiness.h"
#include "Idle.h"
#include "MoveToTarget.h"
#include "ProcessMessages.h"

// Forward declarations
class Entity;

//--------------------------------------------------------------------------------------
// Identifies the behaviours that are available.
//--------------------------------------------------------------------------------------
enum BehaviourType
{
	SelectorType,
	ActiveSelectorType,
	SequenceType,
	ParallelType,
	MonitorType,
	RepeatType,
	ReturnSpecificStatusType,

	MovementTargetSetType,
	ReadyToAttackType,
	AttackTargetSetType,
	GreatestKnownThreatSetType,
	GreatestSuspectedThreatSetType,

	DeterminePatrolTargetType,
	DetermineApproachThreatPositionType,
	DetermineAttackTargetType,
	DetermineGreatestThreatsType,
	AttackTargetType,
	AimAtTargetType,
	UpdateThreatsType,
	UpdateAttackReadinessType,
	IdleType,
	MoveToTargetType,
	ProcessMessagesType
};

//--------------------------------------------------------------------------------------
// Lists available types of behaviour trees.
//--------------------------------------------------------------------------------------
enum BehaviourTreeType
{
	SimpleMovementTree,
	SimpleCombatTree
};


class BehaviourFactory
{
public:
	static Behaviour* CreateBehaviourTree(BehaviourTreeType entityType, Entity* pEntity);
private:
	static Behaviour* CreateSimpleMovementTree(Entity* pEntity);
	static Behaviour* CreateSimpleCombatTree(Entity* pEntity);
	static Behaviour* CreateBehaviour(BehaviourType behaviourType, Entity* pEntity, const char* name, void* pInitData);
};

#endif // BEHAVIOUR_FACTORY_H
