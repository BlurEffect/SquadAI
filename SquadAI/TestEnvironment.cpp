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
										 m_isPaused(true),
										 m_isInEditMode(true)
{
	for(unsigned int i = 0; i < NumberOfTeams; ++i)
	{
		m_soldierCount[i] = 0;
		m_flagSet[i]      = false;
	}
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

	// Initialise the scale factors for the objects 
	m_objectScaleFactors[RedSoldierStartPositionType]	= 1.0f;
	m_objectScaleFactors[BlueSoldierStartPositionType]	= 1.0f;
	m_objectScaleFactors[ObstacleType]					= 1.0f; 
	m_objectScaleFactors[RedFlagStartPositionType]		= 0.5f;
	m_objectScaleFactors[BlueFlagStartPositionType]		= 0.5f;
	m_objectScaleFactors[RedBaseAreaType]				= 1.0f;
	m_objectScaleFactors[BlueBaseAreaType]				= 1.0f; 
	m_objectScaleFactors[RedSoldierType]				= 1.0f; 
	m_objectScaleFactors[BlueSoldierType]				= 1.0f; 
	m_objectScaleFactors[DeadRedSoldierType]			= 1.0f; 
	m_objectScaleFactors[DeadBlueSoldierType]			= 1.0f; 
	m_objectScaleFactors[RedFlagType]					= 0.5f; 
	m_objectScaleFactors[BlueFlagType]					= 0.5f;
	m_objectScaleFactors[ProjectileType]				= 0.2f;

	return InitialiseGrid() && m_pathfinder.Initialise(this);
}

//--------------------------------------------------------------------------------------
// Updates the test environment.
// Param1: The render context that is used to keep track of entities within the environment to be drawn.
// Param2: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void TestEnvironment::Update(RenderContext& pRenderContext, float deltaTime)
{
	for(std::vector<BasicStaticObject>::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); ++it)
	{
		if(m_isInEditMode || (it->GetType() != RedSoldierStartPositionType &&
							  it->GetType() != BlueSoldierStartPositionType &&
						      it->GetType() != RedFlagStartPositionType &&
							  it->GetType() != BlueFlagStartPositionType &&
							  it->GetType() != RedBaseAreaType &&
							  it->GetType() != BlueBaseAreaType))
		{
			XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - it->GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetUniformScale(), it->GetUniformScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);

			pRenderContext.AddInstance(it->GetType(), transform);
		}
	}

	if(!m_isInEditMode)
	{

		// Update soldiers

		for(unsigned int i = 0; i < g_kSoldiersPerTeam * NumberOfTeams; ++i)
		{
			if(!m_isPaused && m_soldiers[i].IsAlive())
			{
				m_soldiers[i].Update(deltaTime);
			}

			XMMATRIX translationMatrix = XMMatrixTranslation(m_soldiers[i].GetPosition().x, m_soldiers[i].GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - m_soldiers[i].GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(m_soldiers[i].GetScale(), m_soldiers[i].GetScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);

			switch(m_soldiers[i].GetTeam())
			{
			case TeamRed:
				if(m_soldiers[i].IsAlive())
				{
					pRenderContext.AddInstance(RedSoldierType, transform);
				}else
				{
					pRenderContext.AddInstance(DeadRedSoldierType, transform);
				}
			case TeamBlue:
				if(m_soldiers[i].IsAlive())
				{
					pRenderContext.AddInstance(BlueSoldierType, transform);
				}else
				{
					pRenderContext.AddInstance(DeadBlueSoldierType, transform);
				}
			}
		}

		// Update flags
		for(unsigned int i = 0; i < NumberOfTeams; ++i)
		{
			m_flags[i].Update(deltaTime);

			XMMATRIX translationMatrix = XMMatrixTranslation(m_flags[i].GetPosition().x, m_flags[i].GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - m_flags[i].GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(m_flags[i].GetUniformScale(), m_flags[i].GetUniformScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);

			switch(m_flags[i].GetTeam())
			{
			case TeamRed:
				pRenderContext.AddInstance(RedFlagType, transform);
			case TeamBlue:
				pRenderContext.AddInstance(BlueFlagType, transform);
			}
		}


		// Update entities and add them to the render context after calculating their transforms
		/*
		for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
		{
			if(!m_isPaused && it->IsAlive())
			{
				it->Update(deltaTime);
			}

			XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - it->GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetScale(), it->GetScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);

			if(it->IsAlive())
			{
				pRenderContext.AddInstance(RedSoldierType, transform);
			}else
			{
				pRenderContext.AddInstance(DeadRedSoldierType, transform);
			}
		}

		for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
		{
			if(!m_isPaused && it->IsAlive())
			{
				it->Update(deltaTime);
			}

			XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - it->GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetScale(), it->GetScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);
		
			if(it->IsAlive())
			{
				pRenderContext.AddInstance(BlueSoldierType, transform);
			}else
			{
				pRenderContext.AddInstance(DeadBlueSoldierType, transform);
			}
		}
	
		for(std::list<CoverPosition>::iterator it = m_coverSpots.begin(); it != m_coverSpots.end(); ++it)
		{
			if(!m_isPaused)
			{
				it->Update(deltaTime);
			}

			XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f -it->GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetScale(), it->GetScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);
			pRenderContext.AddInstance(ObstacleType, transform);
		}
		*/

		std::list<Projectile>::iterator it = m_projectiles.begin();
		while(it != m_projectiles.end())
		{
			XMFLOAT2 oldPos = it->GetPosition();

			if(!m_isPaused)
			{
				it->Update(deltaTime);
			}

			XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - it->GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetUniformScale(), it->GetUniformScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);
			pRenderContext.AddInstance(ProjectileType, transform);

			XMFLOAT2 newPos = it->GetPosition();

			// The entity that was hit by the projectile
			Entity* pHitEntity = nullptr;

			if(CheckCollision(it->GetCollider(), oldPos, GroupTeamBAndObstacles, pHitEntity))
			{
				if((pHitEntity->GetTeam() == TeamRed && it->GetFriendlyTeam() != TeamRed) ||
				   (pHitEntity->GetTeam() == TeamBlue && it->GetFriendlyTeam() != TeamBlue))
				{
					FightingEntity* pFightingEntity = reinterpret_cast<FightingEntity*>(pHitEntity);
					pFightingEntity->Hit(g_kProjectileDamage, it->GetDirection());
				}

				m_projectiles.erase(it++);
			}else
			{
				++it;
			}

		}
	}
	/*
	for(std::list<Projectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); ++it)
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
		pRenderContext.AddInstance(ProjectileType, transform);
	}
	*/
}

