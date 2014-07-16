/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironment.cpp
*  Encapsulates a test setup and contains all entities
*  that are part of it.
*/

#include "TestEnvironment.h"


TestEnvironment::TestEnvironment(void) : m_id(0),
										 m_pGrid(nullptr),
										 m_pNodes(nullptr),
										 m_gridSize(0.0f),
										 m_numberOfGridPartitions(0),
										 m_gridSpacing(0.0f),
										 m_isPaused(true)
{
}

TestEnvironment::~TestEnvironment(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Initialise the test environment
// Param1: The size of a side of the square grid.
// Param2: The number of grid fields along one axis of the square grid
// Returns true if the test environment was successfully initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::Initialise(float gridSize, unsigned int numberOfGridPartitions)
{
	m_gridSize			     = gridSize;
	m_numberOfGridPartitions = numberOfGridPartitions;

	return InitialiseGrid() && m_pathfinder.Initialise(this);
}

//--------------------------------------------------------------------------------------
// Updates the test environment.
// Param1: The render context that is used to keep track of entities within the environment to be drawn.
// Param2: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void TestEnvironment::Update(RenderContext& pRenderContext, float deltaTime)
{

	// Update entities and add them to the render context after calculating their transforms

	for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
	{
		if(!m_isPaused)
		{
			it->Update(deltaTime);
		}

		XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
		XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - it->GetRotation()));
		XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetScale(), it->GetScale(), 1.0f);

		XMFLOAT4X4 transform;
		XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);
		pRenderContext.AddInstance(ASoldier, transform);
	}

	for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
	{
		if(!m_isPaused)
		{
			it->Update(deltaTime);
		}

		XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
		XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - it->GetRotation()));
		XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetScale(), it->GetScale(), 1.0f);

		XMFLOAT4X4 transform;
		XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);
		pRenderContext.AddInstance(BSoldier, transform);
	}
	
	for(std::list<CoverPosition>::iterator it = m_coverSpots.begin(); it != m_coverSpots.end(); ++it)
	{
		XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
		XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f -it->GetRotation()));
		XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetScale(), it->GetScale(), 1.0f);

		XMFLOAT4X4 transform;
		XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);
		pRenderContext.AddInstance(CoverSpot, transform);
	}
}

//--------------------------------------------------------------------------------------
// Releases resources used by the test environment.
//--------------------------------------------------------------------------------------
void TestEnvironment::Cleanup()
{
	CleanupGrid();
}

//--------------------------------------------------------------------------------------
// Adds a new entity to the test environment.
// Param1: The type of the entity that should be added.
// Param2: The world position, at which the entity should be added (that is a the correspondign grid field).
// Param3: The rotation to apply to the new entity.
// Returns true if the entity was successfully added and initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::AddEntity(EntityType type, const XMFLOAT2& position, float rotation)
{
	// Set the entity to the centre of the grid field denoted by the passed in coordinates
	XMFLOAT2 updatedPosition;
	XMFLOAT2 gridPosition;

	WorldToGridPosition(position, gridPosition);

	if(gridPosition.x < 0)
	{
		// Entity cannot be placed outside of the grid.
		return false;
	}

	if(!m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_isEmpty)
	{
		// Field is not empty, do not place entity on top of others
		return false;
	}

	GridToWorldPosition(gridPosition, updatedPosition);

	switch(type)
	{
	case ASoldier:
		{
			Soldier soldier(++m_id, type, updatedPosition, rotation, m_gridSpacing, m_gridSpacing * 0.5f, this, EntityMovementData(XMFLOAT2(0.0f, 0.0f), g_kSoldierMaxVelocity, g_kSoldierMaxForce, g_kSoldierMaxSeeAhead));
			m_teamA.push_back(soldier);
			m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_pEntity = &m_teamA.back();
		}
		break;
	case BSoldier:
		{
			Soldier soldier(++m_id, type, updatedPosition, rotation, m_gridSpacing, m_gridSpacing * 0.5f, this, EntityMovementData(XMFLOAT2(0.0f, 0.0f), g_kSoldierMaxVelocity, g_kSoldierMaxForce, g_kSoldierMaxSeeAhead));
			m_teamB.push_back(soldier);
			m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_pEntity = &m_teamB.back();
		}
		break;
	case CoverSpot:
		m_coverSpots.push_back(CoverPosition(++m_id, type, updatedPosition, rotation, m_gridSpacing, m_gridSpacing , this));  //* 0.5f
		// Update the graph
		UpdateCoverMap(m_pNodes[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)], false);
		m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_pEntity = &m_coverSpots.back();
		break;
	}

	// Update the grid field
	m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_isEmpty  = false;
	m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_id       = m_id;
	m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_type     = type;
	m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_rotation = rotation;

	return true;
}

