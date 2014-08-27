/* 
*  Kevin Meergans, SquadAI, 2014
*  Logger.h
*  This class contains static methods used to write messages and events to 
*  a log for debugging and evaluation purposes.
*/

#ifndef LOGGER_H
#define LOGGER_H

// Includes
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

// Forward declarations
class Entity;
class Behaviour;
class Projectile;
enum EntityTeam;
enum TeamManoeuvreType;

//--------------------------------------------------------------------------------------
// Lists all types of events that can be recorded.
//--------------------------------------------------------------------------------------
enum LogEventType
{
	IndividualBehaviourUpdatedLogEvent, // Called when an individual behaviour of an entity is updated
	EnemySpottedLogEvent,               // Called when an entity spots an enemy
	EntityHitLogEvent,					// Called when an entity is hit by a projectile
	EntityKilledLogEvent,				// Called when an entity was killed
	TeamManoeuvreInitLogEvent,				// Called when a team manoeuvre is initiated
	TeamManoeuvreTerminateLogEvent,			// Called when a team manoeuvre is terminated
	TeamManoeuvrePreconditionCheckLogEvent // Called when the preconditions of a team manoeuvre are checked
};

//--------------------------------------------------------------------------------------
// Bundles data that can be collected during a simulation.
//--------------------------------------------------------------------------------------
struct Statistics
{

};

class Logger
{
public:
	Logger(void);
	~Logger(void);

	bool Open(const char* filename);
	void LogEvent(LogEventType type, void* pObject1, void* pObject2);
	void Close(void);

private:
	void LogIndividualBehaviourUpdated(Entity* pEntity, Behaviour* pBehaviour);
	void LogEnemySpotted(Entity* pSpotter, Entity* pSpotted);
	void LogEntityHit(Entity* pHit, unsigned long* pShooterId);
	void LogEntityKilled(Entity* pKilled, unsigned long* pShooterId);
	void LogManoeuvreInit(EntityTeam* team, TeamManoeuvreType* manoeuvre);
	void LogManoeuvreTerminate(EntityTeam* team, TeamManoeuvreType* manoeuvre);
	void LogManoeuvrePreconditionCheck(EntityTeam* team, TeamManoeuvreType* manoeuvre);

	std::ofstream  m_out; // The out file stream that the logger uses to write messages to the file
};


#endif // LOGGER_H