//--------------------------------------------------------------------------------------
// Releases resources used by the test environment.
//--------------------------------------------------------------------------------------
void TestEnvironment::Cleanup()
{
	CleanupGrid();
}

//--------------------------------------------------------------------------------------
// Prepares the test environment for simulation by creating dynamic objects, colliders
// and a node graph based on the objects placed by the user in edit mode. 
// Returns true if the simulation was prepared successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::PrepareSimulation(void)
{
	// Initialise soldiers and flags

	int soldierIndex = 0;

	for(std::vector<BasicStaticObject>::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); ++it)
	{
		switch(it->GetType())
		{
		case RedSoldierStartPositionType:
			{
			CircleCollider collider(it->GetPosition(), m_gridSpacing * 0.5f);
			bool a = m_soldiers[soldierIndex++].Initialise(EntityInitData(++m_id, TeamRed, RedSoldierType, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), CircleColliderType, &collider, this),
												  EntityMovementInitData(g_kSoldierMaxVelocity, g_kSoldierMaxForce, g_kSoldierMaxSeeAhead, g_kSoldierMaxCollisionAvoidanceForce, g_kSoldierMaxAvoidWallsForce, g_kSoldierMaxSeparationForce, g_kSoldierTargetReachedRadius, m_gridSpacing, m_gridSpacing),
												  EntitySensorInitData(g_kSoldierFieldOfView, g_kSoldierViewingDistance),
												  EntityCombatInitData(g_kSoldierMaxHealth));
			break;
			}
		case BlueSoldierStartPositionType:
			{
			CircleCollider collider(it->GetPosition(), m_gridSpacing * 0.5f);
			bool b = m_soldiers[soldierIndex++].Initialise(EntityInitData(++m_id, TeamBlue, BlueSoldierType, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), CircleColliderType, &collider, this),
												  EntityMovementInitData(g_kSoldierMaxVelocity, g_kSoldierMaxForce, g_kSoldierMaxSeeAhead, g_kSoldierMaxCollisionAvoidanceForce, g_kSoldierMaxAvoidWallsForce, g_kSoldierMaxSeparationForce, g_kSoldierTargetReachedRadius, m_gridSpacing, m_gridSpacing),
												  EntitySensorInitData(g_kSoldierFieldOfView, g_kSoldierViewingDistance),
												  EntityCombatInitData(g_kSoldierMaxHealth));
			break;
			}
		case RedFlagStartPositionType:
			{
			CircleCollider collider(it->GetPosition(), m_gridSpacing * 0.25f);
			m_flags[TeamRed].Initialise(TeamRed, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), g_kFlagResetTimer, &collider);
			break;
			}
		case BlueFlagStartPositionType:
			{
			CircleCollider collider(it->GetPosition(), m_gridSpacing * 0.25f);
			m_flags[TeamBlue].Initialise(TeamBlue, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), g_kFlagResetTimer, &collider);
			break;
			}
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Adds a new object to the test environment (edit mode only).
// Param1: The type of the object that should be added.
// Param2: The world position, at which the object should be added (that is a the corresponding grid field).
// Param3: The rotation to apply to the new entity.
// Returns true if the object was successfully added and initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::AddObject(ObjectType type, const XMFLOAT2& position, float rotation)
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

	if((type == RedSoldierStartPositionType && m_soldierCount[TeamRed] >= g_kSoldiersPerTeam) || 
	   (type == BlueSoldierStartPositionType && m_soldierCount[TeamBlue] >= g_kSoldiersPerTeam) ||
	   (type == RedFlagStartPositionType && m_flagSet[TeamRed]) ||
	   (type == BlueFlagStartPositionType && m_flagSet[TeamBlue]))
	{
		// Maximal number of flags/soldiers was already reached for a certain team.
		return false;
	}

	GridToWorldPosition(gridPosition, updatedPosition);

	// Get the ID for the grid field, that the entity was placed on
	unsigned long gridId = static_cast<int>(gridPosition.x) * m_numberOfGridPartitions + static_cast<int>(gridPosition.y);

	// Check if there are already static objects placed on this field
	std::vector<BasicStaticObject*> foundObjects;

	// Find all objects with the given grid id
	std::vector<BasicStaticObject>::iterator foundIt = std::find_if(m_staticObjects.begin(), m_staticObjects.end(), FindBasicObjectByGridId(gridId));
	while (foundIt != m_staticObjects.end()) {
		foundObjects.push_back(&(*foundIt));
		// Continue search from the position the last object was found.
		foundIt = std::find_if(++foundIt, m_staticObjects.end(), FindBasicObjectByGridId(gridId));
	}

	bool doAddObject = true;

	std::vector<BasicStaticObject*>::iterator it = foundObjects.begin();
	while(doAddObject && (it != foundObjects.end()))
	{
		// Make sure objects are only placed on top of each other if allowed
		doAddObject = !((type == (*it)->GetType()) ||
						((type != RedSoldierStartPositionType || ((*it)->GetType() != RedBaseAreaType)) && 
						 (type != RedBaseAreaType || ((*it)->GetType() != RedSoldierStartPositionType)) &&
						 (type != RedFlagStartPositionType || ((*it)->GetType() != RedBaseAreaType)) &&
					     (type != RedBaseAreaType || ((*it)->GetType() != RedFlagStartPositionType)) &&
						 (type != BlueSoldierStartPositionType || ((*it)->GetType() != BlueBaseAreaType)) &&
					     (type != BlueBaseAreaType || ((*it)->GetType() != BlueSoldierStartPositionType)) &&
					     (type != BlueFlagStartPositionType || ((*it)->GetType() != BlueBaseAreaType)) &&
					     (type != BlueBaseAreaType || ((*it)->GetType() != BlueFlagStartPositionType))
						));

		++it;
	}

	if(doAddObject)
	{
		// Add the new object
		m_staticObjects.emplace_back(gridId, type, updatedPosition, rotation, m_objectScaleFactors[type] * m_gridSpacing);

		if(type == RedSoldierStartPositionType)
		{
			++m_soldierCount[TeamRed];
		}else if(type == BlueSoldierStartPositionType)
		{
			++m_soldierCount[TeamBlue];
		}else if(type == RedFlagStartPositionType)
		{
			m_flagSet[TeamRed] = true;
		}else if(type == BlueFlagStartPositionType)
		{
			m_flagSet[TeamBlue] = true;
		}
	}

	
	/*
	if(!m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_isEmpty)
	{
		// Field is not empty, do not place entity on top of others
		return false;
	}

	GridToWorldPosition(gridPosition, updatedPosition);

	switch(type)
	{
	case RedSoldierType:
		{
			CircleCollider collider(updatedPosition, m_gridSpacing * 0.5f);
			m_teamA.push_back(Soldier());
			if(!m_teamA.back().Initialise(EntityInitData(++m_id, type, updatedPosition, rotation, m_gridSpacing, CircleColliderType, &collider, this),
				EntityMovementInitData(g_kSoldierMaxVelocity, g_kSoldierMaxForce, g_kSoldierMaxSeeAhead, g_kSoldierMaxCollisionAvoidanceForce, g_kSoldierMaxAvoidWallsForce, g_kSoldierMaxSeparationForce, g_kSoldierTargetReachedRadius, m_gridSpacing, m_gridSpacing),
										  EntitySensorInitData(g_kSoldierFieldOfView, g_kSoldierViewingDistance),
										  EntityCombatInitData(g_kSoldierMaxHealth)))
			{
				m_teamA.pop_back();
				return false;
			}

			m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_pEntity = &m_teamA.back();
		}
		break;
	case BlueSoldierType:
		{
			CircleCollider collider(updatedPosition, m_gridSpacing * 0.5f);
			m_teamB.push_back(Soldier());
			if(!m_teamB.back().Initialise(EntityInitData(++m_id, type, updatedPosition, rotation, m_gridSpacing, CircleColliderType, &collider, this),
										  EntityMovementInitData(g_kSoldierMaxVelocity, g_kSoldierMaxForce, g_kSoldierMaxSeeAhead, g_kSoldierMaxCollisionAvoidanceForce, g_kSoldierMaxAvoidWallsForce, g_kSoldierMaxSeparationForce, g_kSoldierTargetReachedRadius, m_gridSpacing, m_gridSpacing),
										  EntitySensorInitData(g_kSoldierFieldOfView, g_kSoldierViewingDistance),
										  EntityCombatInitData(g_kSoldierMaxHealth)))
			{
				m_teamB.pop_back();
				return false;
			}
			
			m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_pEntity = &m_teamB.back();
		}
		break;
	case ObstacleType:
		AxisAlignedRectangleCollider collider(updatedPosition, m_gridSpacing, m_gridSpacing);

		m_coverSpots.push_back(CoverPosition());
		if(!m_coverSpots.back().Initialise(EntityInitData(++m_id, type, updatedPosition, rotation, m_gridSpacing, AxisAlignedRectangleColliderType, &collider, this)))
		{
			m_coverSpots.pop_back();
			return false;
		}
		
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
	*/
	return true;
}