//--------------------------------------------------------------------------------------
// Removes an entity from the test environment.
// Param1: The world position, the corresponding grid field of which holds the entity to be deleted.
// Returns true if the entity was successfully removed, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::RemoveEntity(const XMFLOAT2& position)
{
	XMFLOAT2 gridPosition;
	WorldToGridPosition(position, gridPosition);

	if(gridPosition.x < 0)
	{
		// Cannot delete entities that are not placed on the grid
		return false;
	}

	if(!m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_isEmpty)
	{
		unsigned long deleteId = m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_id;

		switch(m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_type)
		{
		case ASoldier:
			{
				std::list<Soldier>::iterator deleteIterator = std::find_if(m_teamA.begin(), m_teamA.end(), FindEntityById<Soldier>(deleteId));

				if(deleteIterator != m_teamA.end())
				{
					// Delete the entity
					m_teamA.erase(deleteIterator);
				}
			}
			break;
		case BSoldier:
			{
				std::list<Soldier>::iterator deleteIterator = std::find_if(m_teamB.begin(), m_teamB.end(), FindEntityById<Soldier>(deleteId));

				if(deleteIterator != m_teamB.end())
				{
					// Delete the entity
					m_teamB.erase(deleteIterator);
				}
			}
			break;
		case CoverSpot:
			{
				std::list<CoverPosition>::iterator deleteIterator = std::find_if(m_coverSpots.begin(), m_coverSpots.end(), FindEntityById<CoverPosition>(deleteId));

				if(deleteIterator != m_coverSpots.end())
				{
					// Update the test environment graph
					UpdateCoverMap(m_pNodes[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)], true);

					// Delete the entity
					m_coverSpots.erase(deleteIterator);
				}
			}
			break;
		}

		// Mark grid field as empty
		m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_id = 0;
		m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_isEmpty = true;

		return true;
	}else
	{
		// Nothing to delete in that grid field
		return false;
	}
}

//--------------------------------------------------------------------------------------
// Calculates the grid field corresponding to a given point in world space.
// Param1: The world position to convert to a grid position.
// Param2: Out parameter that will hold the grid position in (grid fields). Negative values indicate
//         that the passed in world position does not lie within the grid. The z-value is simply passed through.
//--------------------------------------------------------------------------------------
void TestEnvironment::WorldToGridPosition(const XMFLOAT2& worldPos, XMFLOAT2& gridPos) const
{
	// Assumes that the grid centre is at (0,0)

	// Check for invalid world positions that don't lie within the grid
	if(abs(worldPos.x) > (m_gridSize * 0.5f) || abs(worldPos.y) > (m_gridSize * 0.5f))
	{
		gridPos.x = -1.0f;
		gridPos.y = -1.0f;
	}else
	{
		// World position lies within the grid

		gridPos.x = static_cast<float>(static_cast<int>((worldPos.x + m_gridSize * 0.5f) / m_gridSpacing));
		gridPos.y = static_cast<float>(static_cast<int>((worldPos.y + m_gridSize * 0.5f) / m_gridSpacing));
	}
}

//--------------------------------------------------------------------------------------
// Calculates the grid field corresponding to a given point in world space.
// Param1: The grid position to convert into world space.
// Param2: Out parameter that will hold he world position corresponding to the centre of the grid field.
//--------------------------------------------------------------------------------------
void TestEnvironment::GridToWorldPosition(const XMFLOAT2& gridPos, XMFLOAT2& worldPos) const
{
	worldPos.x = gridPos.x * m_gridSpacing + m_gridSpacing * 0.5f - m_gridSize * 0.5f;
	worldPos.y = gridPos.y * m_gridSpacing + m_gridSpacing * 0.5f - m_gridSize * 0.5f;
}

