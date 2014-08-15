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
#include "MultiflagCTFTeamAI.h"



//--------------------------------------------------------------------------------------
// Creates a behaviour tree for the provided entity.
// Param1: Determines the type of the entity and thus, which behaviour tree will be created.
// Param2: A pointer to the object the behaviour of which should be controlled by the behaviour tree.
// Returns a pointer to the root behaviour of the created tree. The pointer is null if 
// something failed during creation or if the factory does not support the given entity
// type.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateBehaviourTree(BehaviourTreeType treeType, void* pTreeOwner)
{
	if(!pTreeOwner)
	{
		return nullptr;
	}

	switch(treeType)
	{
	case SimpleIndividualMovementTree:
		return CreateSimpleMovementTree(reinterpret_cast<Entity*>(pTreeOwner));
		break;
	case SimpleIndividualCombatTree:
		return CreateSimpleCombatTree(reinterpret_cast<Entity*>(pTreeOwner));
		break;
	case SimpleTeamMultiflagCTFTree:
		return CreateSimpleTeamMultiflagCTFTree(reinterpret_cast<MultiflagCTFTeamAI*>(pTreeOwner));
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
	Behaviour* pRoot = CreateUniversalBehaviour(SelectorType, "Root", nullptr);

	if(pRoot)
	{
		Behaviour* pPatrolSequence = CreateUniversalBehaviour(SequenceType, "PatrolSequence", nullptr);
		Behaviour* pIdleAction     = CreateUniversalIndividualBehaviour(IdleType, pEntity, "IdleAction", nullptr);
		
		if(pPatrolSequence && pIdleAction)
		{
			reinterpret_cast<Composite*>(pRoot)->AddChild(pPatrolSequence);
			reinterpret_cast<Composite*>(pRoot)->AddChild(pIdleAction);

			Behaviour* pDeterminePatrolTargetAction = CreateUniversalIndividualBehaviour(DeterminePatrolTargetType, pEntity, "DeterminePatrolTargetAction", nullptr);
			Behaviour* pMovementTargetSetCondition = CreateUniversalIndividualBehaviour(MovementTargetSetType, pEntity, "MovementTargetSetCondition", nullptr);
			Behaviour* pMoveToTargetAction = CreateUniversalIndividualBehaviour(MoveToTargetType, pEntity, "MoveToTargetAction", nullptr);
		
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
	Behaviour* pRoot = CreateUniversalBehaviour(ActiveSelectorType, "Root", nullptr);

	if(pRoot)
	{
		Behaviour* pUpdateEntitySequence = CreateUniversalBehaviour(SequenceType, "UpdateEntitySequence", nullptr);
		
		if(pUpdateEntitySequence)
		{
			ReturnSpecificStatusInitData data(pUpdateEntitySequence, StatusFailure);
			Behaviour* pAlwaysFailDecorator = CreateUniversalBehaviour(ReturnSpecificStatusType, "AlwaysFailDecorator", &data);

			Behaviour* pProcessMessagesAction		    = CreateUniversalIndividualBehaviour(ProcessMessagesType, pEntity, "ProcessMessagesAction", nullptr);
			Behaviour* pEntityAliveCondition		    = CreateUniversalIndividualBehaviour(EntityAliveType, pEntity, "EntityAliveCondition", nullptr);
			Behaviour* pUpdateThreatsAction				= CreateUniversalIndividualBehaviour(UpdateThreatsType, pEntity, "UpdateThreatsAction", nullptr);
			Behaviour* pUpdateAttackReadinessAction		= CreateUniversalIndividualBehaviour(UpdateAttackReadinessType, pEntity, "UpdateAttackReadinessAction", nullptr);

			if(pProcessMessagesAction && pAlwaysFailDecorator && pEntityAliveCondition && pUpdateThreatsAction && pUpdateAttackReadinessAction)
			{
				reinterpret_cast<Composite*>(pRoot)->AddChild(pAlwaysFailDecorator);

				reinterpret_cast<Composite*>(pUpdateEntitySequence)->AddChild(pProcessMessagesAction);
				reinterpret_cast<Composite*>(pUpdateEntitySequence)->AddChild(pEntityAliveCondition);
				reinterpret_cast<Composite*>(pUpdateEntitySequence)->AddChild(pUpdateThreatsAction);
				reinterpret_cast<Composite*>(pUpdateEntitySequence)->AddChild(pUpdateAttackReadinessAction);

				Behaviour* pFightSequence		   = CreateUniversalBehaviour(SequenceType, "FightSequence", nullptr);
				Behaviour* pApproachThreatSequence = CreateUniversalBehaviour(SequenceType, "ApproachThreatSequence", nullptr);
				Behaviour* pPatrolSequence		   = CreateUniversalBehaviour(SequenceType, "PatrolSequence", nullptr);
				Behaviour* pIdleAction			   = CreateUniversalIndividualBehaviour(IdleType, pEntity, "IdleAction", nullptr);

				if(pFightSequence && pApproachThreatSequence && pPatrolSequence && pIdleAction)
				{
					reinterpret_cast<Composite*>(pRoot)->AddChild(pFightSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pApproachThreatSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pPatrolSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pIdleAction);

					Behaviour* pDetermineGreatestKnownThreatAction = CreateUniversalIndividualBehaviour(DetermineGreatestKnownThreatType, pEntity, "DetermineGreatestKnownThreatAction", nullptr);
					Behaviour* pGreatestKnownThreatSetCondition    = CreateUniversalIndividualBehaviour(GreatestKnownThreatSetType, pEntity, "GreatestKnownThreatSetCondition", nullptr);
					Behaviour* pAttackSelector		               = CreateUniversalBehaviour(SelectorType, "AttackSelector", nullptr);
						
					Behaviour* pDetermineGreatestSuspectedThreatAction   = CreateUniversalIndividualBehaviour(DetermineGreatestSuspectedThreatType, pEntity, "DetermineGreatestSuspectedThreatAction", nullptr);
					Behaviour* pGreatestSuspectedThreatSetCondition	     = CreateUniversalIndividualBehaviour(GreatestSuspectedThreatSetType, pEntity, "GreatestSuspectedThreatSetCondition", nullptr);
					Behaviour* pDetermineApproachThreatPositionAction    = CreateUniversalIndividualBehaviour(DetermineApproachThreatPositionType, pEntity, "DetermineApproachThreatPositionAction", nullptr);
					Behaviour* pMovementTargetApproachThreatSetCondition = CreateUniversalIndividualBehaviour(MovementTargetSetType, pEntity, "MovementTargetApproachThreatSetCondition", nullptr);
					
					ParallelInitData parallelInitData(ParallelPolicy::RequireAll, ParallelPolicy::RequireOne);
					Behaviour* pCheckInvestigatedThreatMonitor               = CreateUniversalBehaviour(MonitorType, "CheckInvestigatedThreatMonitor", &parallelInitData);   
					Behaviour* pCheckInvestigatedTargetSequence              = CreateUniversalBehaviour(SequenceType, "CheckInvestigatedTargetSequence", nullptr);
					Behaviour* pDetermineGreatestSuspectedThreatUpdateAction = CreateUniversalIndividualBehaviour(DetermineGreatestSuspectedThreatType, pEntity, "DetermineGreatestSuspectedThreatUpdateAction", nullptr);
					Behaviour* pStillInvestigatingGreatestThreatCondition    = CreateUniversalIndividualBehaviour(InvestigatingGreatestSuspectedThreatType, pEntity, "StillInvestigatingGreatestThreatCondition", nullptr);

					Behaviour* pMoveToApproachThreatTargetAction	 = CreateUniversalIndividualBehaviour(MoveToTargetType, pEntity, "MoveToApproachThreatTargetAction", nullptr);
					Behaviour* pResolveSuspectedThreatAction		 = CreateUniversalIndividualBehaviour(ResolveSuspectedThreatType, pEntity, "ResolveSuspectedThreatAction", nullptr);

					Behaviour* pDeterminePatrolTargetAction      = CreateUniversalIndividualBehaviour(DeterminePatrolTargetType, pEntity, "DeterminePatrolTargetAction", nullptr);
					Behaviour* pMovementTargetPatrolSetCondition = CreateUniversalIndividualBehaviour(MovementTargetSetType, pEntity, "MovementTargetPatrolSetCondition", nullptr);
					Behaviour* pMoveToPatrolTargetAction		 = CreateUniversalIndividualBehaviour(MoveToTargetType, pEntity, "MoveToPatrolTargetAction", nullptr);
		
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

						Behaviour* pAttackSequence = CreateUniversalBehaviour(SequenceType, "AttackSequence", nullptr);
						Behaviour* pWaitForAttackReadinessIdleAction = CreateUniversalIndividualBehaviour(IdleType, pEntity, "WaitForAttackReadinessIdleAction", nullptr);

						if(pAttackSequence && pWaitForAttackReadinessIdleAction)
						{
							reinterpret_cast<Composite*>(pAttackSelector)->AddChild(pAttackSequence);
							reinterpret_cast<Composite*>(pAttackSelector)->AddChild(pWaitForAttackReadinessIdleAction);

							Behaviour* pReadyToAttackCondition		    = CreateUniversalIndividualBehaviour(ReadyToAttackType, pEntity, "ReadyToAttackCondition", nullptr);
							Behaviour* pDetermineAttackTargetAction		= CreateUniversalIndividualBehaviour(DetermineAttackTargetType, pEntity, "DetermineAttackTargetAction", nullptr);
							Behaviour* pAttackTargetSetCondition		= CreateUniversalIndividualBehaviour(AttackTargetSetType, pEntity, "AttackTargetSetCondition", nullptr);
							Behaviour* pAimAtTargetAction				= CreateUniversalIndividualBehaviour(AimAtTargetType, pEntity, "AimAtTargetAction", nullptr);
							Behaviour* pAttackTargetAction				= CreateUniversalIndividualBehaviour(AttackTargetType, pEntity, "AttackTargetAction", nullptr);
	
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
// Creates a simple behaviour tree for a team AI controlling entities into playing
// multiflag CTF matches.
// Param1: A pointer to the multiflag CTF team AI that the behaviour tree should be created for.
// Returns a pointer to the root behaviour of the created tree. The pointer is null if 
// something failed during creation.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateSimpleTeamMultiflagCTFTree(MultiflagCTFTeamAI* pTeamAI)
{
	Behaviour* pTeamRoot = CreateUniversalBehaviour(ActiveSelectorType, "TeamRoot", nullptr);

	if(pTeamRoot)
	{
		Behaviour* pTeamProcessMessagesAction = CreateUniversalTeamBehaviour(TeamProcessMessagesType, pTeamAI, "TeamProcessMessagesAction", nullptr);

		if(pTeamProcessMessagesAction)
		{
			reinterpret_cast<Composite*>(pTeamRoot)->AddChild(pTeamProcessMessagesAction);
			return pTeamRoot;
		}
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------
// Creates a certain universal behaviour.
// Param1: Determines which behaviour will be created from a range of available types.
// Param2: The name that will be associated to the newly created behaviour.
// Param3: A pointer to a data structure holding the initialisation data for some behaviours.
// Returns a pointer to the newly created behaviour. The pointer is null if something failed 
// during creation or if the factory does not support the given behaviour type.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateUniversalBehaviour(UniversalBehaviourType behaviourType, const char* name, void* pInitData)
{
	switch(behaviourType)
	{
	case SelectorType:
		return new Selector(name);
		break;
	case ActiveSelectorType:
		return new ActiveSelector(name);
		break;
	case SequenceType:
		return new Sequence(name);
		break;
	case ParallelType:
		{
		ParallelInitData* pData = reinterpret_cast<ParallelInitData*>(pInitData);
		return new Parallel(name, pData->m_successPolicy, pData->m_failurePolicy);
		break;
		}
	case MonitorType:
		return new Monitor(name);
		break;
	case RepeatType:
		{
		RepeatInitData* pData = reinterpret_cast<RepeatInitData*>(pInitData);
		return new Repeat(name, pData->m_pChild, pData->m_numberOfRepeats);
		break;
		}
	case ReturnSpecificStatusType:
		{
		ReturnSpecificStatusInitData* pData = reinterpret_cast<ReturnSpecificStatusInitData*>(pInitData);
		return new ReturnSpecificStatus(name, pData->m_pChild, pData->m_returnStatus);
		break;
		}
	default:
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Creates a certain universal individual behaviour for the provided entity.
// Param1: Determines which behaviour will be created from a range of available types.
// Param2: A pointer to the entity that the behaviour tree should be created for.
// Param3: The name that will be associated to the newly created behaviour.
// Param4: A pointer to a data structure holding the initialisation data for some behaviours.
// Returns a pointer to the newly created behaviour. The pointer is null if something failed 
// during creation or if the factory does not support the given behaviour type.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateUniversalIndividualBehaviour(UniversalIndividualBehaviourType behaviourType, Entity* pEntity, const char* name, void* pInitData)
{
	switch(behaviourType)
	{
	case MovementTargetSetType:
		return new MovementTargetSet(name, pEntity);
		break;
	case ReadyToAttackType:
		return new ReadyToAttack(name, pEntity);
		break;
	case AttackTargetSetType:
		return new AttackTargetSet(name, pEntity);
		break;
	case GreatestKnownThreatSetType:
		return new GreatestKnownThreatSet(name, pEntity);
		break;
	case GreatestSuspectedThreatSetType:
		return new GreatestSuspectedThreatSet(name, pEntity);
		break;
	case EntityAliveType:
		return new EntityAlive(name, pEntity);
		break;
	case InvestigatingGreatestSuspectedThreatType:
		return new InvestigatingGreatestSuspectedThreat(name, pEntity);
		break;
	case DeterminePatrolTargetType:
		return new DeterminePatrolTarget(name, pEntity);
		break;
	case DetermineApproachThreatPositionType:
		return new DetermineApproachThreatPosition(name, pEntity);
		break;
	case DetermineAttackTargetType:
		return new DetermineAttackTarget(name, pEntity);
		break;
	case DetermineGreatestKnownThreatType:
		return new DetermineGreatestKnownThreat(name, pEntity);
		break;
	case DetermineGreatestSuspectedThreatType:
		return new DetermineGreatestSuspectedThreat(name, pEntity);
		break;
	case AttackTargetType:
		return new AttackTarget(name, pEntity);
		break;
	case AimAtTargetType:
		return new AimAtTarget(name, pEntity);
		break;
	case UpdateThreatsType:
		return new UpdateThreats(name, pEntity);
		break;
	case UpdateAttackReadinessType:
		return new UpdateAttackReadiness(name, pEntity);
		break;
	case IdleType:
		return new Idle(name, pEntity);
		break;
	case MoveToTargetType:
		return new MoveToTarget(name, pEntity);
		break;
	case ResolveSuspectedThreatType:
		return new ResolveSuspectedThreat(name, pEntity);
	case ProcessMessagesType:
		return new ProcessMessages(name, pEntity);
	default:
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Creates a certain universal team behaviour for the provided team AI.
// Param1: Determines which behaviour will be created from a range of available types.
// Param2: A pointer to the team AI that the behaviour tree should be created for.
// Param3: The name that will be associated to the newly created behaviour.
// Param4: A pointer to a data structure holding the initialisation data for some behaviours.
// Returns a pointer to the newly created behaviour. The pointer is null if something failed 
// during creation or if the factory does not support the given behaviour type.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateUniversalTeamBehaviour(UniversalTeamBehaviourType behaviourType, TeamAI* pTeamAI, const char* name, void* pInitData)
{
	switch(behaviourType)
	{
	case TeamProcessMessagesType:
		return new TeamProcessMessages(name, pTeamAI);
		break;
	default:
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Creates a certain multiflag CTF team behaviour for the provided multiflag CTF team AI.
// Param1: Determines which behaviour will be created from a range of available types.
// Param2: A pointer to the multiflag CTF team AI that the behaviour tree should be created for.
// Param3: The name that will be associated to the newly created behaviour.
// Param4: A pointer to a data structure holding the initialisation data for some behaviours.
// Returns a pointer to the newly created behaviour. The pointer is null if something failed 
// during creation or if the factory does not support the given behaviour type.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateMultiflagCTFTeamBehaviour(MultiflagCTFTeamBehaviourType behaviourType, MultiflagCTFTeamAI* pMultiflagCTFTeamAI, const char* name, void* pInitData)
{
	switch(behaviourType)
	{
	default:
		return nullptr;
	}
}
