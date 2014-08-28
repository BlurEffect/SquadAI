/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamManoeuvre.cpp
*  Abstract base class for all team manoeuvres.
*/

// Includes
#include "Entity.h"
#include "Order.h"


TeamManoeuvre::TeamManoeuvre(TeamManoeuvreType type, TeamManoeuvreCategory category, unsigned int minNumberParticipants, unsigned int maxNumberParticipants)
	: m_active(false),
	  m_succeeded(false),
	  m_failed(false),
	  m_type(type),
	  m_category(category),
	  m_minNumberOfParticipants(minNumberParticipants),
	  m_maxNumberOfParticipants(maxNumberParticipants)
{
}

TeamManoeuvre::~TeamManoeuvre(void)
{
	ClearOrders();
}

//--------------------------------------------------------------------------------------
// Adds an entity to the participating entities of this manoeuvre. All participants have
// to be added to a manoeuvre before initiating it. Thus, at the moment entities that are
// added after the manoeuvre has started, will be added but won't be able to participate.
// Param1: A pointer to the entity that should be added to the manoeuvre.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::AddParticipant(Entity* pEntity)
{
	if(pEntity)
	{
		// Check if the entity is already participating
		std::vector<Entity*>::iterator foundIt = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(pEntity->GetId()));
		if(foundIt == m_participants.end())
		{
			// Add the entity as new participant
			m_participants.push_back(pEntity);
		}
	}
}

//--------------------------------------------------------------------------------------
// Removes an entity from the participating entities of this manoeuvre.
// Param1: The id of the entity to remove from the manoeuvre.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::RemoveParticipant(unsigned long id)
{
	// Find the entity to remove among the participants
	std::vector<Entity*>::iterator foundIt = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(id));
	if(foundIt != m_participants.end())
	{
		// Cancel and delete the currently active order for the entity that will be deleted.
		CancelOrder(*foundIt);
		std::unordered_map<unsigned long, Order*>::iterator foundOrder = m_activeOrders.find((*foundIt)->GetId());
		m_activeOrders.erase(foundOrder);

		// Remove the entity from the participants
		m_participants.erase(foundIt);
	}else
	{
		//DEBUG
		int a = 3;
	}
}

//--------------------------------------------------------------------------------------
// Resets the manoeuvre. Should only be called to reset the manoeuver in between game rounds.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::Reset(void)
{
	ClearOrders();
	ResetCommunication();
	m_participants.clear();
	m_active = false;
	m_succeeded = false;
	m_failed = false;
}

//--------------------------------------------------------------------------------------
// Cancels an order and notifies the associated entity of the cancelling. The order has
// to be deleted separately from the list of active orders after cancelling it.
// Param1: A pointer for the entity, whose active order should be cancelled.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::CancelOrder(Entity* pEntity)
{
	std::unordered_map<unsigned long, Order*>::iterator foundIt = m_activeOrders.find(pEntity->GetId());

	if(foundIt != m_activeOrders.end())
	{
		if(foundIt->second)
		{
			delete foundIt->second;
			foundIt->second = nullptr;
		}
	}
	
	// Notify the entity that the order was cancelled
	SendMessage(pEntity, CancelOrderMessageType, nullptr);
}

//--------------------------------------------------------------------------------------
// Cancels an order and notifies the associated entity of the cancelling. The order has
// to be deleted separately from the list of active orders after cancelling it.
// Param1: The id of the entity for which to cancel the active order.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::CancelOrder(unsigned long id)
{
	// Find the entity
	std::vector<Entity*>::iterator foundEntity = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(id));
	if(foundEntity != m_participants.end())
	{
		CancelOrder(*foundEntity);
	}
}

//--------------------------------------------------------------------------------------
// Deletes all active orders associated to the manoeuvre.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::ClearOrders(void)
{
	for(std::unordered_map<unsigned long, Order*>::iterator it = m_activeOrders.begin(); it != m_activeOrders.end(); ++it)
	{
		if(it->second)
		{
			delete it->second;
			it->second = nullptr;
		}
	}

	m_activeOrders.clear();
}

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::ProcessEvent(EventType type, void* pEventData)
{
	// Do nothing, not expecting any events.
}