//--------------------------------------------------------------------------------------
// Save this test environment to a file.
// Param1: The name/patch of the file, to which the test environment should be saved.
// Returns true if the test environment was successfully saved to the provided file.
//--------------------------------------------------------------------------------------
bool TestEnvironment::Save(std::string filename)
{
	std::ofstream out(filename);

	if(out.good())
	{
		// Save the test environment data

		out << m_gridSize << " " << m_numberOfGridPartitions << "\n";

		// Save the entities

		// Save soldiers of team A
		for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
		{
			out << it->GetType() << " " << it->GetPosition().x << " " << it->GetPosition().y << " " << it->GetRotation() << "\n";
		}

		// Save soldiers of team B
		for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
		{
			out << it->GetType() << " " << it->GetPosition().x << " " << it->GetPosition().y << " " << it->GetRotation() << "\n";
		}

		// Save cover spots
		for(std::list<CoverPosition>::iterator it = m_coverSpots.begin(); it != m_coverSpots.end(); ++it)
		{
			out << it->GetType() << " " << it->GetPosition().x << " " << it->GetPosition().y << " " << it->GetRotation() << "\n";
		}

		out.close();
		return true;
	}

	// Opening of the file failed
	return false;
}
	
//--------------------------------------------------------------------------------------
// Load a test environment from a file.
// Param1: The name/patch of the file, from which the test environment should be loaded.
// Returns true if the test environment was successfully loaded from the provided file.
//--------------------------------------------------------------------------------------
bool TestEnvironment::Load(std::string filename)
{
	std::ifstream in(filename);

	if(in.good())
	{
		// Delete the old test environment data
		// Note: Make a safety copy to be able to revert in case the loading fails.

		CleanupGrid();

		m_teamA.clear();
		m_teamB.clear();
		m_coverSpots.clear();

		m_id = 0;

		// Load the test environment data

		std::string lineOfFile;
		getline(in, lineOfFile);
		std::istringstream iss(lineOfFile);

		iss >> m_gridSize >> m_numberOfGridPartitions;

		// Initialise a new grid with the new data
		InitialiseGrid();
		// Update the pathfinder
		m_pathfinder.UpdateWeights();

		// Load the entities

		while(in.good())
		{
			getline(in, lineOfFile);
			std::istringstream iss(lineOfFile);

			int        type;
			XMFLOAT2   position;
			float      rotation;

			iss >> type >> position.x >> position.y >> rotation;

			AddEntity(EntityType(type), position, rotation);
		}

		in.close();
		return true;
	}

	// Opening of the file failed
	return false;
}

