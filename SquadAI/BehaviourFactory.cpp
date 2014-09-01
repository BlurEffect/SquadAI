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
	case ModifiedSimpleIndividualCombatTree:
		return CreateModifiedSimpleCombatTree(reinterpret_cast<Entity*>(pTreeOwner));
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

			Behaviour* pDetermineMovementTargetAction = CreateUniversalIndividualBehaviour(DetermineMovementTargetType, pEntity, "DetermineMovementTargetAction", nullptr);
			Behaviour* pMovementTargetSetCondition = CreateUniversalIndividualBehaviour(MovementTargetSetType, pEntity, "MovementTargetSetCondition", nullptr);
			Behaviour* pMoveToTargetAction = CreateUniversalIndividualBehaviour(MoveToTargetType, pEntity, "MoveToTargetAction", nullptr);
		
			if(pDetermineMovementTargetAction && pMovementTargetSetCondition && pMoveToTargetAction)
			{
				reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pDetermineMovementTargetAction);
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

					Behaviour* pDetermineMovementTargetAction      = CreateUniversalIndividualBehaviour(DetermineMovementTargetType, pEntity, "DetermineMovementTargetAction", nullptr);
					Behaviour* pMovementTargetPatrolSetCondition = CreateUniversalIndividualBehaviour(MovementTargetSetType, pEntity, "MovementTargetPatrolSetCondition", nullptr);
					Behaviour* pMoveToPatrolTargetAction		 = CreateUniversalIndividualBehaviour(MoveToTargetType, pEntity, "MoveToPatrolTargetAction", nullptr);
		
					if(pDetermineGreatestKnownThreatAction && pGreatestKnownThreatSetCondition && pAttackSelector && pDetermineGreatestSuspectedThreatAction && pGreatestSuspectedThreatSetCondition &&
					   pDetermineApproachThreatPositionAction && pMovementTargetApproachThreatSetCondition && pCheckInvestigatedThreatMonitor && pCheckInvestigatedTargetSequence && pDetermineGreatestSuspectedThreatUpdateAction && 
					   pStillInvestigatingGreatestThreatCondition && pMoveToApproachThreatTargetAction && pResolveSuspectedThreatAction && pDetermineMovementTargetAction && pMovementTargetPatrolSetCondition && pMoveToPatrolTargetAction)
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

						reinterpret_cast<Composite*>(pPatrolSequence)->AddChild(pDetermineMovementTargetAction);
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
// Similar to SimpleCombatTree but modified to allow for better order processing.
// Param1: A pointer to the entity that the behaviour tree should be created for.
// Returns a pointer to the root behaviour of the created tree. The pointer is null if 
// something failed during creation.
//--------------------------------------------------------------------------------------
Behaviour* BehaviourFactory::CreateModifiedSimpleCombatTree(Entity* pEntity)
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
				Behaviour* pMovementSequence		   = CreateUniversalBehaviour(SequenceType, "MovementSequence", nullptr);
				Behaviour* pHoldPositionSelector  = CreateUniversalBehaviour(SelectorType, "HoldPositionSelector", nullptr);
				
				if(pFightSequence && pApproachThreatSequence && pMovementSequence && pHoldPositionSelector)
				{
					reinterpret_cast<Composite*>(pRoot)->AddChild(pFightSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pApproachThreatSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pMovementSequence);
					reinterpret_cast<Composite*>(pRoot)->AddChild(pHoldPositionSelector);


					Behaviour* pDetermineGreatestKnownThreatAction = CreateUniversalIndividualBehaviour(DetermineGreatestKnownThreatType, pEntity, "DetermineGreatestKnownThreatAction", nullptr);
					Behaviour* pGreatestKnownThreatSetCondition    = CreateUniversalIndividualBehaviour(GreatestKnownThreatSetType, pEntity, "GreatestKnownThreatSetCondition", nullptr);
					Behaviour* pAttackSelector		               = CreateUniversalBehaviour(SelectorType, "AttackSelector", nullptr);
						
					Behaviour* pDetermineGreatestSuspectedThreatAction   = CreateUniversalIndividualBehaviour(DetermineGreatestSuspectedThreatType, pEntity, "DetermineGreatestSuspectedThreatAction", nullptr);
					Behaviour* pGreatestSuspectedThreatSetCondition	     = CreateUniversalIndividualBehaviour(GreatestSuspectedThreatSetType, pEntity, "GreatestSuspectedThreatSetCondition", nullptr);
					Behaviour* pDetermineApproachThreatPositionAction    = CreateUniversalIndividualBehaviour(DetermineApproachThreatPositionType, pEntity, "DetermineApproachThreatPositionAction", nullptr);
					Behaviour* pMovementTargetApproachThreatSetCondition = CreateUniversalIndividualBehaviour(MovementTargetSetType, pEntity, "MovementTargetApproachThreatSetCondition", nullptr);
					
					Behaviour* pDeterminePathToSuspectedThreatAction     = CreateUniversalIndividualBehaviour(DeterminePathToTargetType, pEntity, "DeterminePathToSuspectedThreatAction", nullptr);
					Behaviour* pPathToSuspectedThreatSetCondition        = CreateUniversalIndividualBehaviour(PathToTargetSetType, pEntity, "PathToSuspectedThreatSetCondition", nullptr);

					ParallelInitData parallelInitData(ParallelPolicy::RequireAll, ParallelPolicy::RequireOne);
					Behaviour* pCheckInvestigatedThreatMonitor               = CreateUniversalBehaviour(MonitorType, "CheckInvestigatedThreatMonitor", &parallelInitData);   
					Behaviour* pCheckInvestigatedTargetSequence              = CreateUniversalBehaviour(SequenceType, "CheckInvestigatedTargetSequence", nullptr);
					Behaviour* pDetermineGreatestSuspectedThreatUpdateAction = CreateUniversalIndividualBehaviour(DetermineGreatestSuspectedThreatType, pEntity, "DetermineGreatestSuspectedThreatUpdateAction", nullptr);
					Behaviour* pStillInvestigatingGreatestThreatCondition    = CreateUniversalIndividualBehaviour(InvestigatingGreatestSuspectedThreatType, pEntity, "StillInvestigatingGreatestThreatCondition", nullptr);

					Behaviour* pMoveToApproachThreatTargetAction	 = CreateUniversalIndividualBehaviour(MoveToTargetType, pEntity, "MoveToApproachThreatTargetAction", nullptr);
					Behaviour* pResolveSuspectedThreatAction		 = CreateUniversalIndividualBehaviour(ResolveSuspectedThreatType, pEntity, "ResolveSuspectedThreatAction", nullptr);

					Behaviour* pDetermineMovementTargetAction      = CreateUniversalIndividualBehaviour(DetermineMovementTargetType, pEntity, "DetermineMovementTargetAction", nullptr);
					Behaviour* pMovementTargetSetCondition = CreateUniversalIndividualBehaviour(MovementTargetSetType, pEntity, "MovementTargetSetCondition", nullptr);
					
					Behaviour* pDeterminePathToMovementTargetAction = CreateUniversalIndividualBehaviour(DeterminePathToTargetType, pEntity, "DeterminePathToMovementTargetAction", nullptr);
					Behaviour* pPathToMovementTargetSetCondition = CreateUniversalIndividualBehaviour(PathToTargetSetType, pEntity, "PathToMovementTargetSetCondition", nullptr);

					ParallelInitData parallelInitData2(ParallelPolicy::RequireAll, ParallelPolicy::RequireOne);
					Behaviour* pCheckMovementTargetMonitor               = CreateUniversalBehaviour(MonitorType, "CheckMovementTargetMonitor ", &parallelInitData2);   
					Behaviour* pMovingToHighestPriorityTargetCondition		 = CreateUniversalIndividualBehaviour(MovingToHighestPriorityTargetType, pEntity, "MovingToHighestPriorityTargetCondition", nullptr);
					Behaviour* pMoveToTargetAction		 = CreateUniversalIndividualBehaviour(MoveToTargetType, pEntity, "MoveToTargetAction", nullptr);

					Behaviour* pFinaliseMovementAction		 = CreateUniversalIndividualBehaviour(FinaliseMovementType, pEntity, "FinaliseMovementAction", nullptr);


					Behaviour* pChangeObservationTargetSequence = CreateUniversalBehaviour(SequenceType, "ChangeObservationTargetSequence", nullptr);
					Behaviour* pIdleAction			   = CreateUniversalIndividualBehaviour(IdleType, pEntity, "IdleAction", nullptr);


					if(pDetermineGreatestKnownThreatAction && pGreatestKnownThreatSetCondition && pAttackSelector && pDetermineGreatestSuspectedThreatAction && pGreatestSuspectedThreatSetCondition &&
					   pDetermineApproachThreatPositionAction && pMovementTargetApproachThreatSetCondition && pDeterminePathToSuspectedThreatAction && pPathToSuspectedThreatSetCondition && pCheckInvestigatedThreatMonitor && pCheckInvestigatedTargetSequence && pDetermineGreatestSuspectedThreatUpdateAction && 
					   pStillInvestigatingGreatestThreatCondition && pMoveToApproachThreatTargetAction && pResolveSuspectedThreatAction && pDetermineMovementTargetAction && pMovementTargetSetCondition && pDeterminePathToMovementTargetAction && pPathToMovementTargetSetCondition && pCheckMovementTargetMonitor 
					   && pMovingToHighestPriorityTargetCondition && pMoveToTargetAction && pFinaliseMovementAction && pChangeObservationTargetSequence && pIdleAction)
					{
						reinterpret_cast<Composite*>(pFightSequence)->AddChild(pDetermineGreatestKnownThreatAction);
						reinterpret_cast<Composite*>(pFightSequence)->AddChild(pGreatestKnownThreatSetCondition);
						reinterpret_cast<Composite*>(pFightSequence)->AddChild(pAttackSelector);

						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pDetermineGreatestSuspectedThreatAction);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pGreatestSuspectedThreatSetCondition);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pDetermineApproachThreatPositionAction);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pMovementTargetApproachThreatSetCondition);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pDeterminePathToSuspectedThreatAction);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pPathToSuspectedThreatSetCondition);

						// Alternatives
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pCheckInvestigatedThreatMonitor);
						reinterpret_cast<Monitor*>(pCheckInvestigatedThreatMonitor)->AddCondition(pCheckInvestigatedTargetSequence);
						reinterpret_cast<Composite*>(pCheckInvestigatedTargetSequence)->AddChild(pDetermineGreatestSuspectedThreatUpdateAction);
						reinterpret_cast<Composite*>(pCheckInvestigatedTargetSequence)->AddChild(pStillInvestigatingGreatestThreatCondition);
						
						reinterpret_cast<Monitor*>(pCheckInvestigatedThreatMonitor)->AddAction(pMoveToApproachThreatTargetAction);
						//reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pMoveToApproachThreatTargetAction);
						reinterpret_cast<Composite*>(pApproachThreatSequence)->AddChild(pResolveSuspectedThreatAction);

						reinterpret_cast<Composite*>(pMovementSequence)->AddChild(pDetermineMovementTargetAction);
						reinterpret_cast<Composite*>(pMovementSequence)->AddChild(pMovementTargetSetCondition);
						reinterpret_cast<Composite*>(pMovementSequence)->AddChild(pDeterminePathToMovementTargetAction);
						reinterpret_cast<Composite*>(pMovementSequence)->AddChild(pPathToMovementTargetSetCondition);

						reinterpret_cast<Composite*>(pMovementSequence)->AddChild(pCheckMovementTargetMonitor);
						
						reinterpret_cast<Monitor*>(pCheckMovementTargetMonitor)->AddCondition(pMovingToHighestPriorityTargetCondition);
						reinterpret_cast<Monitor*>(pCheckMovementTargetMonitor)->AddAction(pMoveToTargetAction);

						reinterpret_cast<Composite*>(pMovementSequence)->AddChild(pFinaliseMovementAction);
						
						reinterpret_cast<Composite*>(pHoldPositionSelector)->AddChild(pChangeObservationTargetSequence);
						reinterpret_cast<Composite*>(pHoldPositionSelector)->AddChild(pIdleAction);
							
					 

						Behaviour* pAttackSequence = CreateUniversalBehaviour(SequenceType, "AttackSequence", nullptr);
						Behaviour* pWaitForAttackReadinessIdleAction = CreateUniversalIndividualBehaviour(IdleType, pEntity, "WaitForAttackReadinessIdleAction", nullptr);

						Behaviour* pDetermineObservationTargetAction = CreateUniversalIndividualBehaviour(DetermineObservationTargetType, pEntity, "DetermineObservationTargetAction", nullptr);
						Behaviour* pObservationTargetSetCondition = CreateUniversalIndividualBehaviour(ObservationTargetSetType, pEntity, "ObservationTargetSetCondition", nullptr);
						Behaviour* pLookAtObservationTargetAction = CreateUniversalIndividualBehaviour(LookAtTargetType, pEntity, "LookAtObservationTargetAction", nullptr);

						if(pAttackSequence && pWaitForAttackReadinessIdleAction && pDetermineObservationTargetAction && pObservationTargetSetCondition && pLookAtObservationTargetAction)
						{
							reinterpret_cast<Composite*>(pAttackSelector)->AddChild(pAttackSequence);
							reinterpret_cast<Composite*>(pAttackSelector)->AddChild(pWaitForAttackReadinessIdleAction);

							reinterpret_cast<Composite*>(pChangeObservationTargetSequence)->AddChild(pDetermineObservationTargetAction);
							reinterpret_cast<Composite*>(pChangeObservationTargetSequence)->AddChild(pObservationTargetSetCondition);
							reinterpret_cast<Composite*>(pChangeObservationTargetSequence)->AddChild(pLookAtObservationTargetAction);

							Behaviour* pReadyToAttackCondition		    = CreateUniversalIndividualBehaviour(ReadyToAttackType, pEntity, "ReadyToAttackCondition", nullptr);
							Behaviour* pDetermineAttackTargetAction		= CreateUniversalIndividualBehaviour(DetermineAttackTargetType, pEntity, "DetermineAttackTargetAction", nullptr);
							Behaviour* pAttackTargetSetCondition		= CreateUniversalIndividualBehaviour(AttackTargetSetType, pEntity, "AttackTargetSetCondition", nullptr);
							Behaviour* pAimAtTargetAction				= CreateUniversalIndividualBehaviour(AimAtTargetType, pEntity, "AimAtTargetAction", nullptr);
							Behaviour* pAttackTargetAction				= CreateUniversalIndividualBehaviour(AttackTargetType, pEntity, "AttackTargetAction", nullptr);
							
							if(pReadyToAttackCondition && pDetermineAttackTargetAction && pAttackTargetSetCondition && 
							   pAimAtTargetAction && pAttackTargetAction)
							{
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pDetermineAttackTargetAction);
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pAttackTargetSetCondition);
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pAimAtTargetAction);
								reinterpret_cast<Composite*>(pAttackSequence)->AddChild(pReadyToAttackCondition);
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
Behaviour* BehaviourFactory::CreateSimpleTeamMultiflagCTFTree(TeamAI* pTeamAI)
{
	TeamParallelInitData rootInitData(TeamRequireAll, TeamRequireAll);
	TeamBehaviour* pTeamParallelRoot = CreateParentTeamBehaviour(TeamParallelType, pTeamAI, "TeamParallelRoot", &rootInitData);

	if(pTeamParallelRoot)
	{
		TeamBehaviour* pUpdateTeamAISequence = CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "UpdateTeamAISequence", nullptr);
		
		TeamParallelInitData manoeuvreCategoriesInitData(TeamRequireOne, TeamRequireAll);
		TeamBehaviour* pManoeuvreCategoriesParallel = CreateParentTeamBehaviour(TeamParallelType, pTeamAI, "ManoeuvreCategoriesParallel", &manoeuvreCategoriesInitData);

		if(pUpdateTeamAISequence && pManoeuvreCategoriesParallel)
		{
			reinterpret_cast<TeamComposite*>(pTeamParallelRoot)->AddChild(pUpdateTeamAISequence);
			reinterpret_cast<TeamComposite*>(pTeamParallelRoot)->AddChild(pManoeuvreCategoriesParallel);
			
			TeamBehaviour* pTeamProcessMessagesAction = CreatePrimitiveTeamBehaviour(TeamProcessMessagesType, pTeamAI, "TeamProcessMessagesAction", 1.0f, 1.0f, nullptr);

			// The different manoeuvre categories
			TeamBehaviour* pAttackCharacteristicSelector = CreateParentTeamBehaviour(TeamActiveCharacteristicSelectorType, pTeamAI, "AttackCharacteristicSelector", nullptr);
			TeamBehaviour* pDefendCharacteristicSelector = CreateParentTeamBehaviour(TeamActiveCharacteristicSelectorType, pTeamAI, "DefendCharacteristicSelector", nullptr);

			if(pTeamProcessMessagesAction && pAttackCharacteristicSelector && pDefendCharacteristicSelector)
			{
				reinterpret_cast<TeamComposite*>(pUpdateTeamAISequence)->AddChild(pTeamProcessMessagesAction);

				reinterpret_cast<TeamComposite*>(pManoeuvreCategoriesParallel)->AddChild(pAttackCharacteristicSelector);
				reinterpret_cast<TeamComposite*>(pManoeuvreCategoriesParallel)->AddChild(pDefendCharacteristicSelector);

				// These sequences are the "root" nodes of each concrete manoeuvre
				TeamBehaviour* pRunTheFlagHomeSequence			= CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "RunTheFlagHomeSequence", nullptr);
				TeamBehaviour* pRushBaseAttackSequence			= CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "RushBaseAttackSequence", nullptr);
				TeamBehaviour* pCoordinatedBaseAttackSequence	= CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "CoordinatedBaseAttackSequence", nullptr);
				TeamBehaviour* pDistractionBaseAttackSequence	= CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "DistractionBaseAttackSequence", nullptr);
				TeamBehaviour* pDefendBaseEntrancesSequence		= CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "DefendBaseEntrancesSequence", nullptr);
				TeamBehaviour* pReturnDroppedFlagSequence		= CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "ReturnDroppedFlagSequence", nullptr);

				if(pRunTheFlagHomeSequence && pRushBaseAttackSequence && pCoordinatedBaseAttackSequence && pDistractionBaseAttackSequence && pDefendBaseEntrancesSequence && pReturnDroppedFlagSequence)
				{
					reinterpret_cast<TeamComposite*>(pAttackCharacteristicSelector)->AddChild(pRunTheFlagHomeSequence);
					reinterpret_cast<TeamComposite*>(pAttackCharacteristicSelector)->AddChild(pRushBaseAttackSequence);
					reinterpret_cast<TeamComposite*>(pAttackCharacteristicSelector)->AddChild(pCoordinatedBaseAttackSequence);
					reinterpret_cast<TeamComposite*>(pAttackCharacteristicSelector)->AddChild(pDistractionBaseAttackSequence);
					reinterpret_cast<TeamComposite*>(pDefendCharacteristicSelector)->AddChild(pDefendBaseEntrancesSequence);
					reinterpret_cast<TeamComposite*>(pDefendCharacteristicSelector)->AddChild(pReturnDroppedFlagSequence);

					// The precondition checks for each of the manoeuvres
					ManoeuvrePreconditionsFulfilledInitData runTheFlagHomeInitData(RunTheFlagHomeManoeuvre);
					TeamBehaviour* pRunTheFlagHomePreconditionsCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvrePreconditionsFulfilledType, pTeamAI, "RunTheFlagHomePreconditionsCheck", 0.0f, 0.0f, &runTheFlagHomeInitData);
					ManoeuvrePreconditionsFulfilledInitData rushBaseAttackInitData(RushBaseAttackManoeuvre);
					TeamBehaviour* pRushBaseAttackPreconditionsCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvrePreconditionsFulfilledType, pTeamAI, "RushBaseAttackPreconditionsCheck", 0.0f, 0.0f, &rushBaseAttackInitData);
					ManoeuvrePreconditionsFulfilledInitData coordinatedBaseAttackInitData(CoordinatedBaseAttackManoeuvre);
					TeamBehaviour* pCoordinatedBaseAttackPreconditionsCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvrePreconditionsFulfilledType, pTeamAI, "CoordinatedBaseAttackPreconditionsCheck", 0.0f, 0.0f, &coordinatedBaseAttackInitData);
					ManoeuvrePreconditionsFulfilledInitData distractionBaseAttackInitData(DistractionBaseAttackManoeuvre);
					TeamBehaviour* pDistractionBaseAttackPreconditionsCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvrePreconditionsFulfilledType, pTeamAI, "DistractionBaseAttackPreconditionsCheck", 0.0f, 0.0f, &distractionBaseAttackInitData);
					ManoeuvrePreconditionsFulfilledInitData defendBaseEntrancesInitData(DefendBaseEntrancesManoeuvre);
					TeamBehaviour* pDefendBaseEntrancesPreconditionsCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvrePreconditionsFulfilledType, pTeamAI, "DefendBaseEntrancesPreconditionsCheck", 0.0f, 0.0f, &defendBaseEntrancesInitData);
					ManoeuvrePreconditionsFulfilledInitData returnDroppedFlagInitData(ReturnDroppedFlagManoeuvre);
					TeamBehaviour* pReturnDroppedFlagPreconditionsCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvrePreconditionsFulfilledType, pTeamAI, "ReturnDroppedFlagPreconditionsCheck", 0.0f, 0.0f, &returnDroppedFlagInitData);

					// The manoeuvre initiation actions that have to be executed before beginning the execution of the actual behaviours
					InitiateTeamManoeuvreInitData initiateRunTheFlagHomeInitData(RunTheFlagHomeManoeuvre);
					TeamBehaviour* pRunTheFlagHomeInitiationAction = CreatePrimitiveTeamBehaviour(TeamInitiateManoeuvreType, pTeamAI, "RunTheFlagHomeInitiationAction", 0.0f, 0.0f, &initiateRunTheFlagHomeInitData);
					InitiateTeamManoeuvreInitData initiateRushBaseAttackInitData(RushBaseAttackManoeuvre);
					TeamBehaviour* pRushBaseAttackInitiationAction = CreatePrimitiveTeamBehaviour(TeamInitiateManoeuvreType, pTeamAI, "RushBaseAttackInitiationAction", 0.0f, 0.0f, &initiateRushBaseAttackInitData);
					InitiateTeamManoeuvreInitData initiateCoordinatedBaseAttackInitData(CoordinatedBaseAttackManoeuvre);
					TeamBehaviour* pCoordinatedBaseAttackInitiationAction = CreatePrimitiveTeamBehaviour(TeamInitiateManoeuvreType, pTeamAI, "CoordinatedBaseAttackInitiationAction", 0.0f, 0.0f, &initiateCoordinatedBaseAttackInitData);
					InitiateTeamManoeuvreInitData initiateDistractionBaseAttackInitData(DistractionBaseAttackManoeuvre);
					TeamBehaviour* pDistractionBaseAttackInitiationAction = CreatePrimitiveTeamBehaviour(TeamInitiateManoeuvreType, pTeamAI, "DistractionBaseAttackInitiationAction", 0.0f, 0.0f, &initiateDistractionBaseAttackInitData);
					InitiateTeamManoeuvreInitData initiateDefendBaseEntrancesInitData(DefendBaseEntrancesManoeuvre);
					TeamBehaviour* pDefendBaseEntrancesInitiationAction = CreatePrimitiveTeamBehaviour(TeamInitiateManoeuvreType, pTeamAI, "DefendBaseEntrancesInitiationAction", 0.0f, 0.0f, &initiateDefendBaseEntrancesInitData);
					InitiateTeamManoeuvreInitData initiateReturnDroppedFlagInitData(ReturnDroppedFlagManoeuvre);
					TeamBehaviour* pReturnDroppedFlagInitiationAction = CreatePrimitiveTeamBehaviour(TeamInitiateManoeuvreType, pTeamAI, "ReturnDroppedFlagInitiationAction", 0.0f, 0.0f, &initiateReturnDroppedFlagInitData);


					// The monitors constantly checking if the manoeuvres are still valid or should be aborted
					TeamBehaviour* pRunTheFlagHomeMonitor			= CreateParentTeamBehaviour(TeamMonitorType, pTeamAI, "RunTheFlagHomeMonitor", nullptr);
					TeamBehaviour* pRushBaseAttackMonitor			= CreateParentTeamBehaviour(TeamMonitorType, pTeamAI, "RushBaseAttackMonitor", nullptr);
					TeamBehaviour* pCoordinatedBaseAttackMonitor	= CreateParentTeamBehaviour(TeamMonitorType, pTeamAI, "CoordinatedBaseAttackMonitor", nullptr);
					TeamBehaviour* pDistractionBaseAttackMonitor	= CreateParentTeamBehaviour(TeamMonitorType, pTeamAI, "DistractionBaseAttackMonitor", nullptr);
					TeamBehaviour* pDefendBaseEntrancesMonitor		= CreateParentTeamBehaviour(TeamMonitorType, pTeamAI, "DefendBaseEntrancesMonitor", nullptr);
					TeamBehaviour* pReturnDroppedFlagMonitor		= CreateParentTeamBehaviour(TeamMonitorType, pTeamAI, "ReturnDroppedFlagMonitor", nullptr);


					if(pRunTheFlagHomePreconditionsCheck && pRushBaseAttackPreconditionsCheck && pCoordinatedBaseAttackPreconditionsCheck && pDistractionBaseAttackPreconditionsCheck && pDefendBaseEntrancesPreconditionsCheck && pReturnDroppedFlagPreconditionsCheck &&
					   pRunTheFlagHomeInitiationAction && pRushBaseAttackInitiationAction && pCoordinatedBaseAttackInitiationAction && pDistractionBaseAttackInitiationAction && pDefendBaseEntrancesInitiationAction && pReturnDroppedFlagInitiationAction &&
					   pRunTheFlagHomeMonitor && pRushBaseAttackMonitor && pCoordinatedBaseAttackMonitor && pDistractionBaseAttackMonitor && pDefendBaseEntrancesMonitor && pReturnDroppedFlagMonitor)
					{
						reinterpret_cast<TeamComposite*>(pRunTheFlagHomeSequence)->AddChild(pRunTheFlagHomePreconditionsCheck);
						reinterpret_cast<TeamComposite*>(pRunTheFlagHomeSequence)->AddChild(pRunTheFlagHomeInitiationAction);
						reinterpret_cast<TeamComposite*>(pRunTheFlagHomeSequence)->AddChild(pRunTheFlagHomeMonitor);
						reinterpret_cast<TeamComposite*>(pRushBaseAttackSequence)->AddChild(pRushBaseAttackPreconditionsCheck);
						reinterpret_cast<TeamComposite*>(pRushBaseAttackSequence)->AddChild(pRushBaseAttackInitiationAction);
						reinterpret_cast<TeamComposite*>(pRushBaseAttackSequence)->AddChild(pRushBaseAttackMonitor);
						reinterpret_cast<TeamComposite*>(pCoordinatedBaseAttackSequence)->AddChild(pCoordinatedBaseAttackPreconditionsCheck);
						reinterpret_cast<TeamComposite*>(pCoordinatedBaseAttackSequence)->AddChild(pCoordinatedBaseAttackInitiationAction);
						reinterpret_cast<TeamComposite*>(pCoordinatedBaseAttackSequence)->AddChild(pCoordinatedBaseAttackMonitor);
						reinterpret_cast<TeamComposite*>(pDistractionBaseAttackSequence)->AddChild(pDistractionBaseAttackPreconditionsCheck);
						reinterpret_cast<TeamComposite*>(pDistractionBaseAttackSequence)->AddChild(pDistractionBaseAttackInitiationAction);
						reinterpret_cast<TeamComposite*>(pDistractionBaseAttackSequence)->AddChild(pDistractionBaseAttackMonitor);
						reinterpret_cast<TeamComposite*>(pDefendBaseEntrancesSequence)->AddChild(pDefendBaseEntrancesPreconditionsCheck);
						reinterpret_cast<TeamComposite*>(pDefendBaseEntrancesSequence)->AddChild(pDefendBaseEntrancesInitiationAction);
						reinterpret_cast<TeamComposite*>(pDefendBaseEntrancesSequence)->AddChild(pDefendBaseEntrancesMonitor);
						reinterpret_cast<TeamComposite*>(pReturnDroppedFlagSequence)->AddChild(pReturnDroppedFlagPreconditionsCheck);
						reinterpret_cast<TeamComposite*>(pReturnDroppedFlagSequence)->AddChild(pReturnDroppedFlagInitiationAction);
						reinterpret_cast<TeamComposite*>(pReturnDroppedFlagSequence)->AddChild(pReturnDroppedFlagMonitor);

						// The condition checks performed during execution of the manoeuvres to ensure they are still valid
						ManoeuvreStillValidInitData runTheFlagHomeStillValidInitData(RunTheFlagHomeManoeuvre);
						TeamBehaviour* pRunTheFlagHomeStillValidCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvreStillValidType, pTeamAI, "RunTheFlagHomeStillValidCheck", 0.0f, 0.0f, &runTheFlagHomeStillValidInitData);
						ManoeuvreStillValidInitData rushBaseAttackStillValidInitData(RushBaseAttackManoeuvre);
						TeamBehaviour* pRushBaseAttackStillValidCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvreStillValidType, pTeamAI, "RushBaseAttackStillValidCheck", 0.0f, 0.0f, &rushBaseAttackStillValidInitData);
						ManoeuvreStillValidInitData coordinatedBaseAttackStillValidInitData(CoordinatedBaseAttackManoeuvre);
						TeamBehaviour* pCoordinatedBaseAttackStillValidCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvreStillValidType, pTeamAI, "CoordinatedBaseAttackStillValidCheck", 0.0f, 0.0f, &coordinatedBaseAttackStillValidInitData);
						ManoeuvreStillValidInitData distractionBaseAttackStillValidInitData(DistractionBaseAttackManoeuvre);
						TeamBehaviour* pDistractionBaseAttackStillValidCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvreStillValidType, pTeamAI, "DistractionBaseAttackStillValidCheck", 0.0f, 0.0f, &distractionBaseAttackStillValidInitData);
						ManoeuvreStillValidInitData defendBaseEntrancesStillValidInitData(DefendBaseEntrancesManoeuvre);
						TeamBehaviour* pDefendBaseEntrancesStillValidCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvreStillValidType, pTeamAI, "DefendBaseEntrancesStillValidCheck", 0.0f, 0.0f, &defendBaseEntrancesStillValidInitData);
						ManoeuvreStillValidInitData returnDroppedFlagStillValidInitData(ReturnDroppedFlagManoeuvre);
						TeamBehaviour* pReturnDroppedFlagStillValidCheck = CreatePrimitiveTeamBehaviour(TeamManoeuvreStillValidType, pTeamAI, "ReturnDroppedFlagStillValidCheck", 0.0f, 0.0f, &returnDroppedFlagStillValidInitData);
			
						// The actual execution of the manoeuvres is handled by these actions
						ExecuteTeamManoeuvreInitData executeRunTheFlagHomeInitData(RunTheFlagHomeManoeuvre);
						TeamBehaviour* pExecuteRunTheFlagHomeAction = CreatePrimitiveTeamBehaviour(TeamExecuteManoeuvreType, pTeamAI, "ExecuteRunTheFlagHomeAction", 0.0f, 1.0f, &executeRunTheFlagHomeInitData);
						ExecuteTeamManoeuvreInitData executeRushBaseAttackInitData(RushBaseAttackManoeuvre);
						TeamBehaviour* pExecuteRushBaseAttackAction = CreatePrimitiveTeamBehaviour(TeamExecuteManoeuvreType, pTeamAI, "ExecuteRushBaseAttackAction", 1.0f, 0.0f, &executeRushBaseAttackInitData);
						ExecuteTeamManoeuvreInitData executeCoordinatedBaseAttackInitData(CoordinatedBaseAttackManoeuvre);
						TeamBehaviour* pExecuteCoordinatedBaseAttackAction = CreatePrimitiveTeamBehaviour(TeamExecuteManoeuvreType, pTeamAI, "ExecuteCoordinatedBaseAttackAction", 0.5f, 0.5f, &executeCoordinatedBaseAttackInitData);
						ExecuteTeamManoeuvreInitData executeDistractionBaseAttackInitData(DistractionBaseAttackManoeuvre);
						TeamBehaviour* pExecuteDistractionBaseAttackAction = CreatePrimitiveTeamBehaviour(TeamExecuteManoeuvreType, pTeamAI, "ExecuteDistractionBaseAttackAction", 0.5f, 0.6f, &executeDistractionBaseAttackInitData);
						ExecuteTeamManoeuvreInitData executeDefendBaseEntrancesInitData(DefendBaseEntrancesManoeuvre);
						TeamBehaviour* pExecuteDefendBaseEntrancesAction = CreatePrimitiveTeamBehaviour(TeamExecuteManoeuvreType, pTeamAI, "ExecuteDefendBaseEntrancesAction", 0.0f, 1.0f, &executeDefendBaseEntrancesInitData);
						ExecuteTeamManoeuvreInitData returnDroppedFlagInitData(ReturnDroppedFlagManoeuvre);
						TeamBehaviour* pReturnDroppedFlagAction = CreatePrimitiveTeamBehaviour(TeamExecuteManoeuvreType, pTeamAI, "ReturnDroppedFlagAction", 0.5f, 0.5f, &returnDroppedFlagInitData);

						if(pRunTheFlagHomeStillValidCheck && pRushBaseAttackStillValidCheck && pCoordinatedBaseAttackStillValidCheck && pDistractionBaseAttackStillValidCheck && pDefendBaseEntrancesStillValidCheck && pReturnDroppedFlagStillValidCheck &&
						   pExecuteRunTheFlagHomeAction && pExecuteRushBaseAttackAction && pExecuteCoordinatedBaseAttackAction && pExecuteDistractionBaseAttackAction && pExecuteDefendBaseEntrancesAction && pReturnDroppedFlagAction)
						{
							reinterpret_cast<TeamMonitor*>(pRunTheFlagHomeMonitor)->AddCondition(pRunTheFlagHomeStillValidCheck);
							reinterpret_cast<TeamMonitor*>(pRunTheFlagHomeMonitor)->AddAction(pExecuteRunTheFlagHomeAction);
							reinterpret_cast<TeamMonitor*>(pRushBaseAttackMonitor)->AddCondition(pRushBaseAttackStillValidCheck);
							reinterpret_cast<TeamMonitor*>(pRushBaseAttackMonitor)->AddAction(pExecuteRushBaseAttackAction);
							reinterpret_cast<TeamMonitor*>(pCoordinatedBaseAttackMonitor)->AddCondition(pCoordinatedBaseAttackStillValidCheck);
							reinterpret_cast<TeamMonitor*>(pCoordinatedBaseAttackMonitor)->AddAction(pExecuteCoordinatedBaseAttackAction);
							reinterpret_cast<TeamMonitor*>(pDistractionBaseAttackMonitor)->AddCondition(pDistractionBaseAttackStillValidCheck);
							reinterpret_cast<TeamMonitor*>(pDistractionBaseAttackMonitor)->AddAction(pExecuteDistractionBaseAttackAction);
							reinterpret_cast<TeamMonitor*>(pDefendBaseEntrancesMonitor)->AddCondition(pDefendBaseEntrancesStillValidCheck);
							reinterpret_cast<TeamMonitor*>(pDefendBaseEntrancesMonitor)->AddAction(pExecuteDefendBaseEntrancesAction);
							reinterpret_cast<TeamMonitor*>(pReturnDroppedFlagMonitor)->AddCondition(pReturnDroppedFlagStillValidCheck);
							reinterpret_cast<TeamMonitor*>(pReturnDroppedFlagMonitor)->AddAction(pReturnDroppedFlagAction);

							return pTeamParallelRoot;
						}
					}
				}
			}
		}
	}

	/*
	TeamBehaviour* pTeamRoot = CreateParentTeamBehaviour(TeamActiveSelectorType, pTeamAI, "TeamRoot", nullptr);

	if(pTeamRoot)
	{
		TeamBehaviour* pUpdateTeamAISequence = CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "UpdateTeamAISequence", nullptr);

		TeamBehaviour* pTestActiveCharacteristicSelector = CreateParentTeamBehaviour(TeamActiveCharacteristicSelectorType, pTeamAI, "TestActiveCharacteristicSelector", nullptr);
		
		TeamBehaviour* pTestManoeuvreSequence = CreateParentTeamBehaviour(TeamSequenceType, pTeamAI, "TestManoeuvreSequence", nullptr);

		if(pUpdateTeamAISequence && pTestActiveCharacteristicSelector && pTestManoeuvreSequence)// && pAllAttackAction && pAllDefendAction && pAllMoveAction)
		{
			ReturnSpecificStatusInitData data(pUpdateTeamAISequence, StatusFailure);
			TeamBehaviour* pAlwaysFailDecorator = CreateParentTeamBehaviour(TeamReturnSpecificStatusType, pTeamAI, "TeamAlwaysFailDecorator", &data);

			TeamBehaviour* pTeamProcessMessagesAction = CreatePrimitiveTeamBehaviour(TeamProcessMessagesType, pTeamAI, "TeamProcessMessagesAction", 1.0f, 1.0f, nullptr);
			
			ManoeuvrePreconditionsFulfilledInitData conditionCheckData(RushBaseAttackManoeuvre);//(TestAllMoveManoeuvre);
			TeamBehaviour* pTestCheckManoeuvrePreconditions = CreatePrimitiveTeamBehaviour(TeamManoeuvrePreconditionsFulfilledType, pTeamAI, "TestCheckManoeuvrePreconditions", 0.0f, 0.0f, &conditionCheckData);
			
			ExecuteTeamManoeuvreInitData executedManoeuvre(RushBaseAttackManoeuvre);//(DefendBaseEntrancesManoeuvre);//(TestAllMoveManoeuvre);
			TeamBehaviour* pTestExecuteManoeuvreAction = CreatePrimitiveTeamBehaviour(TeamExecuteManoeuvreType, pTeamAI, "TeamExecuteManoeuvreAction", 0.5f, 0.5f, &executedManoeuvre);

			if(pAlwaysFailDecorator && pTeamProcessMessagesAction && pTestCheckManoeuvrePreconditions && pTestExecuteManoeuvreAction)
			{
				reinterpret_cast<TeamComposite*>(pTeamRoot)->AddChild(pAlwaysFailDecorator);
				reinterpret_cast<TeamComposite*>(pTeamRoot)->AddChild(pTestActiveCharacteristicSelector);

				reinterpret_cast<TeamComposite*>(pUpdateTeamAISequence)->AddChild(pTeamProcessMessagesAction);

				reinterpret_cast<TeamComposite*>(pTestActiveCharacteristicSelector)->AddChild(pTestManoeuvreSequence);

				reinterpret_cast<TeamComposite*>(pTestManoeuvreSequence)->AddChild(pTestCheckManoeuvrePreconditions);
				reinterpret_cast<TeamComposite*>(pTestManoeuvreSequence)->AddChild(pTestExecuteManoeuvreAction);

				return pTeamRoot;
			}
		}
	}
	*/
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
	case ObservationTargetSetType:
		return new ObservationTargetSet(name, pEntity);
		break;
	case PathToTargetSetType:
		return new PathToTargetSet(name, pEntity);
		break;
	case MovingToHighestPriorityTargetType:
		return new MovingToHighestPriorityTarget(name, pEntity);
		break;
	case DetermineMovementTargetType:
		return new DetermineMovementTarget(name, pEntity);
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
		break;
	case ProcessMessagesType:
		return new ProcessMessages(name, pEntity);
		break;
	case DetermineObservationTargetType:
		return new DetermineObservationTarget(name, pEntity);
		break;
	case DeterminePathToTargetType:
		return new DeterminePathToTarget(name, pEntity);
		break;
	case LookAtTargetType:
		return new LookAtTarget(name, pEntity);
		break;
	case FinaliseMovementType:
		return new FinaliseMovement(name, pEntity);
		break;
	default:
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Creates a certain primitive team behaviour for the provided team AI.
// Param1: Determines which behaviour will be created from a range of available types.
// Param2: A pointer to the team AI that the behaviour tree should be created for.
// Param3: The name that will be associated to the newly created behaviour.
// Param4: The aggressiveness value associated to this behaviour.
// Param5: The aggressiveness value associated to this behaviour.
// Param6: A pointer to a data structure holding the initialisation data for some behaviours.
// Returns a pointer to the newly created behaviour. The pointer is null if something failed 
// during creation or if the factory does not support the given behaviour type.
//--------------------------------------------------------------------------------------
TeamBehaviour* BehaviourFactory::CreatePrimitiveTeamBehaviour(PrimitiveTeamBehaviourType behaviourType, TeamAI* pTeamAI, const char* name, float aggressiveness, float defensiveness, void* pInitData)
{
	switch(behaviourType)
	{
	case TeamProcessMessagesType:
		{
		return new TeamProcessMessages(name, pTeamAI, aggressiveness, defensiveness);
		break;
		}
	case TeamInitiateManoeuvreType:
		{
		InitiateTeamManoeuvreInitData* pData = reinterpret_cast<InitiateTeamManoeuvreInitData*>(pInitData);
		return new InitiateTeamManoeuvre(name, pTeamAI, pData->m_manoeuvreType, aggressiveness, defensiveness);
		break;
		}
	case TeamExecuteManoeuvreType:
		{
		ExecuteTeamManoeuvreInitData* pData = reinterpret_cast<ExecuteTeamManoeuvreInitData*>(pInitData);
		return new ExecuteTeamManoeuvre(name, pTeamAI, pData->m_manoeuvreType, aggressiveness, defensiveness);
		break;
		}
	case TeamManoeuvrePreconditionsFulfilledType:
		{
		ManoeuvrePreconditionsFulfilledInitData* pData = reinterpret_cast<ManoeuvrePreconditionsFulfilledInitData*>(pInitData);
		return new ManoeuvrePreconditionsFulfilled(name, pTeamAI, pData->m_manoeuvreType, aggressiveness, defensiveness);
		break;
		}
	case TeamManoeuvreStillValidType:
		{
		ManoeuvreStillValidInitData* pData = reinterpret_cast<ManoeuvreStillValidInitData*>(pInitData);
		return new ManoeuvreStillValid(name, pTeamAI, pData->m_manoeuvreType, aggressiveness, defensiveness);
		break;
		}
		/*
	case TeamAllAttackType:
		return new TeamAllAttack(name, pTeamAI);
		break;
	case TeamAllDefendType:
		return new TeamAllDefend(name, pTeamAI);
		break;
	case TeamAllMoveType:
		return new TeamAllMove(name, pTeamAI);
		break;*/
	default:
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Creates a certain parent team behaviour for the provided team AI.
// Param1: Determines which behaviour will be created from a range of available types.
// Param2: A pointer to the team AI that the behaviour tree should be created for.
// Param3: The name that will be associated to the newly created behaviour.
// Param4: A pointer to a data structure holding the initialisation data for some behaviours.
// Returns a pointer to the newly created behaviour. The pointer is null if something failed 
// during creation or if the factory does not support the given behaviour type.
//--------------------------------------------------------------------------------------
TeamBehaviour* BehaviourFactory::CreateParentTeamBehaviour(ParentTeamBehaviourType behaviourType, TeamAI* pTeamAI, const char* name, void* pInitData)
{
	switch(behaviourType)
	{
	case TeamSelectorType:
		return new TeamSelector(name, pTeamAI);
		break;
	case TeamActiveSelectorType:
		return new TeamActiveSelector(name, pTeamAI);
		break;
	case TeamActiveCharacteristicSelectorType:
		return new TeamActiveCharacteristicSelector(name, pTeamAI);
		break;
	case TeamSequenceType:
		return new TeamSequence(name, pTeamAI);
		break;
	case TeamParallelType:
		{
		TeamParallelInitData* pData = reinterpret_cast<TeamParallelInitData*>(pInitData);
		return new TeamParallel(name, pTeamAI, pData->m_successPolicy, pData->m_failurePolicy);
		break;
		}
	case TeamMonitorType:
		return new TeamMonitor(name, pTeamAI);
		break;
	case TeamRepeatType:
		{
		TeamRepeatInitData* pData = reinterpret_cast<TeamRepeatInitData*>(pInitData);
		return new TeamRepeat(name, pTeamAI, pData->m_pChild, pData->m_numberOfRepeats);
		break;
		}
	case TeamReturnSpecificStatusType:
		{
		TeamReturnSpecificStatusInitData* pData = reinterpret_cast<TeamReturnSpecificStatusInitData*>(pInitData);
		return new TeamReturnSpecificStatus(name, pTeamAI, pData->m_pChild, pData->m_returnStatus);
		break;
		}
	default:
		return nullptr;
	}
}



/*
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
	case TeamAllAttackType:
		return new TeamAllAttack(name, pMultiflagCTFTeamAI);
		break;
	case TeamAllDefendType:
		return new TeamAllDefend(name, pMultiflagCTFTeamAI);
		break;
	case TeamAllMoveType:
		return new TeamAllMove(name, pMultiflagCTFTeamAI);
		break;
	default:
		return nullptr;
	}
}
*/