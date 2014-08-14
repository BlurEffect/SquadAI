/* 
*  Kevin Meergans, SquadAI, 2014
*  BehaviourFactory.cpp
*  Class with a couple of static member function used to create behaviours
*  and to assemble them into behaviour trees.
*/

// Includes
#include "BehaviourFactory.h"
#include "Entity.h"
#include "TeamAI.h"

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
// Improved CombatTree2 with better reactiveness towards events and higher priority behaviours
// becoming available.
// Param1: A pointer to the entity that the behaviour tree should be created for.
// Returns a pointer to the root behaviour of the created tree. The pointer is null if 
// something failed during creation.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateSimpleCombatTree(Entity* pEntity)
{
	Behaviour* pRoot = CreateBehaviour(ActiveSelectorType, pEntity, "Root", nullptr);

	if(pRoot)
	{
		Behaviour* pUpdateEntitySequence = CreateBehaviour(SequenceType, pEntity, "UpdateEntitySequence", nullptr);
		
		if(pUpdateEntitySequence)
		{
			ReturnSpecificStatusInitData data(pUpdateEntitySequence, StatusFailure);
			Behaviour* pAlwaysFailDecorator = CreateBehaviour(ReturnSpecificStatusType, pEntity, "AlwaysFailDecorator", &data);

			Behaviour* pProcessMessagesAction		    = CreateBehaviour(ProcessMessagesType, pEntity, "ProcessMessagesAction", nullptr);
			Behaviour* pEntityAliveCondition		    = CreateBehaviour(EntityAliveType, pEntity, "EntityAliveCondition", nullptr);
			Behaviour* pUpdateThreatsAction				= CreateBehaviour(UpdateThreatsType, pEntity, "UpdateThreatsAction", nullptr);
			Behaviour* pUpdateAttackReadinessAction		= CreateBehaviour(UpdateAttackReadinessType, pEntity, "UpdateAttackReadinessAction", nullptr);

			if(pProcessMessagesAction && pAlwaysFailDecorator && pEntityAliveCondition && pUpdateThreatsAction && pUpdateAttackReadinessAction)
			{
				reinterpret_cast<Composite*>(pRoot)->AddChild(pAlwaysFailDecorator);

				reinterpret_cast<Composite*>(pUpdateEntitySequence)->AddChild(pProcessMessagesAction);
				reinterpret_cast<Composite*>(pUpdateEntitySequence)->AddChild(pEntityAliveCondition);
				reinterpret_cast<Composite*>(pUpdateEntitySequence)->AddChild(pUpdateThreatsAction);
				reinterpret_cast<Composite*>(pUpdateEntitySequence)->AddChild(pUpdateAttackReadinessAction);

				Behaviour* pFightSequence		   = CreateBehaviour(SequenceType, pEntity, "FightSequence", nullptr);
				Behaviour* pApproachThreatSequence = CreateBehaviour(SequenceType, pEntity, "ApproachThreatSequence", nullptr);
				Behaviour* pPatrolSequence		   = CreateBehaviour(SequenceType, pEntity, "PatrolSequence", nullptr);
				Behaviour* pIdleAction			   = CreateBehaviour(IdleType, pEntity, "IdleAction", nullptr);

				if(pFightSequence && pApproachThreatSequence && pPatrolSequence && pIdleAction)
				{
					reinterpret_cast<Composite*>(pRoot)->AddChild(pFightSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pApproachThreatSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pPatrolSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pIdleAction);

					Behaviour* pDetermineGreatestKnownThreatAction = CreateBehaviour(DetermineGreatestKnownThreatType, pEntity, "DetermineGreatestKnownThreatAction", nullptr);
					Behaviour* pGreatestKnownThreatSetCondition    = CreateBehaviour(GreatestKnownThreatSetType, pEntity, "GreatestKnownThreatSetCondition", nullptr);
					Behaviour* pAttackSelector		               = CreateBehaviour(SelectorType, pEntity, "AttackSelector", nullptr);
						
					Behaviour* pDetermineGreatestSuspectedThreatAction   = CreateBehaviour(DetermineGreatestSuspectedThreatType, pEntity, "DetermineGreatestSuspectedThreatAction", nullptr);
					Behaviour* pGreatestSuspectedThreatSetCondition	     = CreateBehaviour(GreatestSuspectedThreatSetType, pEntity, "GreatestSuspectedThreatSetCondition", nullptr);
					Behaviour* pDetermineApproachThreatPositionAction    = CreateBehaviour(DetermineApproachThreatPositionType, pEntity, "DetermineApproachThreatPositionAction", nullptr);
					Behaviour* pMovementTargetApproachThreatSetCondition = CreateBehaviour(MovementTargetSetType, pEntity, "MovementTargetApproachThreatSetCondition", nullptr);
					
					ParallelInitData parallelInitData(ParallelPolicy::RequireAll, ParallelPolicy::RequireOne);
					Behaviour* pCheckInvestigatedThreatMonitor               = CreateBehaviour(MonitorType, pEntity, "CheckInvestigatedThreatMonitor", &parallelInitData);   
					Behaviour* pCheckInvestigatedTargetSequence              = CreateBehaviour(SequenceType, pEntity, "CheckInvestigatedTargetSequence", nullptr);
					Behaviour* pDetermineGreatestSuspectedThreatUpdateAction = CreateBehaviour(DetermineGreatestSuspectedThreatType, pEntity, "DetermineGreatestSuspectedThreatUpdateAction", nullptr);
					Behaviour* pStillInvestigatingGreatestThreatCondition    = CreateBehaviour(InvestigatingGreatestSuspectedThreatType, pEntity, "StillInvestigatingGreatestThreatCondition", nullptr);

					Behaviour* pMoveToApproachThreatTargetAction	 = CreateBehaviour(MoveToTargetType, pEntity, "MoveToApproachThreatTargetAction", nullptr);
					Behaviour* pResolveSuspectedThreatAction		 = CreateBehaviour(ResolveSuspectedThreatType, pEntity, "ResolveSuspectedThreatAction", nullptr);

					Behaviour* pDeterminePatrolTargetAction      = CreateBehaviour(DeterminePatrolTargetType, pEntity, "DeterminePatrolTargetAction", nullptr);
					Behaviour* pMovementTargetPatrolSetCondition = CreateBehaviour(MovementTargetSetType, pEntity, "MovementTargetPatrolSetCondition", nullptr);
					Behaviour* pMoveToPatrolTargetAction		 = CreateBehaviour(MoveToTargetType, pEntity, "MoveToPatrolTargetAction", nullptr);
		
					if(pDetermineGreatestKnownThreatAction && pGreatestKnownThreatSetCondition && pAttackSelector && pDetermineGreatestSuspectedThreatAction && pGreatestSuspectedThreatSetCondition &&
					   pDetermineApproachThreatPositionAction && pMovementTargetApproachThreatSetCondition && pCheckInvestigatedThreatMonitor && pCheckInvestigatedTargetSequence && pDetermineGreatestSuspectedThreatUpdateAction && 
					   pStillInvestigatingGreatestThreatCondition && pMoveToApproachThreatTargetAction && pResolveSuspectedThreatAction && pDeterminePatrolTargetAction && pMovementTargetPatrolSetCondition && pMoveToPatrolTargetAction)
					{
						reinterpret_cast<Composite*>(pFightSequence)->AddChild(pDetermineGreatestKnownThreatAction);
						reinterpret_cast<Composite*>(pFightSequence)->AddChild(pGreatestKnownThreatSetCondition);
						reinterpret_cast<Composite*>(pFightSequence)->AddChild(pAttackSelector);

						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pDetermineGreatestSuspectedThreatAction);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pGreatestSuspectedThreatSetCondition);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pDetermineApproachThreatPositionAction);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pMovementTargetApproachThreatSetCondition);
						
						// Alternatives
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pCheckInvestigatedThreatMonitor);
						reinterpret_cast<Monitor*>(pCheckInvestigatedThreatMonitor)->AddCondition(pCheckInvestigatedTargetSequence);
						reinterpret_cast<Composite*>(pCheckInvestigatedTargetSequence)->AddChild(pDetermineGreatestSuspectedThreatUpdateAction);
						reinterpret_cast<Composite*>(pCheckInvestigatedTargetSequence)->AddChild(pStillInvestigatingGreatestThreatCondition);
						
						reinterpret_cast<Monitor*>(pCheckInvestigatedThreatMonitor)->AddAction(pMoveToApproachThreatTargetAction);
						//reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pMoveToApproachThreatTargetAction);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pResolveSuspectedThreatAction);

						reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pDeterminePatrolTargetAction);
						reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pMovementTargetPatrolSetCondition);
						reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pMoveToPatrolTargetAction);

						Behaviour* pAttackSequence = CreateBehaviour(SequenceType, pEntity, "AttackSequence", nullptr);
						Behaviour* pWaitForAttackReadinessIdleAction = CreateBehaviour(IdleType, pEntity, "WaitForAttackReadinessIdleAction", nullptr);

						if(pAttackSequence && pWaitForAttackReadinessIdleAction)
						{
							reinterpret_cast<Composite*>(pAttackSelector)->AddChild(pAttackSequence);
							reinterpret_cast<Composite*>(pAttackSelector)->AddChild(pWaitForAttackReadinessIdleAction);

							Behaviour* pReadyToAttackCondition		    = CreateBehaviour(ReadyToAttackType, pEntity, "ReadyToAttackCondition", nullptr);
							Behaviour* pDetermineAttackTargetAction		= CreateBehaviour(DetermineAttackTargetType, pEntity, "DetermineAttackTargetAction", nullptr);
							Behaviour* pAttackTargetSetCondition		= CreateBehaviour(AttackTargetSetType, pEntity, "AttackTargetSetCondition", nullptr);
							Behaviour* pAimAtTargetAction				= CreateBehaviour(AimAtTargetType, pEntity, "AimAtTargetAction", nullptr);
							Behaviour* pAttackTargetAction				= CreateBehaviour(AttackTargetType, pEntity, "AttackTargetAction", nullptr);
	
							if(pReadyToAttackCondition && pDetermineAttackTargetAction && pAttackTargetSetCondition && 
							   pAimAtTargetAction && pAttackTargetAction)
							{
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pReadyToAttackCondition);
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pDetermineAttackTargetAction);
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pAttackTargetSetCondition);
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pAimAtTargetAction);
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pAttackTargetAction);

								return &(*pRoot);
							}
						}
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
	case EntityAliveType:
		return new EntityAlive(pEntity, name);
		break;
	case InvestigatingGreatestSuspectedThreatType:
		return new InvestigatingGreatestSuspectedThreat(pEntity, name);
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
	case DetermineGreatestKnownThreatType:
		return new DetermineGreatestKnownThreat(pEntity, name);
		break;
	case DetermineGreatestSuspectedThreatType:
		return new DetermineGreatestSuspectedThreat(pEntity, name);
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
	case ResolveSuspectedThreatType:
		return new ResolveSuspectedThreat(pEntity, name);
	case ProcessMessagesType:
		return new ProcessMessages(pEntity, name);
	default:
		return nullptr;
	}
}


