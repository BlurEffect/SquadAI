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
#include "EntityAlive.h"
#include "InvestigatingGreatestSuspectedThreat.h"
// Actions
#include "DeterminePatrolTarget.h"
#include "DetermineApproachThreatPosition.h"
#include "DetermineAttackTarget.h"
#include "DetermineGreatestKnownThreat.h"
#include "DetermineGreatestSuspectedThreat.h"
#include "AttackTarget.h"
#include "AimAtTarget.h"
#include "UpdateThreats.h"
#include "UpdateAttackReadiness.h"
#include "Idle.h"
#include "MoveToTarget.h"
#include "ResolveSuspectedThreat.h"
#include "ProcessMessages.h"
// Team behaviours
#include "TeamProcessMessages.h"

// Forward declarations
class Entity;
class TeamAI;
class MultiflagCTFTeamAI;

//--------------------------------------------------------------------------------------
// Identifies the individual behaviours that are available and can be used on all types
// of entities.
//--------------------------------------------------------------------------------------
enum UniversalIndividualBehaviourType
{
	MovementTargetSetType,
	ReadyToAttackType,
	AttackTargetSetType,
	GreatestKnownThreatSetType,
	GreatestSuspectedThreatSetType,
	EntityAliveType,
	InvestigatingGreatestSuspectedThreatType,

	DeterminePatrolTargetType,
	DetermineApproachThreatPositionType,
	DetermineAttackTargetType,
	DetermineGreatestKnownThreatType,
	DetermineGreatestSuspectedThreatType,
	AttackTargetType,
	AimAtTargetType,
	UpdateThreatsType,
	UpdateAttackReadinessType,
	IdleType,
	MoveToTargetType,
	ResolveSuspectedThreatType,
	ProcessMessagesType
};

//--------------------------------------------------------------------------------------
// Identifies the behaviours that can be universally applied.
//--------------------------------------------------------------------------------------
enum UniversalBehaviourType
{
	SelectorType,
	ActiveSelectorType,
	SequenceType,
	ParallelType,
	MonitorType,
	RepeatType,
	ReturnSpecificStatusType
};

//--------------------------------------------------------------------------------------
// Identifies the behaviours that can be universally applied by all team AIs.
//--------------------------------------------------------------------------------------
enum UniversalTeamBehaviourType
{
	TeamProcessMessagesType
};

//--------------------------------------------------------------------------------------
// Identifies the behaviours that can be used for team AIs in the game mode Multiflag CTF.
//--------------------------------------------------------------------------------------
enum MultiflagCTFTeamBehaviourType
{
	
};

//--------------------------------------------------------------------------------------
// Lists available types of behaviour trees.
//--------------------------------------------------------------------------------------
enum BehaviourTreeType
{
	SimpleIndividualMovementTree,
	SimpleIndividualCombatTree,
	SimpleTeamMultiflagCTFTree
};


class BehaviourFactory
{
public:
	static Behaviour* CreateBehaviourTree(BehaviourTreeType treeType, void* pTreeOwner);

private:
	static Behaviour* CreateSimpleMovementTree(Entity* pEntity);
	static Behaviour* CreateSimpleCombatTree(Entity* pEntity);
	static Behaviour* CreateSimpleTeamMultiflagCTFTree(MultiflagCTFTeamAI* pTeamAI);

	// Factory functions for the different types of behaviours
	static Behaviour* CreateUniversalBehaviour(UniversalBehaviourType behaviourType, const char* name, void* pInitData);
	static Behaviour* CreateUniversalIndividualBehaviour(UniversalIndividualBehaviourType behaviourType, Entity* pEntity, const char* name, void* pInitData);
	static Behaviour* CreateUniversalTeamBehaviour(UniversalTeamBehaviourType behaviourType, TeamAI* pTeamAI, const char* name, void* pInitData);
	static Behaviour* CreateMultiflagCTFTeamBehaviour(MultiflagCTFTeamBehaviourType behaviourType, MultiflagCTFTeamAI* pMultiflagCTFTeamAI, const char* name, void* pInitData);
};

#endif // BEHAVIOUR_FACTORY_H
