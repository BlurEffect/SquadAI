/* 
*  Kevin Meergans, SquadAI, 2014
*  DefendBaseEntrances.cpp
*  A team manoeuvre that orders the participating entities to move to the entrances to
*  their home base and defend it against any intruders.
*/

// Includes
#include "DefendBaseEntrances.h"
#include "MultiflagCTFTeamAI.h"
#include "Entity.h"
#include "TestEnvironment.h"

DefendBaseEntrances::DefendBaseEntrances(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float switchPositionsInterval)
	: TeamManoeuvre(DefendBaseEntrancesManoeuvre, ProtectOwnFlagCategory, minNumberParticipants, maxNumberParticipants),
	  m_pTeamAI(pTeamAI),
	  m_switchPositionsInterval(switchPositionsInterval),
	  m_timer(0.0f)
{
}

DefendBaseEntrances::~DefendBaseEntrances(void)
{
}


//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void DefendBaseEntrances::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case EntityKilledMessageType:
	{
	EntityKilledMessage* pMsg = reinterpret_cast<EntityKilledMessage*>(pMessage);
	if(pMsg->GetData().m_team == GetTeamAI()->GetTeam() && IsParticipant(pMsg->GetData().m_id))
	{
		// Participants that get killed, drop out of the manoeuvre
		m_pTeamAI->ReleaseEntityFromManoeuvre(pMsg->GetData().m_id);

		// A defender was killed, send all idle defenders to the entrance the killed entity was guarding to prevent a 
		// likely intrusion of enemies.
		ShiftDefense(m_guardedEntrances.at(pMsg->GetData().m_id).m_entranceDirection, m_guardedEntrances.at(pMsg->GetData().m_id).m_entrancePosition); 
	}
	break;
	}
	case UpdateOrderStateMessageType:
	{
	// Cancel old order, Send Follow-Up Orders, finish manoeuvre etc
	UpdateOrderStateMessage* pMsg = reinterpret_cast<UpdateOrderStateMessage*>(pMessage);
	if(IsParticipant(pMsg->GetData().m_entityId))
	{
		if(pMsg->GetData().m_orderState == FailedOrderState)
		{
			// Entities executing a defend manoeuvre won't send success messages concerning the order state
			// as the defend order is a passive behaviour that is unlimited in time and thus cannot succeed.
			// It is thus sufficient to check for failure.

			// The order failed -> release the entity from the manoeuvre
			m_pTeamAI->ReleaseEntityFromManoeuvre(pMsg->GetData().m_entityId);
		}
	}
	break;
	}
	default:
		TeamManoeuvre::ProcessMessage(pMessage);
	}
}

//--------------------------------------------------------------------------------------
// Shifts the entities to ensure a more effective defense.
// Param1: The direction of the entrance on which to focus.
// Param2: The position of the entrance towards which the defense should be shifted.
//--------------------------------------------------------------------------------------
void DefendBaseEntrances::ShiftDefense(Direction entranceDirection, const XMFLOAT2& entrancePosition)
{
	
	// Find all idle defenders, and update their defend positions
	// (Idle defenders are all, who don't have any enemies in their sight and thus are
	// guaranteed not to be in a fight with an enemy)
	for(std::vector<Entity*>::const_iterator it = GetParticipants().begin(); it != GetParticipants().end(); ++it)
	{
		if(GetTeamAI()->GetSpottedEnemies().at((*it)->GetId()).empty())
		{
			reinterpret_cast<DefendOrder*>(m_activeOrders[(*it)->GetId()])->SetDefendPosition(entrancePosition);
			m_guardedEntrances.at((*it)->GetId()).m_entranceDirection = entranceDirection;
			m_guardedEntrances.at((*it)->GetId()).m_entrancePosition = entrancePosition;
		}
	}
}

