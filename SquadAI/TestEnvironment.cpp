/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironment.cpp
*  Encapsulates a test setup and contains all entities
*  that are part of it.
*/

#include "TestEnvironment.h"

//--------------------------------------------------------------------------------------
// Initialise the test environment
// Param1: A data structure containing the initialisation data required by the test environment for setup.
// Returns true if the test environment was successfully initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::Initialise(const TestEnvironmentData& initData)
{
	m_data = initData;
	return true;
}

//--------------------------------------------------------------------------------------
// Updates the test environment.
// Param1: The render context that is used to keep track of entities within the environment to be drawn.
//--------------------------------------------------------------------------------------
void TestEnvironment::Update(RenderContext& pRenderContext)
{
	EntityType type(BSoldier);
	XMFLOAT3 position(0.0f, 0.0f, 0.0f);
	XMFLOAT3 rotation(0.0f, 0.0f, 0.0f);
	XMFLOAT3 scale(10.0f, 10.0f, 10.0f);

	XMFLOAT4X4 matTransform;
	XMStoreFloat4x4(&matTransform, XMMatrixIdentity());
	matTransform._11 = 100.0f;
	matTransform._22 = 100.0f;
	matTransform._33 = 100.0f;
	pRenderContext.AddInstance(type, matTransform);
}

//--------------------------------------------------------------------------------------
// Releases resources used by the test environment.
//--------------------------------------------------------------------------------------
void TestEnvironment::Cleanup()
{
}