//--------------------------------------------------------------------------------------
// Removes all objects from a grid field of the test environment (only edit mode).
// Param1: The world position, the corresponding grid field of which holds the entity to be deleted.
// Returns true if the entity was successfully removed, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::RemoveObjects(const XMFLOAT2& position)
{
	XMFLOAT2 gridPosition;
	WorldToGridPosition(position, gridPosition);

	if(gridPosition.x < 0)
	{
		// Cannot delete entities that are not placed on the grid
		return false;
	}

	// Get the ID for the grid field, that the entity was placed on
	unsigned long gridId = static_cast<int>(gridPosition.x) * m_numberOfGridPartitions + static_cast<int>(gridPosition.y);

	// Find all objects with the given grid id to be removed

	std::vector<BasicStaticObject*> foundObjects;
	std::vector<BasicStaticObject>::iterator foundIt = std::find_if(m_staticObjects.begin(), m_staticObjects.end(), FindBasicObjectByGridId(gridId));
	while (foundIt != m_staticObjects.end()) {
		foundObjects.push_back(&(*foundIt));
		// Continue search from the position the last object was found.
		foundIt = std::find_if(++foundIt, m_staticObjects.end(), FindBasicObjectByGridId(gridId));
	}

	if(foundObjects.empty())
	{
		return false;
	}

	for(std::vector<BasicStaticObject*>::iterator it = foundObjects.begin(); it != foundObjects.end(); ++it)
	{
		// Update counts
		if((*it)->GetType() == RedSoldierStartPositionType)
		{
			--m_soldierCount[TeamRed];
		}else if((*it)->GetType() == BlueSoldierStartPositionType)
		{
			--m_soldierCount[TeamBlue];
		}else if((*it)->GetType() == RedFlagStartPositionType)
		{
			m_flagSet[TeamRed] = false;
		}else if((*it)->GetType() == BlueFlagStartPositionType)
		{
			m_flagSet[TeamBlue] = false;
		}
	}

	// Actual deletion of the objects
	std::vector<BasicStaticObject>::iterator removeIt = std::remove_if(m_staticObjects.begin(), m_staticObjects.end(), FindBasicObjectByGridId(gridId));
	m_staticObjects.erase(removeIt, m_staticObjects.end());
	return true;

	/*
	if(!m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_isEmpty)
	{
		unsigned long deleteId = m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_id;

		switch(m_pGrid[static_cast<int>(gridPosition.x)][static_cast<int>(gridPosition.y)].m_type)
		{
		case RedSoldierType:
			{
				std::list<Soldier>::iterator deleteIterator = std::find_if(m_teamA.begin(), m_teamA.end(), FindEntityById<Entity>(deleteId));

				if(deleteIterator != m_teamA.end())
				{
					// Delete the entity
					m_teamA.erase(deleteIterator);
				}
			}
			break;
		case BlueSoldierType:
			{
				std::list<Soldier>::iterator deleteIterator = std::find_if(m_teamB.begin(), m_teamB.end(), FindEntityById<Entity>(deleteId));

				if(deleteIterator != m_teamB.end())
				{
					// Delete the entity
					m_teamB.erase(deleteIterator);
				}
			}
			break;
		case ObstacleType:
			{
				std::list<CoverPosition>::iterator deleteIterator = std::find_if(m_coverSpots.begin(), m_coverSpots.end(), FindEntityById<Entity>(deleteId));

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
	*/
}