//--------------------------------------------------------------------------------------
// Determines the closest object in a moving entity's movement direction that it could
// collide with. Mostly based on: http://stackoverflow.com/questions/1073336/circle-line-collision-detection
// Param1: The moving entity, for which collisions should be checked.
// Returns a pointer to the object that the passed in entity will collide with if it 
// keeps moving in the current direction. Returns a nullpointer if there is no collision
// object in sight.
//--------------------------------------------------------------------------------------
const Entity* TestEnvironment::GetCollisionObject(const MovingEntity& entity)
{
	// Use square distances throughout to save on using square root.
	// Make sure that colliding objects are within the see ahead range
	float shortestCollisionDistance = entity.GetMaxSeeAhead() * entity.GetMaxSeeAhead();

	Entity* pCollisionObject = nullptr;

	// The normalised movement direction of the entity
	XMVECTOR normDirection = XMVector2Normalize(XMLoadFloat2(&entity.GetVelocity()));

	/*
	// Check other soldiers for collisions
	for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
	{
		// Don't check for collision of the entity with itself
		if(it->GetId() != entity.GetId())
		{
			// The vector from the centre of the radius surrounding the possible collision object to the start of the 
			// direction vector of the entity. This equals the vector from the centre of the collision object to the
			// centre of the entity.
			XMVECTOR colObjectToEntity = XMLoadFloat2(&entity.GetPosition()) - XMLoadFloat2(&it->GetPosition());

			float a;
			float b;
			float c;

			float radiusSquare = it->GetRadius() * it->GetRadius();

			XMStoreFloat(&a, XMVector2Dot(normDirection, normDirection));
			XMStoreFloat(&b, 2.0f * XMVector2Dot(colObjectToEntity, normDirection));
			XMStoreFloat(&c, XMVector2Dot(colObjectToEntity, colObjectToEntity) - XMLoadFloat(&radiusSquare));

			float discriminant = b * b - 4 * a * c;

			// If discriminant is smaller than 0, there is no intersection
			if(discriminant >= 0)
			{
				// There is an intersection of the movement vector and the object. 

				// Note: The intersection points could now be deduced but for the moment it is sufficient
				//       to simply know that an object intersects and to determine the closest colliding object.

				// Get the square distance between the object and the entity
				float distance; 
				XMStoreFloat(&distance, XMVector2Dot(colObjectToEntity, colObjectToEntity));

				if(distance <= shortestCollisionDistance)
				{
					pCollisionObject = &(*it);
					shortestCollisionDistance = distance;
				}
			}
		}
	}

	for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
	{
		// Don't check for collision of the entity with itself
		if(it->GetId() != entity.GetId())
		{
			// The vector from the centre of the radius surrounding the possible collision object to the start of the 
			// direction vector of the entity. This equals the vector from the centre of the collision object to the
			// centre of the entity.
			XMVECTOR colObjectToEntity = XMLoadFloat2(&entity.GetPosition()) - XMLoadFloat2(&it->GetPosition());

			float a;
			float b;
			float c;

			float radiusSquare = it->GetRadius() * it->GetRadius();

			XMStoreFloat(&a, XMVector2Dot(normDirection, normDirection));
			XMStoreFloat(&b, 2.0f * XMVector2Dot(colObjectToEntity, normDirection));
			XMStoreFloat(&c, XMVector2Dot(colObjectToEntity, colObjectToEntity) - XMLoadFloat(&radiusSquare));

			float discriminant = b * b - 4 * a * c;

			// If discriminant is smaller than 0, there is no intersection
			if(discriminant >= 0)
			{
				// There is an intersection of the movement vector and the object. 

				// Note: The intersection points could now be deduced but for the moment it is sufficient
				//       to simply know that an object intersects and to determine the closest colliding object.

				// Get the square distance between the object and the entity
				float distance; 
				XMStoreFloat(&distance, XMVector2Dot(colObjectToEntity, colObjectToEntity));

				if(distance <= shortestCollisionDistance)
				{
					pCollisionObject = &(*it);
					shortestCollisionDistance = distance;
				}
			}
		}
	}
	*/
	for(std::list<CoverPosition>::iterator it = m_coverSpots.begin(); it != m_coverSpots.end(); ++it)
	{
		// The vector from the centre of the radius surrounding the possible collision object to the start of the 
		// direction vector of the entity. This equals the vector from the centre of the collision object to the
		// centre of the entity.
		XMVECTOR colObjectToEntity = XMLoadFloat2(&entity.GetPosition()) - XMLoadFloat2(&it->GetPosition());

		float a;
		float b;
		float c;

		float radiusSquare = it->GetRadius() * it->GetRadius();

		XMStoreFloat(&a, XMVector2Dot(normDirection, normDirection));
		XMStoreFloat(&b, 2.0f * XMVector2Dot(colObjectToEntity, normDirection));
		XMStoreFloat(&c, XMVector2Dot(colObjectToEntity, colObjectToEntity) - XMLoadFloat(&radiusSquare));

		float discriminant = b * b - 4 * a * c;

		// If discriminant is smaller than 0, there is no intersection
		if(discriminant >= 0)
		{
			// There is an intersection of the movement vector and the object. 

			// Note: The intersection points could now be deduced but for the moment it is sufficient
			//       to simply know that an object intersects and to determine the closest colliding object.

			// Get the square distance between the object and the entity
			float distance; 
			XMStoreFloat(&distance, XMVector2Dot(colObjectToEntity, colObjectToEntity));

			if(distance <= shortestCollisionDistance)
			{
				pCollisionObject = &(*it);
				shortestCollisionDistance = distance;
			}
		}
		
	}

	return pCollisionObject;
}

