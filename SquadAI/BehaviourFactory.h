/* 
*  Kevin Meergans, SquadAI, 2014
*  BehaviourFactory.h
*  Class with a couple of static member function used to create behaviours
*  and to assemble them into behaviour trees.
*/

#ifndef BEHAVIOUR_FACTORY_H
#define BEHAVIOUR_FACTORY_H

// Includes

#include "Parallel.h"
#include "Monitor.h"
#include "Repeat.h"
#include "Selector.h"
#include "ActiveSelector.h"
#include "Sequence.h"

#include "DeterminePatrolTarget.h"
#include "Idle.h"
#include "MovementTargetSet.h"
#include "MoveToTarget.h"

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
	DeterminePatrolTargetType,
	IdleType,
	MovementTargetSetType,
	MoveToTargetType
};

//--------------------------------------------------------------------------------------
// Lists available types of behaviour trees.
//--------------------------------------------------------------------------------------
enum BehaviourTreeType
{
	SimpleMovementTree
};


class BehaviourFactory
{
public:
	static Behaviour* CreateBehaviourTree(BehaviourTreeType entityType, Entity* pEntity);
private:
	static Behaviour* CreateSimpleMovementTree(Entity* pEntity);
	static Behaviour* CreateBehaviour(BehaviourType behaviourType, Entity* pEntity, const char* name, void* pInitData);
};

#endif // BEHAVIOUR_FACTORY_H