//--------------------------------------------------------------------------------------
// Adds a new projectile entity to the test environment.
// Param1: The team that fired the projectile.
// Param2: The position from which the projectile is being fired.
// Param3: The target position towards which the projectile is being fired.
// Returns true if the projectile was successfully added and initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::AddProjectile(EntityTeam friendlyTeam, const XMFLOAT2& origin, const XMFLOAT2& target)
{
	if((origin.x == target.x) && (origin.y == target.y))
	{
		return false;
	}

	// Note: Instead of using a vector and dynamic allocation, it might make sense to use a pool of pre-allocated
	//       projectiles.

	m_projectiles.push_back(Projectile(origin, 0.0f, m_objectScaleFactors[ProjectileType] * m_gridSpacing));

	XMFLOAT2 direction(0.0f, 0.0f);
	XMStoreFloat2(&direction, XMLoadFloat2(&target) - XMLoadFloat2(&origin));

	CircleCollider collider(origin, m_gridSpacing * m_objectScaleFactors[ProjectileType] * 0.5f);
	if(!m_projectiles.back().Initialise(direction, g_kProjectileSpeed, friendlyTeam, &collider))
	{
		m_projectiles.pop_back();
		return false;
	}

	return true;
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
	if(abs(worldPos.x) >= (m_gridSize * 0.5f) || abs(worldPos.y) >= (m_gridSize * 0.5f))
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

		// Save the static objects on the grid

		for(std::vector<BasicStaticObject>::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); ++it)
		{
			out << it->GetType() << " " << it->GetPosition().x << " " << it->GetPosition().y << " " << it->GetRotation() << "\n";
		}
		/*
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
		}*/

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

		//m_teamA.clear();
		//m_teamB.clear();
		//m_coverSpots.clear();
		m_staticObjects.clear();

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

			AddObject(ObjectType(type), position, rotation);
		}

		in.close();
		return true;
	}

	// Opening of the file failed
	return false;
}

