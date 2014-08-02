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
	case SimpleCombatTree:
		return CreateSimpleCombatTree(pEntity);
		break;
	default:
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Creates a simple behaviour tree controlling basic movement for an entity.
// Param1: A pointer to the entity that the behaviour tree should be created for.
// Returns a pointer to the root behaviour of the created tree. The pointer is null if 
// something failed during creation.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateSimpleMovementTree(Entity* pEntity)
{
	Behaviour* pRoot = CreateBehaviour(SelectorType, pEntity, "Root", nullptr);

	if(pRoot)
	{
		Behaviour* pPatrolSequence = CreateBehaviour(SequenceType, pEntity, "PatrolSequence", nullptr);
		Behaviour* pIdleAction     = CreateBehaviour(IdleType, pEntity, "IdleAction", nullptr);
		
		if(pPatrolSequence && pIdleAction)
		{
			reinterpret_cast<Composite*>(pRoot)->AddChild(pPatrolSequence);
			reinterpret_cast<Composite*>(pRoot)->AddChild(pIdleAction);

			Behaviour* pDeterminePatrolTargetAction = CreateBehaviour(DeterminePatrolTargetType, pEntity, "DeterminePatrolTargetAction", nullptr);
			Behaviour* pMovementTargetSetCondition = CreateBehaviour(MovementTargetSetType, pEntity, "MovementTargetSetCondition", nullptr);
			Behaviour* pMoveToTargetAction = CreateBehaviour(MoveToTargetType, pEntity, "MoveToTargetAction", nullptr);
		
			if(pDeterminePatrolTargetAction && pMovementTargetSetCondition && pMoveToTargetAction)
			{
				reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pDeterminePatrolTargetAction);
				reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pMovementTargetSetCondition);
				reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pMoveToTargetAction);
				return &(*pRoot);
			}
		}
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------
// Creates a simple behaviour tree controlling basic movement and combat for an entity.
// Param1: A pointer to the entity that the behaviour tree should be created for.
// Returns a pointer to the root behaviour of the created tree. The pointer is null if 
// something failed during creation.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateSimpleCombatTree(Entity* pEntity)
{
	Behaviour* pRoot = CreateBehaviour(SequenceType, pEntity, "Root", nullptr);

	if(pRoot)
	{
		Behaviour* pThreatMgmtSequence = CreateBehaviour(SequenceType, pEntity, "ThreatManagementSequence", nullptr);
		
		if(pThreatMgmtSequence)
		{
			Behaviour* pUpdateThreatsAction				= CreateBehaviour(UpdateThreatsType, pEntity, "UpdateThreatsAction", nullptr);
			Behaviour* pDetermineGreatestThreatsAction  = CreateBehaviour(DetermineGreatestThreatsType, pEntity, "DetermineGreatestThreatsAction", nullptr);
			
			ReturnSpecificStatusInitData initData(pThreatMgmtSequence, StatusSuccess);
			Behaviour* pSuccessDecorator = CreateBehaviour(ReturnSpecificStatusType, pEntity, "SuccessDecorator", &initData);

			if(pUpdateThreatsAction && pDetermineGreatestThreatsAction && pSuccessDecorator)
			{
				reinterpret_cast<Composite*>(pRoot)->AddChild(pSuccessDecorator);
				reinterpret_cast<Composite*>(pThreatMgmtSequence)->AddChild(pUpdateThreatsAction);
				reinterpret_cast<Composite*>(pThreatMgmtSequence)->AddChild(pDetermineGreatestThreatsAction);

				Behaviour* pMainSelector		   = CreateBehaviour(SelectorType, pEntity, "MainSelector", nullptr);
				Behaviour* pAttackSequence		   = CreateBehaviour(SequenceType, pEntity, "AttackSequence", nullptr);
				Behaviour* pApproachThreatSequence = CreateBehaviour(SequenceType, pEntity, "ApproachThreatSequence", nullptr);
				Behaviour* pPatrolSequence		   = CreateBehaviour(SequenceType, pEntity, "PatrolSequence", nullptr);
				Behaviour* pIdleAction			   = CreateBehaviour(IdleType, pEntity, "IdleAction", nullptr);

				if(pMainSelector && pAttackSequence && pApproachThreatSequence && pPatrolSequence && pIdleAction)
				{
					reinterpret_cast<Composite*>(pRoot)		   ->AddChild(pMainSelector);
					reinterpret_cast<Composite*>(pMainSelector)->AddChild(pAttackSequence);
					reinterpret_cast<Composite*>(pMainSelector)->AddChild(pApproachThreatSequence);
					reinterpret_cast<Composite*>(pMainSelector)->AddChild(pPatrolSequence);
					reinterpret_cast<Composite*>(pMainSelector)->AddChild(pIdleAction);

					Behaviour* pGreatestKnownThreatSetCondition = CreateBehaviour(GreatestKnownThreatSetType, pEntity, "GreatestKnownThreatSetCondition", nullptr);
					Behaviour* pUpdateAttackReadinessAction		= CreateBehaviour(UpdateAttackReadinessType, pEntity, "UpdateAttackReadinessAction", nullptr);
					Behaviour* pReadyToAttackCondition		    = CreateBehaviour(ReadyToAttackType, pEntity, "ReadyToAttackCondition", nullptr);
					Behaviour* pDetermineAttackTargetAction		= CreateBehaviour(DetermineAttackTargetType, pEntity, "DetermineAttackTargetAction", nullptr);
					Behaviour* pAttackTargetSetCondition		= CreateBehaviour(AttackTargetSetType, pEntity, "AttackTargetSetCondition", nullptr);
					Behaviour* pAimAtTargetAction				= CreateBehaviour(AimAtTargetType, pEntity, "AimAtTargetAction", nullptr);
					Behaviour* pAttackTargetAction				= CreateBehaviour(AttackTargetType, pEntity, "AttackTargetAction", nullptr);
	
					Behaviour* pGreatestSuspectedThreatSetCondition	  = CreateBehaviour(GreatestSuspectedThreatSetType, pEntity, "GreatestSuspectedThreatSetCondition", nullptr);
					Behaviour* pDetermineApproachThreatPositionAction = CreateBehaviour(DetermineApproachThreatPositionType, pEntity, "DetermineApproachThreatPositionAction", nullptr);
					Behaviour* pMovementTargetSetCondition1			  = CreateBehaviour(MovementTargetSetType, pEntity, "MovementTargetSetCondition1", nullptr);
					Behaviour* pMoveToTargetAction1					  = CreateBehaviour(MoveToTargetType, pEntity, "MoveToTargetAction1", nullptr);

					Behaviour* pDeterminePatrolTargetAction = CreateBehaviour(DeterminePatrolTargetType, pEntity, "DeterminePatrolTargetAction", nullptr);
					Behaviour* pMovementTargetSetCondition2  = CreateBehaviour(MovementTargetSetType, pEntity, "MovementTargetSetCondition2", nullptr);
					Behaviour* pMoveToTargetAction2			= CreateBehaviour(MoveToTargetType, pEntity, "MoveToTargetAction2", nullptr);
		
					if(pGreatestKnownThreatSetCondition && pUpdateAttackReadinessAction && pReadyToAttackCondition && pDetermineAttackTargetAction &&
					   pAttackTargetSetCondition && pAimAtTargetAction && pAttackTargetAction && pGreatestSuspectedThreatSetCondition &&
					   pDetermineApproachThreatPositionAction && pMovementTargetSetCondition1 && pMoveToTargetAction1 && pDeterminePatrolTargetAction &&
					   pMovementTargetSetCondition2 && pMoveToTargetAction2)
					{
						reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pGreatestKnownThreatSetCondition);
						reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pUpdateAttackReadinessAction);
						reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pReadyToAttackCondition);
						reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pDetermineAttackTargetAction);
						reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pAttackTargetSetCondition);
						reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pAimAtTargetAction);
						reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pAttackTargetAction);

						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pGreatestSuspectedThreatSetCondition);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pDetermineApproachThreatPositionAction);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pMovementTargetSetCondition1);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pMoveToTargetAction1);

						reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pDeterminePatrolTargetAction);
						reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pMovementTargetSetCondition2);
						reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pMoveToTargetAction2);

						return &(*pRoot);
					}
				}
			}
		}
	}

	return nullptr;
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
	case ReturnSpecificStatusType:
		{
		ReturnSpecificStatusInitData* pData = reinterpret_cast<ReturnSpecificStatusInitData*>(pInitData);
		return new ReturnSpecificStatus(pEntity, name, pData->m_pChild, pData->m_returnStatus);
		break;
		}
	case MovementTargetSetType:
		return new MovementTargetSet(pEntity, name);
		break;
	case ReadyToAttackType:
		return new ReadyToAttack(pEntity, name);
		break;
	case AttackTargetSetType:
		return new AttackTargetSet(pEntity, name);
		break;
	case GreatestKnownThreatSetType:
		return new GreatestKnownThreatSet(pEntity, name);
		break;
	case GreatestSuspectedThreatSetType:
		return new GreatestSuspectedThreatSet(pEntity, name);
		break;
	case DeterminePatrolTargetType:
		return new DeterminePatrolTarget(pEntity, name);
		break;
	case DetermineApproachThreatPositionType:
		return new DetermineApproachThreatPosition(pEntity, name);
		break;
	case DetermineAttackTargetType:
		return new DetermineAttackTarget(pEntity, name);
		break;
	case DetermineGreatestThreatsType:
		return new DetermineGreatestThreats(pEntity, name);
		break;
	case AttackTargetType:
		return new AttackTarget(pEntity, name);
		break;
	case AimAtTargetType:
		return new AimAtTarget(pEntity, name);
		break;
	case UpdateThreatsType:
		return new UpdateThreats(pEntity, name);
		break;
	case UpdateAttackReadinessType:
		return new UpdateAttackReadiness(pEntity, name);
		break;
	case IdleType:
		return new Idle(pEntity, name);
		break;
	case MoveToTargetType:
		return new MoveToTarget(pEntity, name);
		break;
	default:
		return nullptr;
	}
}


