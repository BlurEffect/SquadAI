/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironment.h
*  Encapsulates a test setup and contains all entities
*  that are part of it.
*/

#ifndef TEST_ENVIRONMENT_H
#define TEST_ENVIRONMENT_H

// Includes
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "RenderContext.h"
#include "TestEnvironmentData.h"
#include "Soldier.h"
#include "CoverPosition.h"

using namespace DirectX;

class TestEnvironment
{
public:
	TestEnvironment(void);
	~TestEnvironment(void);

	bool Initialise(const TestEnvironmentData& initData);
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

	// Data access functions
	const TestEnvironmentData& GetData(void) const;

private:

	bool InitialiseGrid(void);
	void CleanupGrid(void);

	unsigned long       m_id;    // An id is assigned to each entity being created in the environment

	TestEnvironmentData m_data;  // Contains initialisation data for the environment, especially the grid to be created
	GridField**         m_pGrid; // The grid the test application is using and on which entities are placed and moved around

	float               m_horizontalSpacing;
	float               m_verticalSpacing;

	std::vector<Soldier>	   m_teamA;
	std::vector<Soldier>	   m_teamB;
	std::vector<CoverPosition> m_coverSpots;


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
			//Entity* pEntity = &entity;

			return entity.GetId() == m_id;
		}
	private:
		unsigned long m_id;
	};
};

#endif // TEST_ENVIRONMENT_H