//--------------------------------------------------------------------------------------
// Distributes the participating entities over all base entrances to ensure that all
// directions are guarded.
// Returns true if the distribution of the team members succeeded, false otherwise.
//--------------------------------------------------------------------------------------
bool DefendBaseEntrances::DistributeEntities(void)
{
	if(GetNumberOfParticipants() == 0 || m_pTeamAI->GetTestEnvironment()->GetBaseEntrances(m_pTeamAI->GetTeam()).empty())
	{
		return false;
	}

	m_guardedEntrances.clear();

	// Keeps track of how many entities are guarding each direction
	unsigned int entityCount[NumberOfDirections] = {0};

	int startIndex = rand() % m_pTeamAI->GetTestEnvironment()->GetBaseEntrances(m_pTeamAI->GetTeam()).size(); 

	std::unordered_map<Direction, std::vector<XMFLOAT2>>::const_iterator entranceIt = m_pTeamAI->GetTestEnvironment()->GetBaseEntrances(m_pTeamAI->GetTeam()).begin();
	std::advance(entranceIt, startIndex);

	for(std::vector<Entity*>::iterator it = m_participants.begin(); it != m_participants.end(); ++it)
	{
		// Randomly determine an entrance from the available entrances for the current direction
		// Make sure the entrance is not yet guarded by another participant.

		// Note: This could be implemented better, it's still not a perfectly even distribution among all entrances.
		unsigned int entranceNumber(0);
		do
		{
			entranceNumber = rand() % entranceIt->second.size();
		}while(entityCount[entranceIt->first] < entranceIt->second.size() && IsGuarded(entranceIt->first, entranceIt->second[entranceNumber]));

		++entityCount[entranceIt->first];

		// Determine the observation direction
		XMFLOAT2 viewDirection(0.0f, 0.0f);
		XMStoreFloat2(&viewDirection, XMLoadFloat2(&entranceIt->second[entranceNumber]) - XMLoadFloat2(&m_pTeamAI->GetFlagData(m_pTeamAI->GetTeam()).m_basePosition));

		Order* pNewOrder = new DefendOrder((*it)->GetId(), DefendPositionOrder, MediumPriority, entranceIt->second[entranceNumber], viewDirection);
			
		if(!pNewOrder)
		{
			return false;
		}

		FollowOrderMessageData data(pNewOrder);
		SendMessage(*it, FollowOrderMessageType, &data);

		m_guardedEntrances.insert(std::pair<unsigned long, GuardData>((*it)->GetId(), GuardData(entranceIt->first, entranceIt->second[entranceNumber])));
		m_activeOrders.insert(std::pair<unsigned long, Order*>((*it)->GetId(), pNewOrder));

		if(++entranceIt == m_pTeamAI->GetTestEnvironment()->GetBaseEntrances(m_pTeamAI->GetTeam()).end())
		{
			entranceIt = m_pTeamAI->GetTestEnvironment()->GetBaseEntrances(m_pTeamAI->GetTeam()).begin();
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Checks whether a given entrance is already being guarded or not.
// Param1: The direction, in which the entrance is facing.
// Param2: The entrance to check.
// Returns true if the given entrance is already being guarded by a participant of the manoeuvre.
//--------------------------------------------------------------------------------------
bool DefendBaseEntrances::IsGuarded(Direction direction, const XMFLOAT2& entrance) const
{
	for(std::unordered_map<unsigned long, GuardData>::const_iterator it = m_guardedEntrances.begin(); it != m_guardedEntrances.end(); ++it)
	{
		if(it->second.m_entranceDirection == direction && it->second.m_entrancePosition.x == entrance.x && it->second.m_entrancePosition.y == entrance.y)
		{
			return true;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
// Returns a behaviour status code representing the current state of the initiation of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus DefendBaseEntrances::Initiate(void)
{
	if(DistributeEntities())
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
	}
}

//--------------------------------------------------------------------------------------
// Updates the manoeuvre. This mostly consists of processing any messages received from
// the participating entities and reacting accordingly. This can include the sending
// out of follow-up orders for entities that have completed a stage of the manoeuvre. Eventually
// the function should also check whether the goal of the manoeuvre has been achieved in 
// order to initiate termination.
// Param1: The time passed since the last update.
// Returns a behaviour status representing the state of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus DefendBaseEntrances::Update(float deltaTime)
{
	// Keep track of who has reached the target and let those defend until all have arrived
	SortOutProcessedMessages();
	ProcessMessages();

	m_timer += deltaTime;

	if(m_switchPositionsInterval != 0.0f && m_timer >= m_switchPositionsInterval)
	{
		DistributeEntities();
		m_timer = 0.0f;
	}

	if(!IsActive() || HasFailed() || (GetNumberOfParticipants() < GetMinNumberOfParticipants()))
	{
		// The manoeuvre will fail if something failed during the initiation or if it wasn't
		// initiated at all.
		return StatusFailure;
	}else if(HasSucceeded())
	{
		return StatusSuccess;
	}

	return StatusRunning;
}

//--------------------------------------------------------------------------------------
// Terminates the manoeuvre. This mostly consists of cancelling all active orders and
// removing all participating entities.
//--------------------------------------------------------------------------------------
void DefendBaseEntrances::Terminate(void)
{
	m_guardedEntrances.clear();
	m_timer = 0.0f;
	TeamManoeuvre::Terminate();
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void DefendBaseEntrances::Reset(void)
{
	m_timer = 0.0f;
	m_guardedEntrances.clear();

	TeamManoeuvre::Reset();
}

const MultiflagCTFTeamAI* DefendBaseEntrances::GetTeamAI(void) const
{
	return m_pTeamAI;
}

float DefendBaseEntrances::GetSwitchPositionsInterval(void) const
{
	return m_switchPositionsInterval;
}

void DefendBaseEntrances::SetTeamAI(MultiflagCTFTeamAI* pTeamAI)
{
	if(pTeamAI)
	{
		m_pTeamAI = pTeamAI;
	}
}

void DefendBaseEntrances::SetSwitchPositionsInterval(float interval)
{
	m_switchPositionsInterval = interval;
}