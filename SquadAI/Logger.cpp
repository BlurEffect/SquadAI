/* 
*  Kevin Meergans, SquadAI, 2014
*  Logger.cpp
*  This class contains static methods used to write messages and events to 
*  a log for debugging and evaluation purposes.
*/

// Includes
#include "Logger.h"
#include "Entity.h"
#include "Behaviour.h"
#include "Projectile.h"
#include "TeamManoeuvre.h"
#include "ObjectTypes.h"

Logger::Logger(void)
{

}

Logger::~Logger(void)
{

}

//--------------------------------------------------------------------------------------
// Open a new log file to write to. The logger will always create a new log file 
// adding date and time information to the front of the provided filename.
// Param1: The filename of the file that should be created and written to.
// Returns true if the file could be created and opened successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Logger::Open(const char* filename)
{
	// Add data and time to filename
	time_t     currentTime = time(0);
    struct tm  timeStruct;
    char       buffer[80];
    localtime_s(&timeStruct, &currentTime);

	// Get a string with the current date and time information
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H-%M-%S", &timeStruct);

	// Build the filename for the logfile
	std::string s("Logs/");
	s.append(buffer);
	s.append(" - ");
	s.append(filename);

	// Open the logfile for writing
	m_out.open(s.c_str(), std::ofstream::out);

	return m_out.is_open();
}

//--------------------------------------------------------------------------------------
// Processes an event by writing it to a log file and/or updating the statistics for this
// simulation.
// Param1: A pointer to the first object involved in the event.
// Param2: A pointer to the second object involved in the event.
//--------------------------------------------------------------------------------------
void Logger::LogEvent(LogEventType type, void* pObject1, void* pObject2)
{
	switch(type)
	{
	case IndividualBehaviourUpdatedLogEvent:
		LogIndividualBehaviourUpdated(reinterpret_cast<Entity*>(pObject1), reinterpret_cast<Behaviour*>(pObject2));
		break;
	case EnemySpottedLogEvent:
		LogEnemySpotted(reinterpret_cast<Entity*>(pObject1), reinterpret_cast<Entity*>(pObject2));
		break;
	case EntityHitLogEvent:
		LogEntityHit(reinterpret_cast<Entity*>(pObject1), reinterpret_cast<unsigned long*>(pObject2));
		break;
	case EntityKilledLogEvent:
		LogEntityKilled(reinterpret_cast<Entity*>(pObject1), reinterpret_cast<unsigned long*>(pObject2));
		break;
	case TeamManoeuvreInitLogEvent:
		LogManoeuvreInit(reinterpret_cast<EntityTeam*>(pObject1), reinterpret_cast<TeamManoeuvreType*>(pObject2));
		break;
	case TeamManoeuvreTerminateLogEvent:
		LogManoeuvreTerminate(reinterpret_cast<EntityTeam*>(pObject1), reinterpret_cast<TeamManoeuvreType*>(pObject2));
		break;
	case TeamManoeuvrePreconditionCheckLogEvent:
		LogManoeuvrePreconditionCheck(reinterpret_cast<EntityTeam*>(pObject1), reinterpret_cast<TeamManoeuvreType*>(pObject2));
		break;
	}
}

//--------------------------------------------------------------------------------------
// Writes a new entry to the log file that tells that a certain behaviour was called
// for a certain entity.
// Param1: A pointer to the entity that the behaviour was called on.
// Param2: A pointer to the behaviour that was called on the entity.
//--------------------------------------------------------------------------------------
void Logger::LogIndividualBehaviourUpdated(Entity* pEntity, Behaviour* pBehaviour)
{
	if(m_out.is_open())
	{
		m_out << '\n' << "Entity with ID " << pEntity->GetId() << " of team " << pEntity->GetTeam() << " updated behaviour with id " << pBehaviour->GetId() << " and name " << pBehaviour->GetName() << '.';
	}
}