//--------------------------------------------------------------------------------------
// Processes an inbox message that the manoeuvre received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::ProcessMessage(Message* pMessage)
{
	// Default implementations of some messages that can be forwarded from a team AI

	switch(pMessage->GetType())
	{
	case EnemySpottedMessageType:
		{
		// Update any attack orders on this enemy with the newest position
		EnemySpottedMessage* pMsg = reinterpret_cast<EnemySpottedMessage*>(pMessage);
		UpdateAttackOrders(pMsg->GetData().m_enemyId, pMsg->GetData().m_enemyPosition);
		break;
		}
	case UpdateEnemyPositionMessageType:
		{
		// Update attack orders
		UpdateEnemyPositionMessage* pMsg = reinterpret_cast<UpdateEnemyPositionMessage*>(pMessage);
		UpdateAttackOrders(pMsg->GetData().m_enemyId, pMsg->GetData().m_enemyPosition);
		break;
		}
	case EntityKilledMessageType:
		{
		// Cancel all attack orders, cancel/remove a participant (dependign on manoeuvre)
		EntityKilledMessage* pMsg = reinterpret_cast<EntityKilledMessage*>(pMessage);
		
		// Make sure to cancel all active attack orders associated to the killed enemy
		std::unordered_map<unsigned long, Order*>::iterator it = m_activeOrders.begin();
		while(it != m_activeOrders.end())
		{
			if(it->second->GetOrderType() == AttackEnemyOrder && reinterpret_cast<AttackOrder*>(it->second)->GetEnemyId() == pMsg->GetData().m_id)
			{
				// TODO: Check if this actually works, involves erasing of the iterator
				CancelOrder(it->first);
				it = m_activeOrders.erase(it);
			}else
			{
				++it;
			}
		}
		break;
		}
	case UpdateOrderStateMessageType:
		{
		// Cancel old order, Send Follow-Up Orders, finish manoeuvre etc
		UpdateOrderStateMessage* pMsg = reinterpret_cast<UpdateOrderStateMessage*>(pMessage);
		if(pMsg->GetData().m_orderState == SucceededOrderState || pMsg->GetData().m_orderState == FailedOrderState)
		{
			CancelOrder(pMsg->GetData().m_entityId);

			// Find the order based on the entity's ID
			std::unordered_map<unsigned long, Order*>::iterator foundIt = m_activeOrders.find(pMsg->GetData().m_entityId);
			m_activeOrders.erase(foundIt);
		}
		break;
		}
	default:
		Communicator::ProcessMessage(pMessage);
	}
}

//--------------------------------------------------------------------------------------
// Checks whether an entity is taking part in this manoeuvre.
// Param1: The id of the entity to check.
// Returns true if the checked entity is taking part in the manoeuvre, false otherwise.
//--------------------------------------------------------------------------------------
bool TeamManoeuvre::IsParticipant(unsigned long id) const
{
	std::vector<Entity*>::const_iterator foundIt = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(id));
	return foundIt != m_participants.end();
}

//--------------------------------------------------------------------------------------
// Static helper function to get the string representation of the name of a given
// manoeuvre type.
// Param1: The type of manoeuvre, for which to retrieve the string name.
// Returns a string containing the name of the manoeuvre. If the provided type is invalid,
// the returned string will be empty.
//--------------------------------------------------------------------------------------
const char* TeamManoeuvre::GetManoeuvreNameFromType(TeamManoeuvreType type)
{
	switch(type)
	{
	case DefendBaseEntrancesManoeuvre:
		return "Defend Base Entrances";
		break;
	case RushBaseAttackManoeuvre:
		return "Rush Base Attack";
		break;
	case RunTheFlagHomeManoeuvre:
		return "Run The Flag Home";
		break;
	default:
		return "";
	}
}

