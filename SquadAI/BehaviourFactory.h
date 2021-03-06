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
#include "ObservationTargetSet.h"
#include "PathToTargetSet.h"
#include "MovingToHighestPriorityTarget.h"
// Actions
#include "DetermineMovementTarget.h"
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
#include "DetermineObservationTarget.h"
#include "DeterminePathToTarget.h"
#include "LookAtTarget.h"
#include "FinaliseMovement.h"
// Primitive Team Behaviours
#include "TeamProcessMessages.h"
#include "ExecuteTeamManoeuvre.h"
#include "ManoeuvrePreconditionsFulfilled.h"
#include "ManoeuvreStillValid.h"
#include "InitiateTeamManoeuvre.h"
// Parent Team Behaviours
#include "TeamSelector.h"
#include "TeamActiveSelector.h"
#include "TeamActiveCharacteristicSelector.h"
#include "TeamSequence.h"
#include "TeamParallel.h"
#include "TeamMonitor.h"
#include "TeamRepeat.h"
#include "TeamReturnSpecificStatus.h"

// Forward declarations
class Entity;
class TeamAI;

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
	ObservationTargetSetType,
	PathToTargetSetType,
	MovingToHighestPriorityTargetType,

	DetermineMovementTargetType,
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
	ProcessMessagesType,
	DetermineObservationTargetType,
	DeterminePathToTargetType,
	LookAtTargetType,
	FinaliseMovementType
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
enum PrimitiveTeamBehaviourType
{
	TeamProcessMessagesType,
	TeamInitiateManoeuvreType,
	TeamExecuteManoeuvreType,
	TeamManoeuvrePreconditionsFulfilledType,
	TeamManoeuvreStillValidType
};

//--------------------------------------------------------------------------------------
// Identifies the behaviours that can be universally applied by all team AIs.
//--------------------------------------------------------------------------------------
enum ParentTeamBehaviourType
{
	TeamSelectorType,
	TeamActiveSelectorType,
	TeamActiveCharacteristicSelectorType,
	TeamSequenceType,
	TeamParallelType,
	TeamMonitorType,
	TeamRepeatType,
	TeamReturnSpecificStatusType
};

//--------------------------------------------------------------------------------------
// Lists available types of behaviour trees.
//--------------------------------------------------------------------------------------
enum BehaviourTreeType
{
	SimpleIndividualMovementTree,
	SimpleIndividualCombatTree,
	ModifiedSimpleIndividualCombatTree, // The most evolved individual behaviour tree
	SimpleTeamMultiflagCTFTree          // The behaviour tree for the CTF team AI
};


class BehaviourFactory
{
public:
	static Behaviour* CreateBehaviourTree(BehaviourTreeType treeType, void* pTreeOwner);

private:
	static Behaviour* CreateSimpleMovementTree(Entity* pEntity);
	static Behaviour* CreateSimpleCombatTree(Entity* pEntity);
	static Behaviour* CreateModifiedSimpleCombatTree(Entity* pEntity);
	static Behaviour* CreateSimpleTeamMultiflagCTFTree(TeamAI* pTeamAI);

	// Factory functions for the different types of behaviours
	static Behaviour* CreateUniversalBehaviour(UniversalBehaviourType behaviourType, const char* name, void* pInitData);
	static Behaviour* CreateUniversalIndividualBehaviour(UniversalIndividualBehaviourType behaviourType, Entity* pEntity, const char* name, void* pInitData);
	static TeamBehaviour* CreatePrimitiveTeamBehaviour(PrimitiveTeamBehaviourType behaviourType, TeamAI* pTeamAI, const char* name, float aggressiveness, float defensiveness, void* pInitData);
	static TeamBehaviour* CreateParentTeamBehaviour(ParentTeamBehaviourType behaviourType, TeamAI* pTeamAI, const char* name, void* pInitData);
};

#endif // BEHAVIOUR_FACTORY_H
