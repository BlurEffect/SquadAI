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

#include "RenderContext.h"
#include "TestEnvironmentData.h"
#include "Soldier.h"
#include "CoverPosition.h"
#include "Node.h"
#include "Pathfinder.h"
#include "ApplicationSettings.h"


using namespace DirectX;

class TestEnvironment
{
public:
	TestEnvironment(void);
	~TestEnvironment(void);

	bool Initialise(float gridSize, unsigned int numberOfGridPartitions);
	void Update(RenderContext& pRenderContext);
	void Cleanup(void);

	bool AddEntity(EntityType type, const XMFLOAT2& position, float rotation);
	bool RemoveEntity(const XMFLOAT2& position);

	bool Save(std::string filename);
	bool Load(std::string filename);

	void StartSimulation(void);
	void EndSimulation(void);
	void PauseSimulation(void);
	void ResumeSimulation(void);

	void WorldToGridPosition(const XMFLOAT2& worldPos, XMFLOAT2& gridPos) const;
	void GridToWorldPosition(const XMFLOAT2& gridPos, XMFLOAT2& worldPos) const;

	const Entity& GetCollisionObject(const MovingEntity& entity);

	// Data access functions
	float		 GetGridSize(void) const;
	unsigned int GetNumberOfGridPartitions(void) const;
	float	     GetGridSpacing(void) const;

	Pathfinder&  GetPathfinder(void);
	Node**	     GetNodes(void);

private:

	bool InitialiseGrid(void);
	void CleanupGrid(void);
	void UpdateCoverMap(Node& coverNode, bool doDelete);


	unsigned long       m_id;    // An id is assigned to each entity being created in the environment

	GridField**         m_pGrid;  // The grid the test application is using and on which entities are placed in edit mode
	Node**              m_pNodes; // The graph made up of nodes representing the test environment when in simulation mode

	Pathfinder          m_pathfinder; // The pathfinder associated to this environment.

	float               m_gridSize;               // The size of the grid along x and y axis
	unsigned int        m_numberOfGridPartitions; // The number of grid fields along x and y axis
	float               m_gridSpacing;			  // The size of a grid field along x and y axis

	std::list<Soldier>	     m_teamA;      // Holds the soldier entities associated to Team A
	std::list<Soldier>	     m_teamB;      // Holds the soldier entities associated to Team B
	std::list<CoverPosition> m_coverSpots; // Holds the cover objects that were placed in the environment


	//--------------------------------------------------------------------------------------
	// Private functor used to find an entity within a container based on its id.
	//--------------------------------------------------------------------------------------
	template <class EntityTypeName>
	class FindEntityById
	{
	public:
		FindEntityById(unsigned long id) : m_id(id){}
		bool operator()(EntityTypeName entity)
		{
			return entity.GetId() == m_id;
		}
	private:
		unsigned long m_id;
	};
};

#endif // TEST_ENVIRONMENT_H