//--------------------------------------------------------------------------------------
// Writes a new entry to the log file that tells that an entity spotted an enemy.
// Param1: A pointer to the entity that spotted an enemy.
// Param2: A pointer to the hostile entity that was spotted.
//--------------------------------------------------------------------------------------
void Logger::LogEnemySpotted(Entity* pSpotter, Entity* pSpotted)
{
	if(m_out.is_open())
	{
		m_out << '\n' << "Entity with ID " << pSpotter->GetId() << " of team " << pSpotter->GetTeam() << " spotted hostile entity with ID " << pSpotted->GetId() << " of team " << pSpotted->GetTeam() << '.';
	}
}

//--------------------------------------------------------------------------------------
// Writes a new entry to the log file that tells that an entity was hit by a projectile
// fired by another entity.
// Param1: A pointer to the entity that was hit by a projectile.
// Param2: A pointer to the id of the entity that shot the projectile that killed the entity.
//--------------------------------------------------------------------------------------
void Logger::LogEntityHit(Entity* pHit, unsigned long* pShooterId)
{
	if(m_out.is_open())
	{
		m_out << '\n' << "Entity with ID " << pHit->GetId() << " of team " << pHit->GetTeam() << " was hit by a projectile fired from entity with ID " << (*pShooterId) << '.';
	}
}

//--------------------------------------------------------------------------------------
// Writes a new entry to the log file that tells that an entity was killed by a
// projectile fired by another entity.
// Param1: A pointer to the entity that was killed.
// Param2: A pointer to the id of the entity that shot the projectile that killed the entity.
//--------------------------------------------------------------------------------------
void Logger::LogEntityKilled(Entity* pKilled, unsigned long* pShooterId)
{
	if(m_out.is_open())
	{
		m_out << '\n' << "Entity with ID " << pKilled->GetId() << " of team " << pKilled->GetTeam() << " was killed by a projectile fired from entity with ID " << (*pShooterId) << '.';
	}
}

//--------------------------------------------------------------------------------------
// Writes a new entry to the log file that tells that a team manoeuvre was initiated by
// one of the teams.
// Param1: A pointer to the team identifier for the team that initiated the manoeuvre.
// Param2: A pointer to the identifier for the team manoeuvre that tells which type of
//         manoeuvre was initiated.
//--------------------------------------------------------------------------------------
void Logger::LogManoeuvreInit(EntityTeam* team, TeamManoeuvreType* manoeuvre)
{
	if(m_out.is_open())
	{
		m_out << '\n' << GetTeamName(*team) << " initiated manoeuvre " << TeamManoeuvre::GetManoeuvreNameFromType(*manoeuvre) << '.';
	}
}

//--------------------------------------------------------------------------------------
// Writes a new entry to the log file that tells that a team manoeuvre was terminated by
// one of the teams.
// Param1: A pointer to the team identifier for the team that terminated the manoeuvre.
// Param2: A pointer to the identifier for the team manoeuvre that tells which type of
//         manoeuvre was terminated.
//--------------------------------------------------------------------------------------
void Logger::LogManoeuvreTerminate(EntityTeam* team, TeamManoeuvreType* manoeuvre)
{
	if(m_out.is_open())
	{
		m_out << '\n' << GetTeamName(*team) << " terminated manoeuvre " << TeamManoeuvre::GetManoeuvreNameFromType(*manoeuvre) << '.';
	}
}

//--------------------------------------------------------------------------------------
// Writes a new entry to the log file that tells that the preconditions of a team manoeuvre 
// were checked by a team AI.
// Param1: A pointer to the team identifier for the team that checked the preconditions of the manoeuvre.
// Param2: A pointer to the identifier for the team manoeuvre that tells which type of
//         manoeuvre the preconditions were checked for.
//--------------------------------------------------------------------------------------
void Logger::LogManoeuvrePreconditionCheck(EntityTeam* team, TeamManoeuvreType* manoeuvre)
{
	if(m_out.is_open())
	{
		m_out << '\n' << GetTeamName(*team) << " checked preconditions for manoeuvre " << TeamManoeuvre::GetManoeuvreNameFromType(*manoeuvre) << '.';
	}
}

//--------------------------------------------------------------------------------------
// Closes the log file.
//--------------------------------------------------------------------------------------
void Logger::Close()
{
	if(m_out.is_open())
	{
		// Close the current logfile
		m_out.close();
	}
}

