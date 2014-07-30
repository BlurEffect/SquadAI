/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironment.h
*  Encapsulates a test setup and contains all entities
*  that are part of it.
*/

// Note: The test environments are currently limited to square grids.

#ifndef TEST_ENVIRONMENT_H
#define TEST_ENVIRONMENT_H

// Includes
#include <DirectXMath.h>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

#include "RenderContext.h"
#include "TestEnvironmentData.h"
#include "Soldier.h"
#include "Projectile.h"
#include "Node.h"
#include "Pathfinder.h"
#include "SoldierProperties.h"
#include "CircleCollider.h"
#include "AxisAlignedRectangleCollider.h"
#include "EditModeObject.h"
#include "ApplicationSettings.h"
#include "Flag.h"
#include "Obstacle.h"

class FightingEntity;
class MovingEntity;
class Entity;

using namespace DirectX;

class TestEnvironment
{
public:
	TestEnvironment(void);
	~TestEnvironment(void);

	bool Initialise(float gridSize, unsigned int numberOfGridPartitions);
	void Update(RenderContext& pRenderContext, float deltaTime);
	void Cleanup(void);

	bool AddObject(ObjectType type, const XMFLOAT2& position, float rotation);
	bool RemoveObjects(const XMFLOAT2& position);

	bool AddProjectile(EntityTeam friendlyTeam, const XMFLOAT2& origin, const XMFLOAT2& target);

	bool Save(std::string filename);
	bool Load(std::string filename);

	bool StartSimulation(void);
	void EndSimulation(void);
	void PauseSimulation(void);
	void ResumeSimulation(void);

	void WorldToGridPosition(const XMFLOAT2& worldPos, XMFLOAT2& gridPos) const;
	void GridToWorldPosition(const XMFLOAT2& gridPos, XMFLOAT2& worldPos) const;

	void GetNearbyObjects(const XMFLOAT2& position, float radius, EntityGroup entityGroup, std::multimap<float, CollidableObject*>& collisionObjects);

	//const Entity* GetCollisionObject(const MovingEntity& entity); // currently not used
	bool          CheckLineOfSight(int startGridX, int startGridY, int endGridX, int endGridY);

	// Data access functions
	float		 GetGridSize(void) const;
	unsigned int GetNumberOfGridPartitions(void) const;
	float	     GetGridSpacing(void) const;
	bool         IsPaused(void) const;

	Pathfinder&  GetPathfinder(void);
	Node**	     GetNodes(void);

private:

	bool PrepareSimulation(void);
	bool InitialiseGrid(void);
	void CleanupGrid(void);
	void UpdateCoverSpots(CollidableObject* obstacle);
	void UpdateBaseEntrances(void);
	void UpdateNodeGraph(void);
	bool CheckCollision(const CollidableObject* pCollidableObject,  const XMFLOAT2& oldPosition, EntityGroup entityGroup, CollidableObject*& outCollisionObject);

	unsigned long m_id;           // An id is assigned to each entity being created in the environment
	bool          m_isPaused;     // Tells whether the simulation running in the environment is currently paused
	bool          m_isInEditMode; // Tells whether the environment is in edit or simulation mode
	
	// Grid
	float               m_gridSize;               // The size of the grid along x and y axis
	unsigned int        m_numberOfGridPartitions; // The number of grid fields along x and y axis
	float               m_gridSpacing;			  // The size of a grid field along x and y axis
	Node**              m_pNodes;                 // The graph made up of nodes representing the test environment when in simulation mode

	// Objects
	std::vector<EditModeObject> m_staticObjects;									// The static test environment objects, as set up by the user in edit mode
	Soldier				        m_soldiers[g_kSoldiersPerTeam * (NumberOfTeams-1)]; // The soldier objects of all teams
	Flag                        m_flags[NumberOfTeams-1];						    // The flags of all teams
	std::list<Obstacle>         m_obstacles;										// The obstacles within the environment 
	std::list<Projectile>       m_projectiles;										// Holds the currently active projectiles
	std::vector<XMFLOAT2>       m_spawnPoints[NumberOfTeams-1];					    // Holds the spawn points of all teams

	Pathfinder   m_pathfinder;                              // The pathfinder associated to this environment.
	float        m_objectScaleFactors[NumberOfObjectTypes]; // Determines the scale of the different objects in relation to a grid field
	unsigned int m_soldierCount[NumberOfTeams-1];			// Keeps track of how many soldiers have been placed in edit mode
	bool		 m_flagSet[NumberOfTeams-1];				// Keeps track of the flags that have been placed in edit mode
	unsigned int m_spawnPointCount[NumberOfTeams-1];		// Keeps track of the spawn points that have been placed in edit mode
	

	
};

#endif // TEST_ENVIRONMENT_H