//--------------------------------------------------------------------------------------
// Starts the simulation, switches from edit to simulation mode.
//--------------------------------------------------------------------------------------
void TestEnvironment::StartSimulation(void)
{
	for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
	{
		it->Initialise();
	}

	for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
	{
		it->Initialise();
	}

	m_isPaused = false;
}

//--------------------------------------------------------------------------------------
// Stops the simulation and resets the test environment into its starting state.
//--------------------------------------------------------------------------------------
void TestEnvironment::EndSimulation(void)
{
	for(unsigned int i = 0; i < m_numberOfGridPartitions; ++i)
	{
		for(unsigned int k = 0; k < m_numberOfGridPartitions; ++k)
		{
			if(!m_pGrid[i][k].m_isEmpty)
			{
				if(m_pGrid[i][k].m_type == ASoldier || m_pGrid[i][k].m_type == BSoldier)
				{
					XMFLOAT2 position;
					GridToWorldPosition(XMFLOAT2(i, k), position);
					m_pGrid[i][k].m_pEntity->SetPosition(position);
					m_pGrid[i][k].m_pEntity->SetRotation(m_pGrid[i][k].m_rotation);
				}
			}
		}
	}

	m_isPaused = true;
}

//--------------------------------------------------------------------------------------
// Pauses the simulation.
//--------------------------------------------------------------------------------------
void TestEnvironment::PauseSimulation(void)
{
	m_isPaused = true;
}

//--------------------------------------------------------------------------------------
// Resumes the simulation from paused state.
//--------------------------------------------------------------------------------------
void TestEnvironment::ResumeSimulation(void)
{
	m_isPaused = false;
}