//--------------------------------------------------------------------------------------
// Updates the currently active attack orders by making sure the contained enemy 
// positions are up to data.
// Param1: The id of the enemy that moved and for which associated attack orders have to be updated.
// Param2: The new position of the enemy.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::UpdateAttackOrders(unsigned long enemyId, const XMFLOAT2 newPosition)
{
	// Check if there are any attack orders that have to be updated.
	for(std::unordered_map<unsigned long, Order*>::iterator it = m_activeOrders.begin(); it != m_activeOrders.end(); ++it)
	{
		if(it->second->GetOrderType() == AttackEnemyOrder)
		{
			AttackOrder* pAttackOrder = reinterpret_cast<AttackOrder*>(it->second);
			if(pAttackOrder->GetEnemyId() == enemyId)
			{
				// Update the order with the latest known position of the enemy
				pAttackOrder->SetEnemyPosition(newPosition);
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
// Returns a behaviour status code representing the current state of the initiation of the manoeuvre.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamManoeuvre::Initiate(void)
{
	// Default implementation.
	m_active = true;
	return StatusSuccess;
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
BehaviourStatus TeamManoeuvre::Update(float deltaTime)
{
	SortOutProcessedMessages();
	ProcessMessages();

	if(GetNumberOfParticipants() < GetMinNumberOfParticipants())
	{
		return StatusFailure;
	}

	if(m_succeeded)
	{
		return StatusSuccess;
	}else if(m_failed)
	{
		return StatusFailure;
	}else
	{
		return StatusRunning;
	}
}

//--------------------------------------------------------------------------------------
// Terminates the manoeuvre. This mostly consists of cancelling all active orders and
// removing all participating entities.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::Terminate(void)
{
	// Default implementation
	
	// Cancel orders for all participants
	for(std::unordered_map<unsigned long, Order*>::iterator it = m_activeOrders.begin(); it != m_activeOrders.end(); ++it)
	{
		CancelOrder(it->first);
	}

	m_activeOrders.clear();
	m_participants.clear();

	m_active = false;
	m_succeeded = false;
	m_failed = false;
}


const Entity* TeamManoeuvre::GetParticipant(unsigned long id) const
{
	std::vector<Entity*>::const_iterator foundIt = std::find_if(m_participants.begin(), m_participants.end(), Entity::FindEntityById(id));

	if(foundIt != m_participants.end())
	{
		return (*foundIt);
	}

	return nullptr;
}

// Data access functions

bool TeamManoeuvre::IsActive(void) const
{
	return m_active;
}

bool TeamManoeuvre::HasSucceeded(void) const
{
	return m_succeeded;
}

bool TeamManoeuvre::HasFailed(void) const
{
	return m_failed;
}

TeamManoeuvreType TeamManoeuvre::GetType(void) const
{
	return m_type;
}

TeamManoeuvreCategory TeamManoeuvre::GetCategory(void) const
{
	return m_category;
}

unsigned int TeamManoeuvre::GetMinNumberOfParticipants(void) const
{
	return m_minNumberOfParticipants;
}

unsigned int TeamManoeuvre::GetMaxNumberOfParticipants(void) const
{
	return m_maxNumberOfParticipants;
}

const std::vector<Entity*>& TeamManoeuvre::GetParticipants(void) const
{
	return m_participants;
}

unsigned int TeamManoeuvre::GetNumberOfParticipants(void) const
{
	return m_participants.size();
}

void TeamManoeuvre::SetActive(bool active)
{
	m_active = active;
}

void TeamManoeuvre::SetSucceeded(bool succeeded)
{
	m_succeeded = succeeded;
}

void TeamManoeuvre::SetFailed(bool failed)
{
	m_failed = failed;
}

void TeamManoeuvre::SetType(TeamManoeuvreType type)
{
	m_type = type;
}

void TeamManoeuvre::SetCategory(TeamManoeuvreCategory category)
{
	m_category = category;
}

void TeamManoeuvre::SetMinNumberOfParticipants(unsigned int minNumberOfParticipants) 
{
	m_minNumberOfParticipants = minNumberOfParticipants;
}

void TeamManoeuvre::SetMaxNumberOfParticipants(unsigned int maxNumberOfParticipants) 
{
	m_maxNumberOfParticipants = maxNumberOfParticipants;
}

