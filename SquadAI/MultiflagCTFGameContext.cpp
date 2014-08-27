/* 
*  Kevin Meergans, SquadAI, 2014
*  MultiflagCTFGameContext.cpp
*  This game context contains the state of a running multiflag Capture-the-flag game
*  and is in charge of controlling the game mode specific flag objects.
*/

// Includes
#include "MultiflagCTFGameContext.h"
#include "Entity.h"

MultiflagCTFGameContext::MultiflagCTFGameContext(float maxTime, float notifyTimeInterval, unsigned int winScore, float flagResetTimeLimit) 
	: GameContext(MultiflagCTF, maxTime, notifyTimeInterval, winScore),
	  m_flagResetTimeLimit(flagResetTimeLimit)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_flagResetTimers[i]   = m_flagResetTimeLimit;
		m_flagStates[i]		   = InBase;	    
	    m_flagBasePositions[i] = XMFLOAT2(0.0f, 0.0f);
	    m_flagPositions[i]	   = XMFLOAT2(0.0f, 0.0f);    
	    m_flagCarriers[i]	   = nullptr;    
	}
}

MultiflagCTFGameContext::~MultiflagCTFGameContext(void)
{
}

//--------------------------------------------------------------------------------------
// Updates the multiflag CTF game state.
// Param1: The time passed since the last update.
//--------------------------------------------------------------------------------------
void MultiflagCTFGameContext::Update(float deltaTime)
{
	GameContext::Update(deltaTime);

	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		if(m_flagStates[i] == Dropped)
		{
			// Update the return timer.
			m_flagResetTimers[i] -= deltaTime;
			if(m_flagResetTimers[i] <= 0.0f)
			{
				// Timer expired, return the flag to the base.
				FlagReturned(EntityTeam(i));
			}
		}else if(m_flagStates[i] == Stolen)
		{
			// Update the flag position with the one of the entity carrying it.
			m_flagPositions[i] = m_flagCarriers[i]->GetPosition();
			m_flags[i]->SetPosition(m_flagPositions[i]);
			m_flags[i]->UpdateColliderPosition(m_flagPositions[i]);
		}

		// If a carrier dies, drop the flag
		if(m_flagCarriers[i] && !m_flagCarriers[i]->IsAlive())
		{
			FlagDropped(EntityTeam(i));
		}
	}
}

//--------------------------------------------------------------------------------------
// Resets the multiflag CTF game state.
//--------------------------------------------------------------------------------------
void MultiflagCTFGameContext::Reset(void)
{
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_flagResetTimers[i] = m_flagResetTimeLimit;
		m_flagStates[i]		 = InBase;	    
	    m_flagPositions[i]	 = m_flagBasePositions[i];
	    m_flagCarriers[i]	 = nullptr;    
	    m_flags[i]->SetPosition(m_flagPositions[i]);
		m_flags[i]->UpdateColliderPosition(m_flagPositions[i]);
	}

	GameContext::Reset();
}


//--------------------------------------------------------------------------------------
// Processes a given message. Default implementation.
// Param1: A pointer to the message to process.
// Returns true if this was the final communicator to process the message, false if the
// message was forwarded to another one.
//--------------------------------------------------------------------------------------
bool MultiflagCTFGameContext::ProcessMessage(Message* pMessage)
{
	// Not expecting any messages, call the default implementation that simply marks all
	// messages as processed.
	return GameContext::ProcessMessage(pMessage);
}

