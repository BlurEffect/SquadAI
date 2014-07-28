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
#include "ProjectileProperties.h"
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

	// 
	void GetNearbyObjects(const XMFLOAT2& position, float radius, EntityGroup entityGroup, std::multimap<float, CollidableObject*>& collisionObjects);

	//const Entity* GetCollisionObject(const MovingEntity& entity); // currently not used
	bool          CheckLineOfSight(int startGridX, int startGridY, int endGridX, int endGridY);

	//void          GetEnemies(const FightingEntity* pEntity, std::list<FightingEntity*>& enemies);

	// Data access functions
	float		 GetGridSize(void) const;
	unsigned int GetNumberOfGridPartitions(void) const;
	float	     GetGridSpacing(void) const;
	bool         IsPaused(void) const;

	//const std::list<Soldier>& GetTeamA(void) const;
	//const std::list<Soldier>& GetTeamB(void) const;
	//const std::list<CoverPosition>& GetCoverSpots(void) const;
	

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

	unsigned long       m_id;    // An id is assigned to each entity being created in the environment

	//GridField**         m_pGrid;  // The grid the test application is using and on which entities are placed in edit mode
	Node**              m_pNodes; // The graph made up of nodes representing the test environment when in simulation mode

	std::vector<EditModeObject> m_staticObjects; // The static test environment objects, as set up by the user in edit mode
	// Dynamic test environment objects

	Soldier m_soldiers[g_kSoldiersPerTeam * (NumberOfTeams-1)];
	Flag    m_flags[NumberOfTeams-1];

	std::list<Obstacle> m_obstacles;

	// -> soldiers, projectiles, flags


	Pathfinder          m_pathfinder; // The pathfinder associated to this environment.

	float               m_gridSize;               // The size of the grid along x and y axis
	unsigned int        m_numberOfGridPartitions; // The number of grid fields along x and y axis
	float               m_gridSpacing;			  // The size of a grid field along x and y axis

	//std::list<Soldier>	     m_teamA;       // Holds the soldier entities associated to Team A
	//std::list<Soldier>	     m_teamB;       // Holds the soldier entities associated to Team B
	//std::list<CoverPosition> m_coverSpots;  // Holds the cover objects that were placed in the environment
	std::list<Projectile>    m_projectiles; // Holds the currently active projectiles

	bool m_isPaused; // Tells whether the simulation running in the environment is currently paused
	bool m_isInEditMode;
	float m_objectScaleFactors[NumberOfObjectTypes]; // Determines the scale of the different objects in relation to a grid field

	unsigned int m_soldierCount[NumberOfTeams-1]; // Keeps track of how many soldiers have been placed
	bool		 m_flagSet[NumberOfTeams-1];		// Keeps track of the flags that have been placed

	unsigned int m_spawnPointCount[NumberOfTeams-1];
	std::vector<XMFLOAT2> m_spawnPoints[NumberOfTeams-1];

	//--------------------------------------------------------------------------------------
	// Private functor used to find an entity within a container based on its id.
	//--------------------------------------------------------------------------------------
	template <class EntityTypeName>
	class FindEntityById
	{
	public:
		FindEntityById(unsigned long id) : m_id(id){}
		bool operator()(const EntityTypeName& entity)
		{
			return entity.GetId() == m_id;
		}
	private:
		unsigned long m_id;
	};

	//--------------------------------------------------------------------------------------
	// Private functor used to find a basic object within a container based on the id of the
	// grid field it is placed on.
	//--------------------------------------------------------------------------------------
	class FindEditModeObjectByGridId
	{
	public:
		FindEditModeObjectByGridId(unsigned long gridId) : m_gridId(gridId){}
		bool operator()(const EditModeObject& basicObject)
		{
			return basicObject.GetGridId() == m_gridId;
		}
	private:
		unsigned long m_gridId;
	};
};

#endif // TEST_ENVIRONMENT_H
