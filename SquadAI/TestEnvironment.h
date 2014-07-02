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

	void AddEntity(EntityType type, const XMFLOAT3& position, float rotation);

	void GetGridPosition(const XMFLOAT3& worldPos, XMFLOAT3& gridPos);
private:
	TestEnvironmentData m_data;  // Contains initialisation data for the environment, especially the grid to be created
	GridField**         m_pGrid; // The grid the test application is using and on which entities are placed and moved around

	float               m_horizontalSpacing;
	float               m_verticalSpacing;

	std::vector<Soldier>	   m_teamA;
	std::vector<Soldier>	   m_teamB;
	std::vector<CoverPosition> m_coverSpots;
};

#endif // TEST_ENVIRONMENT_H