//--------------------------------------------------------------------------------------
// Process a given event.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void MultiflagCTFGameContext::ProcessEvent(EventType type, void* pEventData)
{
	switch(type)
	{
	case ObjectiveAddedEventType:
		{
		ObjectiveAddedEventData* pAddObjectiveData = reinterpret_cast<ObjectiveAddedEventData*>(pEventData);
		SetFlag(pAddObjectiveData->m_pObjective->GetTeam(), pAddObjectiveData->m_pObjective);
		m_flagBasePositions[pAddObjectiveData->m_pObjective->GetTeam()] = pAddObjectiveData->m_pObjective->GetPosition();
		break;
		}
	case EntityReachedObjectiveEventType:
		{
		EntityReachedObjectiveEventData* pEntityReachedObjectiveData = reinterpret_cast<EntityReachedObjectiveEventData*>(pEventData);
		switch(m_flagStates[pEntityReachedObjectiveData->m_pObjective->GetTeam()])
		{
		case InBase:
			if(pEntityReachedObjectiveData->m_pEntity->GetTeam() != pEntityReachedObjectiveData->m_pObjective->GetTeam())
			{
				// An entity of a hostile team reached the flag, make the entity the new flag carrier
				FlagPickedUp(pEntityReachedObjectiveData->m_pObjective->GetTeam(), pEntityReachedObjectiveData->m_pEntity);
			}else
			{
				for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
				{
					// Check if there is a homecoming flag carrier
					// (flag can only be returned when the own flag is at home, this collision with the own flag
					// in base is used as an indicator whether the flag was successfully captured)
					if(m_flagCarriers[i] && (pEntityReachedObjectiveData->m_pEntity->GetId() == m_flagCarriers[i]->GetId()))
					{
						// The flag carrier reached the base position of his own flag -> captured
						FlagCaptured(EntityTeam(i));
					}
				}
			}
			break;
		case Dropped:
			if(pEntityReachedObjectiveData->m_pEntity->GetTeam() != pEntityReachedObjectiveData->m_pObjective->GetTeam())
			{
				// An entity of a hostile team reached the flag, make the entity the new flag carrier
				FlagPickedUp(pEntityReachedObjectiveData->m_pObjective->GetTeam(), pEntityReachedObjectiveData->m_pEntity);
			}else
			{
				// An entity of the team the flag belongs to has reached the flag -> return it
				FlagReturned(pEntityReachedObjectiveData->m_pObjective->GetTeam());
			}
			break;
		}
		break;
		}
	default:
		// Forward all other events to the base class
		GameContext::ProcessEvent(type, pEventData);
	}
}
/*
//--------------------------------------------------------------------------------------
// Receives and processes a given message.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void MultiflagCTFGameContext::ProcessMessage(Message* pMessage)
{	
	// Catch all messages relevant to this game mode

	switch(pMessage->GetType())
	{
	case AddObjectiveMessageType:
		{
		AddObjectiveMessage* pAddObjectiveMessage = reinterpret_cast<AddObjectiveMessage*>(pMessage);
		SetFlag(pAddObjectiveMessage->GetObjective()->GetTeam(), pAddObjectiveMessage->GetObjective());
		m_flagBasePositions[pAddObjectiveMessage->GetObjective()->GetTeam()] = pAddObjectiveMessage->GetObjective()->GetPosition();
		break;
		}
	case EntityReachedObjectiveMessageType:
		{
		EntityReachedObjectiveMessage* pEntityReachedObjectiveMessage = reinterpret_cast<EntityReachedObjectiveMessage*>(pMessage);
		switch(m_flagStates[pEntityReachedObjectiveMessage->GetObjective()->GetTeam()])
		{
		case InBase:
			if(pEntityReachedObjectiveMessage->GetEntity()->GetTeam() != pEntityReachedObjectiveMessage->GetObjective()->GetTeam())
			{
				// An entity of a hostile team reached the flag, make the entity the new flag carrier
				FlagPickedUp(pEntityReachedObjectiveMessage->GetObjective()->GetTeam(), pEntityReachedObjectiveMessage->GetEntity());
			}else
			{
				for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
				{
					// Check if there is a homecoming flag carrier
					// (flag can only be returned when the own flag is at home, this collision with the own flag
					// in base is used as an indicator whether the flag was successfully captured)
					if(m_flagCarriers[i] && (pEntityReachedObjectiveMessage->GetEntity()->GetId() == m_flagCarriers[i]->GetId()))
					{
						// The flag carrier reached the base position of his own flag -> captured
						FlagCaptured(pEntityReachedObjectiveMessage->GetObjective()->GetTeam());
					}
				}
			}
			break;
		case Dropped:
			if(pEntityReachedObjectiveMessage->GetEntity()->GetTeam() != pEntityReachedObjectiveMessage->GetObjective()->GetTeam())
			{
				// An entity of a hostile team reached the flag, make the entity the new flag carrier
				FlagPickedUp(pEntityReachedObjectiveMessage->GetObjective()->GetTeam(), pEntityReachedObjectiveMessage->GetEntity());
			}else
			{
				// An entity of the team the flag belongs to has reached the flag -> return it
				FlagReturned(pEntityReachedObjectiveMessage->GetObjective()->GetTeam());
			}
			break;
		}
		break;
		}
	}
}
*/
//--------------------------------------------------------------------------------------
// Called when a flag is picked up.
// Param1: The team, whose flag was picked up, that is stolen.
// Param2: A pointer to the entity that stole the flag.
//--------------------------------------------------------------------------------------
void MultiflagCTFGameContext::FlagPickedUp(EntityTeam flagOwner, Entity* pCarrier)
{
	m_flagStates[flagOwner]   = Stolen;
	m_flagCarriers[flagOwner] = pCarrier;

	// Notify teams.
	FlagPickedUpMessageData data(flagOwner, pCarrier->GetId());
	BroadcastMessage(FlagPickedUpMessageType, &data);
}

