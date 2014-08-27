/* 
*  Kevin Meergans, SquadAI, 2014
*  ObjectTypes.h
*  Contains an enumeration of all possible types of objects.
*/

#ifndef OBJECT_TYPES_H
#define OBJECT_TYPES_H


//--------------------------------------------------------------------------------------
// Identifies available teams that entities can belong to and other objects can be
// associated to.
//--------------------------------------------------------------------------------------
enum EntityTeam
{
	TeamRed,		// The red team  
	TeamBlue,		// The blue team
	None,			// Indicates that the entity doesn't belong to any team (initial value)
	NumberOfTeams
};

//--------------------------------------------------------------------------------------
// Global helper to get the string representation of a team name.
// Param1: Identifier for the team, for which to retrieve the name in string form.
// Returns a string containing the name of the team. If the provided team identifier is
// invalid, an empty string is returned.
//--------------------------------------------------------------------------------------
static const char* GetTeamName(EntityTeam team)
{
	switch(team)
	{
	case TeamRed:
		return "Team Red";
		break;
	case TeamBlue:
		return "Team Blue";
		break;
	case None:
		return "No Team";
		break;
	default:
		return "";
	}
}

//--------------------------------------------------------------------------------------
// Identifies the different types of objects that can be present in the test environment, 
// some of which are placable while others are created at runtime.
//--------------------------------------------------------------------------------------
enum ObjectType
{
	// Placable
	RedSoldierType,			// A soldier of team red
	BlueSoldierType,		// A soldier of team blue
	ObstacleType,			// An obstacle that blocks the way of soldiers
	RedFlagType,			// The flag of team red
	BlueFlagType,			// The flag of team blue
	RedBaseAreaType,		// A grid field that belongs to the red base
	BlueBaseAreaType,		// A grid field that belongs to the blue base
	RedSpawnPointType,		// A point, where red team members can respawn
	BlueSpawnPointType,		// A point, where blue team members can respawn
	// Non placable
	DeadRedSoldierType,		// A dead soldier of team red
	DeadBlueSoldierType,	// A dead soldier of team blue
	ProjectileType,			// A projectile shot by one of the soldiers
	NumberOfObjectTypes
};


#endif // OBJECT_TYPES_H