//--------------------------------------------------------------------------------------
// Determines the entities of a specific group that are within a specified circle-shaped
// area around a given entity. This checks against the centres of entities. Does not include
// the entity itself in the return list.
// Param1: A pointer to the entity, for which the nearby entities should be found.
// Param2: The radius of the circle area.
// Param3: Specifies which set of entities will be checked and returned.
// Param4: A multimap that will hold the entities positioned within the radius, 
//         sorted by the square distance to the entity as key elements.
//--------------------------------------------------------------------------------------
void TestEnvironment::GetNearbyEntities(const Entity* pEntity, float radius, EntityGroup entityGroup, std::multimap<float, Entity*>& collisionObjects)
{
	// work on nodes, not on grid
	// get rid of grid
	// add more info to nodes (entrance, base, flag pos?, collider if obstacle)
	// create node graph
	// allow to create grid at runtime (console)
	// other key to start simulation (not return -> save load)
	// check test env methods if all works (uncomment original soldier/coverPos)
	// new structure for entity, soldier, etc
	// design BT
	// If all works -> Cleanup

	float squareRadius = radius * radius;
	float squareDistance = 0.0f;

	if(entityGroup != GroupObstacles) 
	{
		// Check soldiers
		for(unsigned int i = 0; i < g_kSoldiersPerTeam * NumberOfTeams; ++i)
		{
			if(it->GetId() != pEntity->GetId() && it->IsAlive())
			{
				XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&pEntity->GetPosition())));
				if(squareDistance <= squareRadius)
				{
					collisionObjects.insert(std::pair<float, Entity*>(squareDistance,(&(*it))));
				}
			}
		}
	}

	if(entityGroup == GroupTeamB || entityGroup == GroupAllSoldiers || entityGroup == GroupTeamBAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		// Check team B for collision
		for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
		{
			if(it->GetId() != pEntity->GetId() && it->IsAlive())
			{
				XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&pEntity->GetPosition())));
				if(squareDistance <= squareRadius)
				{
					collisionObjects.insert(std::pair<float, Entity*>(squareDistance,(&(*it))));
				}
			}
		}
	}

	if(entityGroup == GroupObstacles || entityGroup == GroupTeamAAndObstacles || entityGroup == GroupTeamBAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		// Only check nearby obstacles for collision
		unsigned int maxGridDistance = static_cast<unsigned int>(radius / m_gridSpacing) + 1;

		// Get the current grid position of the entity
		XMFLOAT2 gridPos;
		WorldToGridPosition(pEntity->GetPosition(), gridPos);

		unsigned int startX = (gridPos.x > maxGridDistance) ? (static_cast<int>(gridPos.x) - maxGridDistance) : 0;
		unsigned int startY = (gridPos.y > maxGridDistance) ? (static_cast<int>(gridPos.y) - maxGridDistance) : 0;
		unsigned int endX = (gridPos.x + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.x) + maxGridDistance) : (m_numberOfGridPartitions - 1);
		unsigned int endY = (gridPos.y + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.y) + maxGridDistance) : (m_numberOfGridPartitions - 1);

		// Check the grid within that distance for colliding obstacles
		for(unsigned int i = startX; i <= endX; ++i)
		{
			for(unsigned int k = startY; k <= endY; ++k)
			{
				if(!m_pGrid[i][k].m_isEmpty && (m_pGrid[i][k].m_type == ObstacleType) && (m_pGrid[i][k].m_id != pEntity->GetId()))
				{
					XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&m_pGrid[i][k].m_pEntity->GetPosition()) - XMLoadFloat2(&pEntity->GetPosition())));
					if(squareDistance <= squareRadius)
					{
						collisionObjects.insert(std::pair<float, Entity*>(squareDistance, m_pGrid[i][k].m_pEntity));
					}
				}
			}
		}
	}

	/*
	if(entityGroup == GroupTeamRed || entityGroup == GroupAllSoldiers || entityGroup == GroupTeamAAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		// Check team A
		for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
		{
			if(it->GetId() != pEntity->GetId() && it->IsAlive())
			{
				XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&pEntity->GetPosition())));
				if(squareDistance <= squareRadius)
				{
					collisionObjects.insert(std::pair<float, Entity*>(squareDistance,(&(*it))));
				}
			}
		}
	}

	if(entityGroup == GroupTeamB || entityGroup == GroupAllSoldiers || entityGroup == GroupTeamBAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		// Check team B for collision
		for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
		{
			if(it->GetId() != pEntity->GetId() && it->IsAlive())
			{
				XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&pEntity->GetPosition())));
				if(squareDistance <= squareRadius)
				{
					collisionObjects.insert(std::pair<float, Entity*>(squareDistance,(&(*it))));
				}
			}
		}
	}

	if(entityGroup == GroupObstacles || entityGroup == GroupTeamAAndObstacles || entityGroup == GroupTeamBAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		// Only check nearby obstacles for collision
		unsigned int maxGridDistance = static_cast<unsigned int>(radius / m_gridSpacing) + 1;

		// Get the current grid position of the entity
		XMFLOAT2 gridPos;
		WorldToGridPosition(pEntity->GetPosition(), gridPos);

		unsigned int startX = (gridPos.x > maxGridDistance) ? (static_cast<int>(gridPos.x) - maxGridDistance) : 0;
		unsigned int startY = (gridPos.y > maxGridDistance) ? (static_cast<int>(gridPos.y) - maxGridDistance) : 0;
		unsigned int endX = (gridPos.x + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.x) + maxGridDistance) : (m_numberOfGridPartitions - 1);
		unsigned int endY = (gridPos.y + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.y) + maxGridDistance) : (m_numberOfGridPartitions - 1);

		// Check the grid within that distance for colliding obstacles
		for(unsigned int i = startX; i <= endX; ++i)
		{
			for(unsigned int k = startY; k <= endY; ++k)
			{
				if(!m_pGrid[i][k].m_isEmpty && (m_pGrid[i][k].m_type == ObstacleType) && (m_pGrid[i][k].m_id != pEntity->GetId()))
				{
					XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&m_pGrid[i][k].m_pEntity->GetPosition()) - XMLoadFloat2(&pEntity->GetPosition())));
					if(squareDistance <= squareRadius)
					{
						collisionObjects.insert(std::pair<float, Entity*>(squareDistance, m_pGrid[i][k].m_pEntity));
					}
				}
			}
		}
	}
	*/
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
	XMVECTOR normDirection = XMVector2Normalize(XMLoadFloat2(&entity.GetViewDirection()));

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

	XMFLOAT2 lineEndPoint;
	XMStoreFloat2(&lineEndPoint, XMLoadFloat2(&entity.GetPosition()) + normDirection * entity.GetMaxSeeAhead());
	
	
	for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
	{
		// Don't check for collision of the entity with itself
		if(it->GetId() != entity.GetId() && it->IsAlive())
		{
			if(it->GetCollider()->CheckLineCollision(entity.GetPosition(), lineEndPoint))
			{
				XMVECTOR colObjectToEntity = XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&entity.GetPosition());
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
		if(it->GetId() != entity.GetId() && it->IsAlive())
		{
			if(it->GetCollider()->CheckLineCollision(entity.GetPosition(), lineEndPoint))
			{
				XMVECTOR colObjectToEntity = XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&entity.GetPosition());
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
	
	// This approach is faster than the commented one below
	// The number of grid fields that the entity can maximally see ahead
	unsigned int maxGridDistance = static_cast<unsigned int>(entity.GetMaxSeeAhead() / m_gridSpacing) + 1;

	// Get the current grid position of the entity
	XMFLOAT2 gridPos;
	WorldToGridPosition(entity.GetPosition(), gridPos);

	
				

	unsigned int startX = (gridPos.x > maxGridDistance) ? (static_cast<int>(gridPos.x) - maxGridDistance) : 0;
	unsigned int startY = (gridPos.y > maxGridDistance) ? (static_cast<int>(gridPos.y) - maxGridDistance) : 0;
	unsigned int endX = (gridPos.x + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.x) + maxGridDistance) : (m_numberOfGridPartitions - 1);
	unsigned int endY = (gridPos.y + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.y) + maxGridDistance) : (m_numberOfGridPartitions - 1);

	// Check the grid within that distance for colliding obstacles
	for(unsigned int i = startX; i <= endX; ++i)
	{
		for(unsigned int k = startY; k <= endY; ++k)
		{
			if(!m_pGrid[i][k].m_isEmpty && m_pGrid[i][k].m_type == ObstacleType)
			{
				
				
				if(m_pGrid[i][k].m_pEntity->GetCollider()->CheckLineCollision(entity.GetPosition(), lineEndPoint))
				{
					XMVECTOR colObjectToEntity = XMLoadFloat2(&m_pGrid[i][k].m_pEntity->GetPosition()) - XMLoadFloat2(&entity.GetPosition());
					// Get the square distance between the object and the entity
					float distance; 
					XMStoreFloat(&distance, XMVector2Dot(colObjectToEntity, colObjectToEntity));

					if(distance <= shortestCollisionDistance)
					{
						pCollisionObject = &(*m_pGrid[i][k].m_pEntity);
						shortestCollisionDistance = distance;
					}
				}

				/*
				XMVECTOR colObjectToEntity = XMLoadFloat2(&entity.GetPosition()) - XMLoadFloat2(&m_pGrid[i][k].m_pEntity->GetPosition());

				float a;
				float b;
				float c;

				float radiusSquare = m_pGrid[i][k].m_pEntity->GetRadius() * m_pGrid[i][k].m_pEntity->GetRadius();

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
						pCollisionObject = &(*m_pGrid[i][k].m_pEntity);
						shortestCollisionDistance = distance;
					}
				}
				*/
			}
		}
	}
	

	/*
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
	*/
	return pCollisionObject;
}