//--------------------------------------------------------------------------------------
// Called when a flag is dropped.
// Param1: The team, whose flag was dropped.
//--------------------------------------------------------------------------------------
void MultiflagCTFGameContext::FlagDropped(EntityTeam flagOwner)
{
	m_flagStates[flagOwner]      = Dropped;
	m_flagCarriers[flagOwner]    = nullptr;
	m_flagResetTimers[flagOwner] = m_flagResetTimeLimit;

	// Notify teams.
	FlagDroppedMessageData data(flagOwner, m_flagPositions[flagOwner]);
	BroadcastMessage(FlagDroppedMessageType, &data);
}

//--------------------------------------------------------------------------------------
// Called when a flag is returned.
// Param1: The team, whose flag was returned.
//--------------------------------------------------------------------------------------
void MultiflagCTFGameContext::FlagReturned(EntityTeam flagOwner)
{
	m_flagResetTimers[flagOwner] = m_flagResetTimeLimit;
	m_flagStates[flagOwner]		 = InBase;	    
	m_flagPositions[flagOwner]	 = m_flagBasePositions[flagOwner];

	m_flags[flagOwner]->SetPosition(m_flagPositions[flagOwner]);
	m_flags[flagOwner]->UpdateColliderPosition(m_flagPositions[flagOwner]);

	// Notify teams.
	FlagReturnedMessageData data(flagOwner);
	BroadcastMessage(FlagReturnedMessageType, &data);
}

//--------------------------------------------------------------------------------------
// Called when a flag is captured.
// Param1: The team, whose flag was captured.
//--------------------------------------------------------------------------------------
void MultiflagCTFGameContext::FlagCaptured(EntityTeam flagOwner)
{
	if(flagOwner == TeamRed)
	{
		AddScore(TeamBlue, 1);
	}else
	{
		AddScore(TeamRed, 1);
	}

	m_flagCarriers[flagOwner] = nullptr;
	FlagReturned(flagOwner);
}

// Data access functions

float MultiflagCTFGameContext::GetFlagResetTimeLimit(void) const
{
	return m_flagResetTimeLimit;
}

float MultiflagCTFGameContext::GetFlagResetTimeLeft(EntityTeam team) const
{
	return m_flagResetTimers[team];
}

FlagState MultiflagCTFGameContext::GetFlagState(EntityTeam team) const
{
	return m_flagStates[team];
}

const XMFLOAT2& MultiflagCTFGameContext::GetFlagBasePosition(EntityTeam team) const
{
	return m_flagBasePositions[team];
}

const XMFLOAT2& MultiflagCTFGameContext::GetFlagPosition(EntityTeam team) const
{
	return m_flagPositions[team];
}

const Entity* MultiflagCTFGameContext::GetFlagCarrier(EntityTeam team) const
{
	return m_flagCarriers[team];
}

void MultiflagCTFGameContext::SetFlagResetTimeLimit(float timeLimit)
{
	m_flagResetTimeLimit = timeLimit;
}

void MultiflagCTFGameContext::SetFlagBasePosition(EntityTeam team, const XMFLOAT2& basePosition)
{
	m_flagBasePositions[team] = basePosition;
}

void MultiflagCTFGameContext::SetFlag(EntityTeam flagOwner, Objective* pCollidableObject)
{
	m_flags[flagOwner] = pCollidableObject;
}
