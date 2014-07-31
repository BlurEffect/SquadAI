/* 
*  Kevin Meergans, SquadAI, 2014
*  BehaviourFactory.cpp
*  Class with a couple of static member function used to create behaviours
*  and to assemble them into behaviour trees.
*/

// Includes
#include "BehaviourFactory.h"
#include "Entity.h"

//--------------------------------------------------------------------------------------
// Creates a behaviour tree for the provided entity.
// Param1: Determines the type of the entity and thus, which behaviour tree will be created.
// Param2: A pointer to the entity that the behaviour tree should be created for.
// Returns a pointer to the root behaviour of the created tree. The pointer is null if 
// something failed during creation or if the factory does not support the given entity
// type.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateBehaviourTree(BehaviourTreeType behaviourTreeType, Entity* pEntity)
{
	switch(behaviourTreeType)
	{
	case SimpleMovementTree:
		return CreateSimpleMovementTree(pEntity);
		break;
	default:
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Creates a simple behaviour tree controlling basic movement for an entity.
// Param2: A pointer to the soldier entity that the behaviour tree should be created for.
// Returns a pointer to the root behaviour of the created tree. The pointer is null if 
// something failed during creation.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateSimpleMovementTree(Entity* pEntity)
{
	Behaviour* pRoot = CreateBehaviour(SelectorType, pEntity, "Root", nullptr);

	return pRoot;
}

//--------------------------------------------------------------------------------------
// Creates a certain behaviour for the provided entity.
// Param1: Determines which behaviour will be created from a range of available types.
// Param2: A pointer to the entity that the behaviour tree should be created for.
// Param3: The name that will be associated to the newly created behaviour.
// Param4: A pointer to a data structure holding the initialisation data for some behaviours.
// Returns a pointer to the newly created behaviour. The pointer is null if something failed 
// during creation or if the factory does not support the given behaviour type.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateBehaviour(BehaviourType behaviourType, Entity* pEntity, const char* name, void* pInitData)
{
	switch(behaviourType)
	{
	case SelectorType:
		return new Selector(pEntity, name);
		break;
	case ActiveSelectorType:
		return new ActiveSelector(pEntity, name);
		break;
	case SequenceType:
		return new Sequence(pEntity, name);
		break;
	case ParallelType:
		{
		ParallelInitData* pData = reinterpret_cast<ParallelInitData*>(pInitData);
		return new Parallel(pEntity, name, pData->m_successPolicy, pData->m_failurePolicy);
		break;
		}
	case MonitorType:
		return new Monitor(pEntity, name);
		break;
	case RepeatType:
		{
		RepeatInitData* pData = reinterpret_cast<RepeatInitData*>(pInitData);
		return new Repeat(pEntity, name, pData->m_pChild, pData->m_numberOfRepeats);
		break;
		}
	case DeterminePatrolTargetType:
		return new DeterminePatrolTarget(pEntity, name);
		break;
	case IdleType:
		return new Idle(pEntity, name);
		break;
	case MovementTargetSetType:
		return new MovementTargetSet(pEntity, name);
		break;
	case MoveToTargetType:
		return new MoveToTarget(pEntity, name);
		break;
	default:
		return nullptr;
	}

	
}