//--------------------------------------------------------------------------------------
// Checks fro collisions between an entity and a specified group of other entities.
// Param1: A pointer to the collider that should be checked for collision with other entities.
// Param2: The previous position of the entity (during the last frame).
// Param3: Specifies the group of entities that should be checked for collision with the given entity.
// Param4: Out parameter that will hold a pointer to the colliding entity that is closest to the entity specified in Param1. 
//         Null if there is no collision at all. 
// Returns true if the entity is about to collide with an entity of the specified group, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::CheckCollision(const Collider* pCollider,  const XMFLOAT2& oldPosition, EntityGroup entityGroup, Entity*& outCollisionEntity)
{
	float shortestSquareDistance = std::numeric_limits<float>::max();
	outCollisionEntity = nullptr;

	// Determine the points of the line that the colliders of other entities will be checked against
	
	// Current position of the entity
	XMFLOAT2 start = oldPosition;
	// The position, where the entity will at the end of the frame
	XMFLOAT2 end = pCollider->GetCentre();

	if(entityGroup == GroupTeamA || entityGroup == GroupAllSoldiers || entityGroup == GroupTeamAAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		// Check team A for collision
		for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
		{
			if(it->IsAlive())
			{
				if(it->GetCollider()->CheckLineCollision(start, end))
				{
					float squareDistance = 0.0f;
					XMVECTOR vector = XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&pCollider->GetCentre());
					XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));

					if(squareDistance < shortestSquareDistance)
					{
						shortestSquareDistance = squareDistance;
						outCollisionEntity = &(*it);
					}
				}
			}
		}
	}

	if(entityGroup == GroupTeamB || entityGroup == GroupAllSoldiers || entityGroup == GroupTeamBAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		// Check team B for collision
		for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
		{
			if(it->IsAlive())
			{
				if(it->GetCollider()->CheckLineCollision(start, end))
				{
					float squareDistance = 0.0f;
					XMVECTOR vector = XMLoadFloat2(&it->GetPosition()) - XMLoadFloat2(&pCollider->GetCentre());
					XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));

					if(squareDistance < shortestSquareDistance)
					{
						shortestSquareDistance = squareDistance;
						outCollisionEntity = &(*it);
					}
				}
			}
		}
	}

	if(entityGroup == GroupObstacles || entityGroup == GroupTeamAAndObstacles || entityGroup == GroupTeamBAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		float distance = 0.0f;
		XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&pCollider->GetCentre()) - XMLoadFloat2(&oldPosition)));

		// Only check nearby obstacles for collision
		unsigned int maxGridDistance = static_cast<unsigned int>(distance / m_gridSpacing) + 1;

		// Get the current grid position of the entity
		XMFLOAT2 gridPos;
		WorldToGridPosition(pCollider->GetCentre(), gridPos);

		unsigned int startX = (gridPos.x > maxGridDistance) ? (static_cast<int>(gridPos.x) - maxGridDistance) : 0;
		unsigned int startY = (gridPos.y > maxGridDistance) ? (static_cast<int>(gridPos.y) - maxGridDistance) : 0;
		unsigned int endX = (gridPos.x + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.x) + maxGridDistance) : (m_numberOfGridPartitions - 1);
		unsigned int endY = (gridPos.y + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.y) + maxGridDistance) : (m_numberOfGridPartitions - 1);

		// Check the grid within that distance for colliding obstacles
		for(unsigned int i = startX; i <= endX; ++i)
		{
			for(unsigned int k = startY; k <= endY; ++k)
			{
				if(!m_pGrid[i][k].m_isEmpty > 0 && m_pGrid[i][k].m_type == ObstacleType)
				{
					if(m_pGrid[i][k].m_pEntity->GetCollider()->CheckLineCollision(start, end))
					{
						float squareDistance = 0.0f;
						XMVECTOR vector = XMLoadFloat2(&m_pGrid[i][k].m_pEntity->GetPosition()) - XMLoadFloat2(&pCollider->GetCentre());
						XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));

						if(squareDistance < shortestSquareDistance)
						{
							shortestSquareDistance = squareDistance;
							outCollisionEntity = m_pGrid[i][k].m_pEntity;
						}
					}
				}
			}
		}

		/*
		// Check cover positions (obstacles) for collision
		for(std::list<CoverPosition>::iterator it = m_coverSpots.begin(); it != m_coverSpots.end(); ++it)
		{
			if(it->GetCollider()->CheckLineCollision(start, end))
			{
				return true;
			}
		}
		*/
	}

	return (outCollisionEntity != nullptr);
}