//--------------------------------------------------------------------------------------
// Initialise the grid containing the test environment
// Returns true if the grid was successfully initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::InitialiseGrid()
{
	m_gridSpacing = m_gridSize / static_cast<float>(m_numberOfGridPartitions);

	// Initialise the grid

	m_pGrid = new GridField*[m_numberOfGridPartitions];

	if(!m_pGrid)
	{
		return false;
	}

	for(unsigned int i = 0; i < m_numberOfGridPartitions; ++i)
	{
		m_pGrid[i] = new GridField[m_numberOfGridPartitions];
		
		if(!m_pGrid[i])
		{
			return false;
		}

		// All grid fields are initially empty
		m_pGrid[i]->m_isEmpty = true;
	}

	// Initialise the nodes

	m_pNodes = new Node*[m_numberOfGridPartitions];

	if(!m_pNodes)
	{
		return false;
	}

	for(unsigned int i = 0; i < m_numberOfGridPartitions; ++i)
	{
		m_pNodes[i] = new Node[m_numberOfGridPartitions];
		
		if(!m_pNodes[i])
		{
			return false;
		}
	}

	// Initialise the nodes

	for(unsigned int i = 0; i < m_numberOfGridPartitions; ++i)
	{
		for(unsigned int k = 0; k < m_numberOfGridPartitions; ++k)
		{
			XMFLOAT2 gridPos(static_cast<float>(i), static_cast<float>(k));
			XMFLOAT2 worldPos;

			GridToWorldPosition(gridPos, worldPos);

			// Use the array position as node ID
			m_pNodes[i][k].Initialise(i * m_numberOfGridPartitions + k, gridPos, worldPos, false);
		}
	}

	// Set up adjacency information for the nodes
	for(unsigned int i = 0; i < m_numberOfGridPartitions; ++i)
	{
		for(unsigned int k = 0; k < m_numberOfGridPartitions; ++k)
		{
			if(i > 0)
			{
				m_pNodes[i][k].AddAdjacentNode(&m_pNodes[i-1][k]);
				if(k > 0)
				{
					m_pNodes[i][k].AddAdjacentNode(&m_pNodes[i-1][k-1]);
				}
				if(k < m_numberOfGridPartitions-1)
				{
					m_pNodes[i][k].AddAdjacentNode(&m_pNodes[i-1][k+1]);
				}
			}

			if(i < m_numberOfGridPartitions-1)
			{
				m_pNodes[i][k].AddAdjacentNode(&m_pNodes[i+1][k]);
				if(k > 0)
				{
					m_pNodes[i][k].AddAdjacentNode(&m_pNodes[i+1][k-1]);
				}
				if(k < m_numberOfGridPartitions-1)
				{
					m_pNodes[i][k].AddAdjacentNode(&m_pNodes[i+1][k+1]);
				}
			}

			if(k > 0)
			{
				m_pNodes[i][k].AddAdjacentNode(&m_pNodes[i][k-1]);
			}

			if(k < m_numberOfGridPartitions-1)
			{
				m_pNodes[i][k].AddAdjacentNode(&m_pNodes[i][k+1]);
			}
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Releases resources allocated for the grid of the test environment.
//--------------------------------------------------------------------------------------
void TestEnvironment::CleanupGrid()
{
	if(m_pGrid)
	{
		for(unsigned int i = 0; i < m_numberOfGridPartitions; ++i)
		{
			delete[] m_pGrid[i];
			m_pGrid[i] = nullptr;
		}

		delete[] m_pGrid;
		m_pGrid = nullptr;
	}

	if(m_pNodes)
	{
		for(unsigned int i = 0; i < m_numberOfGridPartitions; ++i)
		{
			delete[] m_pNodes[i];
			m_pNodes[i] = nullptr;
		}

		delete[] m_pNodes;
		m_pNodes = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Updates the nodes of the graph according to the new cover spot added to the envrionment
// or a cover spot deleted from it.
// Param1: The node with the new cover position.
// Param2: Tells whether the cover position is added or deleted.
//--------------------------------------------------------------------------------------
void TestEnvironment::UpdateCoverMap(Node& coverNode, bool doDelete)
{
	coverNode.SetObstacle(!doDelete);

	int gridX = static_cast<int>(coverNode.GetGridPosition().x);
	int gridY = static_cast<int>(coverNode.GetGridPosition().y);

	// Update the adjacent nodes with regard to the added/deleted cover spot

	if(gridX > 0)
	{
		m_pNodes[gridX-1][gridY].SetCovered(East, !doDelete);
		if(gridY > 0)
		{
			m_pNodes[gridX-1][gridY-1].SetCovered(NorthEast, !doDelete);
		}
		if(gridY < m_numberOfGridPartitions-1)
		{
			m_pNodes[gridX-1][gridY+1].SetCovered(SouthEast, !doDelete);
		}
	}

	if(gridX < m_numberOfGridPartitions-1)
	{
		m_pNodes[gridX+1][gridY].SetCovered(West, !doDelete);
		if(gridY > 0)
		{
			m_pNodes[gridX+1][gridY-1].SetCovered(NorthWest, !doDelete);
		}
		if(gridY < m_numberOfGridPartitions-1)
		{
			m_pNodes[gridX+1][gridY+1].SetCovered(SouthWest, !doDelete);
		}
	}

	if(gridY > 0)
	{
		m_pNodes[gridX][gridY-1].SetCovered(North, !doDelete);
	}

	if(gridY < m_numberOfGridPartitions-1)
	{
		m_pNodes[gridX][gridY+1].SetCovered(South, !doDelete);
	}
}

// Data access functions
	
bool TestEnvironment::IsPaused(void) const
{
	return m_isPaused;
}

float TestEnvironment::GetGridSize(void) const
{
	return m_gridSize;
}
	
unsigned int TestEnvironment::GetNumberOfGridPartitions(void) const
{
	return m_numberOfGridPartitions;
}
	
float TestEnvironment::GetGridSpacing(void) const
{
	return m_gridSpacing;
}

Pathfinder& TestEnvironment::GetPathfinder(void) 
{
	return m_pathfinder;
}

Node** TestEnvironment::GetNodes(void)
{
	return m_pNodes;
}

const std::list<Soldier>& TestEnvironment::GetTeamA(void) const
{
	return m_teamA;
}

const std::list<Soldier>& TestEnvironment::GetTeamB(void) const
{
	return m_teamB;
}

const std::list<CoverPosition>& TestEnvironment::GetCoverSpots(void) const
{
	return m_coverSpots;
}