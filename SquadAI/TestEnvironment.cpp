/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironment.cpp
*  Encapsulates a test setup and contains all entities
*  that are part of it.
*/

#include "TestEnvironment.h"



TestEnvironment::TestEnvironment(void) : m_pGrid(nullptr),
										 m_horizontalSpacing(0.0f),
										 m_verticalSpacing(0.0f)
{
}

TestEnvironment::~TestEnvironment(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the test environment
// Param1: A data structure containing the initialisation data required by the test environment for setup.
// Returns true if the test environment was successfully initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::Initialise(const TestEnvironmentData& initData)
{
	m_data = initData;

	m_horizontalSpacing = m_data.m_gridWidth / static_cast<float>(m_data.m_gridHorizontalPartitions);
	m_verticalSpacing   = m_data.m_gridHeight / static_cast<float>(m_data.m_gridVerticalPartitions);

	m_pGrid = new GridField*[m_data.m_gridVerticalPartitions];

	if(!m_pGrid)
	{
		return false;
	}

	for(int i = 0; i < m_data.m_gridVerticalPartitions; ++i)
	{
		m_pGrid[i] = new GridField[m_data.m_gridHorizontalPartitions];
		
		if(!m_pGrid[i])
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Updates the test environment.
// Param1: The render context that is used to keep track of entities within the environment to be drawn.
//--------------------------------------------------------------------------------------
void TestEnvironment::Update(RenderContext& pRenderContext)
{
	// Add entities to the render context and calculate their transforms

	for(std::vector<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
	{
		XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, it->GetPosition().z);
		XMMATRIX rotationMatrix    = XMMatrixRotationZ(it->GetRotation());

		XMFLOAT4X4 transform;
		XMStoreFloat4x4(&transform, translationMatrix * rotationMatrix);
		pRenderContext.AddInstance(ASoldier, transform);
	}

	for(std::vector<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
	{
		XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, it->GetPosition().z);
		XMMATRIX rotationMatrix    = XMMatrixRotationZ(it->GetRotation());

		XMFLOAT4X4 transform;
		XMStoreFloat4x4(&transform, translationMatrix * rotationMatrix);
		pRenderContext.AddInstance(BSoldier, transform);
	}
	
	for(std::vector<CoverPosition>::iterator it = m_coverSpots.begin(); it != m_coverSpots.end(); ++it)
	{
		XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, it->GetPosition().z);
		XMMATRIX rotationMatrix    = XMMatrixRotationZ(it->GetRotation());

		XMFLOAT4X4 transform;
		XMStoreFloat4x4(&transform, translationMatrix * rotationMatrix);
		pRenderContext.AddInstance(CoverSpot, transform);
	}

	/*
	EntityType type(BSoldier);
	XMFLOAT3 position(0.0f, 0.0f, 0.0f);
	XMFLOAT3 rotation(0.0f, 0.0f, 0.0f);
	XMFLOAT3 scale(10.0f, 10.0f, 10.0f);

	XMFLOAT4X4 matTransform;
	XMStoreFloat4x4(&matTransform, XMMatrixIdentity());
	matTransform._11 = 1.0f;
	matTransform._22 = 1.0f;
	matTransform._33 = 1.0f;
	pRenderContext.AddInstance(type, matTransform);
	*/
}

//--------------------------------------------------------------------------------------
// Releases resources used by the test environment.
//--------------------------------------------------------------------------------------
void TestEnvironment::Cleanup()
{
	if(m_pGrid)
	{
		for(int i = 0; i < m_data.m_gridVerticalPartitions; ++i)
		{
			delete[] m_pGrid[i];
			m_pGrid[i] = nullptr;
		}

		delete[] m_pGrid;
		m_pGrid = nullptr;
	}
}


void TestEnvironment::AddEntity(EntityType type, const XMFLOAT3& position, float rotation)
{
	// Set the entity to the centre of the grid field denoted by the passed in coordinates
	XMFLOAT3 updatedPosition;

	GetGridPosition(position, updatedPosition);

	updatedPosition.x = updatedPosition.x * m_horizontalSpacing + m_horizontalSpacing * 0.5f - m_data.m_gridWidth * 0.5f;
	updatedPosition.y = updatedPosition.y  * m_verticalSpacing + m_verticalSpacing * 0.5f - m_data.m_gridHeight * 0.5f;

	switch(type)
	{
	case ASoldier:
		m_teamA.push_back(Soldier(type, updatedPosition, rotation));
		break;
	case BSoldier:
		m_teamB.push_back(Soldier(type, updatedPosition, rotation));
		break;
	case CoverSpot:
		m_coverSpots.push_back(CoverPosition(type, updatedPosition, rotation));
		break;
	}
}

//--------------------------------------------------------------------------------------
// Calculates the grid field corresponding to a given point in world space.
// Param1: The world position to convert to a grid position.
// Param2: Out parameter that will hold the grid position in (grid fields). Negative values indicate
//         that the passed in world position does not lie within the grid. The z-value is simply passed through.
//--------------------------------------------------------------------------------------
void TestEnvironment::GetGridPosition(const XMFLOAT3& worldPos, XMFLOAT3& gridPos)
{
	// Assumes that the grid centre is at (0,0)

	// Check for invalid world positions that don't lie within the grid
	if(abs(worldPos.x) > (m_data.m_gridWidth * 0.5f) || abs(worldPos.x) > (m_data.m_gridWidth * 0.5f))
	{
		gridPos.x = -1.0f;
		gridPos.y = -1.0f;
		gridPos.z = worldPos.z;
	}else
	{
		// World position lies within the grid

		gridPos.x = static_cast<float>(static_cast<int>((worldPos.x + m_data.m_gridWidth * 0.5f) / m_horizontalSpacing));
		gridPos.y = static_cast<float>(static_cast<int>((worldPos.y + m_data.m_gridHeight * 0.5f) / m_verticalSpacing));
		gridPos.z = worldPos.z;
	}
}

//--------------------------------------------------------------------------------------
// Save this test environment to a file.
// Param1: The name/patch of the file, to which the test environment should be saved.
// Returns true if the test environment was successfully saved to the provided file.
//--------------------------------------------------------------------------------------
bool TestEnvironment::Save(std::string filename)
{
	return true;
}
	
//--------------------------------------------------------------------------------------
// Load a test environment from a file.
// Param1: The name/patch of the file, from which the test environment should be loaded.
// Returns true if the test environment was successfully loaded from the provided file.
//--------------------------------------------------------------------------------------
bool TestEnvironment::Load(std::string filename)
{
	return true;
}