//--------------------------------------------------------------------------------------
// Accumulates a list of entities hostile to the passed in entity.
// Param1: The entity, for which enemies should be found.
// Param2: The list that will hold pointers to the found enemies (out parameter).
//--------------------------------------------------------------------------------------
void TestEnvironment::GetEnemies(const FightingEntity* pEntity, std::list<FightingEntity*>& enemies)
{
	if(pEntity == nullptr)
	{
		return;
	}

	if(pEntity->GetType() == RedSoldierType)
	{
		for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
		{
			enemies.push_back(&(*it));
		}
	}else
	{
		for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
		{
			enemies.push_back(&(*it));
		}
	}
}

//--------------------------------------------------------------------------------------
// Determines whether there is a direct line of sight between two fields on the grid.
// Note: This only checks between the centre of the two given grid fields. Uses 
//       Bresenham's line algorithm.
// Param1: The x-coordinate of the start grid field in grid units.
// Param2: The y-coordinate of the start grid field in grid units.
// Param3: The x-coordinate of the end grid field in grid units.
// Param4: The y-coordinate of the end grid field in grid units.
// Returns true if a direct line of sight exists, false if an obstacle obstructs the view.
//--------------------------------------------------------------------------------------
bool TestEnvironment::CheckLineOfSight(int startGridX, int startGridY, int endGridX, int endGridY)
{
	// Prepare the coordinates for the calculation according to the properties of the line 
	// connecting drawn between them.
    bool steep = std::abs(endGridY - startGridY) > std::abs(endGridX - startGridX);
    if(steep) 
	{
		std::swap(startGridX, startGridY);
		std::swap(endGridX, endGridY);
    }
    if(startGridX > endGridX) 
	{
		std::swap(startGridX, endGridX);
		std::swap(startGridY, endGridY);
    }

    int deltaX = endGridX - startGridX;
    int deltaY = std::abs(endGridY - startGridY);
    int error = 0;
    int yStep;
    int y = startGridY;

    if (startGridY < endGridY)
	{
		yStep = 1;
	}else 
	{
		yStep = -1;
	}

	for(int x = startGridX; x <= endGridX; x++) 
	{
		// Check if there is an obstacle blocking the line of sight
		if((!m_pGrid[y][x].m_isEmpty > 0 && steep && (m_pGrid[y][x].m_type == ObstacleType)) || (!m_pGrid[x][y].m_isEmpty > 0 && !steep && (m_pGrid[x][y].m_type == ObstacleType)))
		{
			return false;
		}

        error += deltaY;
        if(2 * error >= deltaX) 
		{
            y += yStep;
            error -= deltaX;
        }
    }

	return true;
}

