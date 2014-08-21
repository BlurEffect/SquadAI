/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamManoeuvre.cpp
*  Abstract base class for all team manoeuvres.
*/

// Includes
#include "Entity.h"
#include "Order.h"


TeamManoeuvre::TeamManoeuvre(TeamManoeuvreType type, unsigned int minNumberParticipants, unsigned int maxNumberParticipants)
	: m_active(false),
	  m_type(type),
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
		// Cancel the currently active order for the entity that will be deleted.
		CancelOrder(*foundIt);
		// Remove the entity from the participants
		m_participants.erase(foundIt);
	}
}

//--------------------------------------------------------------------------------------
// Cancels an order and deletes it from the list of active orders.
// Param1: A pointer for the entity, whose active order should be cancelled.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::CancelOrder(Entity* pEntity)
{
	std::unordered_map<unsigned long, Order*>::iterator foundIt = m_activeOrders.find(pEntity->GetId());

	if(foundIt != m_activeOrders.end())
	{
		delete foundIt->second;
		foundIt->second = nullptr;

		m_activeOrders.erase(foundIt);
	}

	// Notify the entity that the order was cancelled
	SendMessage(pEntity, CancelOrderMessageType, nullptr);
}

//--------------------------------------------------------------------------------------
// Cancels an order and deletes it from the list of active orders.
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
// Processes an inbox message that the team AI received.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::ProcessMessage(Message* pMessage)
{
	switch(pMessage->GetType())
	{
	case EnemySpottedMessageType:
		// Update attack orders for that enemy
		break;
	case UpdateEnemyPositionMessageType:
		// Update attack orders
		break;
	case EntityKilledMessageType:
		// Cancel all attack orders, cancel/remove a participant (dependign on manoeuvre)
		break;
	case UpdateOrderStateMessageType:
		// Cancel old order, Send Follow-Up Orders, finish manoeuvre etc
		break;
	}
}

//--------------------------------------------------------------------------------------
// Initiates the manoeuvre. This mostly consists of sending initial orders to all
// participating entities and everything that is involved in that process, such as 
// determining targets etc.
//--------------------------------------------------------------------------------------
void TeamManoeuvre::Initiate(void)
{
	// Default implementation.
	m_active = true;
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
	if(m_active)
	{
		return StatusSuccess;
	}else
	{
		return StatusFailure;
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
	std::unordered_map<unsigned long, Order*>::iterator it = m_activeOrders.begin();
	while(it != m_activeOrders.end())
	{
		// This call includes erasing of the current iterator, thus incrementing is not necessary.
		// TODO: Check if this is safe.
		CancelOrder(it->first);
	}

	m_participants.clear();

	m_active = false;
}

// Data access functions

bool TeamManoeuvre::IsActive(void) const
{
	return m_active;
}

TeamManoeuvreType TeamManoeuvre::GetType(void) const
{
	return m_type;
}

unsigned int TeamManoeuvre::GetMinNumberOfParticipants(void) const
{
	return m_minNumberOfParticipants;
}

unsigned int TeamManoeuvre::GetMaxNumberOfParticipants(void) const
{
	return m_maxNumberOfParticipants;
}

void TeamManoeuvre::SetActive(bool active)
{
	m_active = active;
}

void TeamManoeuvre::SetType(TeamManoeuvreType type)
{
	m_type = type;
}

void TeamManoeuvre::SetMinNumberOfParticipants(unsigned int minNumberOfParticipants) 
{
	m_minNumberOfParticipants = minNumberOfParticipants;
}

void TeamManoeuvre::SetMaxNumberOfParticipants(unsigned int maxNumberOfParticipants) 
{
	m_maxNumberOfParticipants = maxNumberOfParticipants;
}