//--------------------------------------------------------------------------------------
// Starts the simulation, switches from edit to simulation mode.
// Returns true if the simulation could be started successfully.
//--------------------------------------------------------------------------------------
bool TestEnvironment::StartSimulation(void)
{
	/*
	for(std::list<Soldier>::iterator it = m_teamA.begin(); it != m_teamA.end(); ++it)
	{
		it->Activate();
	}

	for(std::list<Soldier>::iterator it = m_teamB.begin(); it != m_teamB.end(); ++it)
	{
		it->Activate();
	}
	*/

	for(unsigned int i = 0; i < NumberOfTeams; ++i)
	{
		if(!m_flagSet[i] || m_soldierCount[i] < g_kSoldiersPerTeam)
		{
			return false;
		}
	}

	PrepareSimulation();

	for(unsigned int i = 0; i < g_kSoldiersPerTeam * NumberOfTeams; ++i)
	{
		m_soldiers[i].Activate();
	}

	m_isInEditMode = false;
	m_isPaused = false;

	return true;
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
				if(m_pGrid[i][k].m_type == RedSoldierType || m_pGrid[i][k].m_type == BlueSoldierType)
				{
					XMFLOAT2 position;
					GridToWorldPosition(XMFLOAT2(static_cast<float>(i), static_cast<float>(k)), position);
					m_pGrid[i][k].m_pEntity->SetPosition(position);
					m_pGrid[i][k].m_pEntity->SetRotation(m_pGrid[i][k].m_rotation);
					m_pGrid[i][k].m_pEntity->Reset();
				}
			}
		}
	}

	// Delete all projectiles
	m_projectiles.clear();

	m_isInEditMode = true;
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

	unsigned int gridX = static_cast<unsigned int>(coverNode.GetGridPosition().x);
	unsigned int gridY = static_cast<unsigned int>(coverNode.GetGridPosition().y);

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