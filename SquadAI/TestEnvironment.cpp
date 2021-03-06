/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironment.cpp
*  Encapsulates a test setup and contains all entities
*  that are part of it.
*/

#include "TestEnvironment.h"

//#define DEBUG

TestEnvironment::TestEnvironment(void) : m_id(0),
										 m_isPaused(true),
										 m_isInEditMode(true),
										 m_gridSize(0.0f),
										 m_numberOfGridPartitions(0),
										 m_gridSpacing(0.0f),
										 m_pNodes(nullptr)
{
	for(unsigned int i = 0; i < NumberOfObjectTypes; ++i)
	{
		m_objectScaleFactors[i] = 1.0f;
	}	
	
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_soldierCount[i]		  = 0;
		m_flagSet[i]			  = false;
		m_spawnPointCount[i]	  = 0;
		m_pTeamAI[i]			  = nullptr;
		m_attackPositionsCount[i] = 0;
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
	m_objectScaleFactors[RedSoldierType]				= 1.0f; 
	m_objectScaleFactors[BlueSoldierType]				= 1.0f; 
	m_objectScaleFactors[ObstacleType]					= 1.0f; 
	m_objectScaleFactors[RedFlagType]					= 0.5f; 
	m_objectScaleFactors[BlueFlagType]					= 0.5f;
	m_objectScaleFactors[RedBaseAreaType]				= 1.0f;
	m_objectScaleFactors[BlueBaseAreaType]				= 1.0f; 
	m_objectScaleFactors[RedSpawnPointType]				= 0.75f;
	m_objectScaleFactors[BlueSpawnPointType]			= 0.75f;
	m_objectScaleFactors[RedAttackPositionType]			= 0.5f;
	m_objectScaleFactors[BlueAttackPositionType]		= 0.5f;
	m_objectScaleFactors[DeadRedSoldierType]			= 1.0f; 
	m_objectScaleFactors[DeadBlueSoldierType]			= 1.0f; 
	m_objectScaleFactors[ProjectileType]				= 0.2f;

	m_isPaused = true;
	m_isInEditMode = true;

	m_projectiles.clear();
	m_obstacles.clear();
	m_staticObjects.clear();

	m_pGameContext = new MultiflagCTFGameContext(g_kGameRoundTimeLimit, g_kNotifyTimeInterval, g_kWinScore, g_kFlagResetTimer);
	if(!m_pGameContext)
	{
		return false;
	}

	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_soldierCount[i]		  = 0;
		m_flagSet[i]			  = false;
		m_spawnPointCount[i]	  = 0;
		m_attackPositionsCount[i] = 0;
		m_pTeamAI[i]			  = new MultiflagCTFTeamAI();

		if(!m_pTeamAI[i])
		{
			return false;
		}

		if(i == TeamRed)
		{
			if(!m_pTeamAI[i]->Initialise(EntityTeam(i), this, CharAggressive))
			{
				return false;
			}
		}else
		{
			if(!m_pTeamAI[i]->Initialise(EntityTeam(i), this, CharDefensive))
			{
				return false;
			}
		}

		m_pGameContext->RegisterTeamAI(m_pTeamAI[i]);
	}

	// Initialise the random number generator.
	srand(static_cast<unsigned int>(time(NULL)));

	return InitialiseGrid() && m_pathfinder.Initialise(this);
}

//--------------------------------------------------------------------------------------
// Updates the test environment.
// Param1: The render context that is used to keep track of entities within the environment to be drawn.
// Param2: The time in seconds passed since the last frame.
//--------------------------------------------------------------------------------------
void TestEnvironment::Update(RenderContext& pRenderContext, float deltaTime)
{
	if(m_isInEditMode)
	{
		for(std::vector<EditModeObject>::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); ++it)
		{
			XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - it->GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetUniformScale(), it->GetUniformScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);

			pRenderContext.AddInstance(it->GetType(), transform);
		}
	}else
	{
		if(!m_isPaused && !m_pGameContext->IsTerminated())
		{
			UpdateRespawns(deltaTime);
			SortOutProcessedMessages();

			// Update the team AIs
			for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
			{
				if(m_pTeamAI[i])
				{
					m_pTeamAI[i]->Update(deltaTime);
				}
			}
		}

		// Update soldiers

		for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
		{
			
			XMFLOAT2 oldPos = m_soldiers[i].GetPosition();

			if(!m_pGameContext->IsTerminated() && !m_isPaused && m_soldiers[i].IsAlive())
			{
				m_soldiers[i].Update(deltaTime);
			}

			XMMATRIX translationMatrix = XMMatrixTranslation(m_soldiers[i].GetPosition().x, m_soldiers[i].GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - m_soldiers[i].GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(m_soldiers[i].GetUniformScale(), m_soldiers[i].GetUniformScale(), 1.0f);

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

			// The objective possibly reached by the soldier
			CollidableObject* pHitEntity = nullptr;

			if(!m_pGameContext->IsTerminated() && !m_isPaused && m_soldiers[i].IsAlive())
			{
				// Check for collisions of the soldiers with any objectives
				if(CheckCollision(&m_soldiers[i], oldPos, GroupObjectivesTeamRed, pHitEntity))
				{
					EntityReachedObjectiveEventData data(&m_soldiers[i], reinterpret_cast<Objective*>(pHitEntity));
					SendEvent(m_pGameContext, EntityReachedObjectiveEventType, &data);
				}
				if(CheckCollision(&m_soldiers[i], oldPos, GroupObjectivesTeamBlue, pHitEntity))
				{
					EntityReachedObjectiveEventData data(&m_soldiers[i], reinterpret_cast<Objective*>(pHitEntity));
					SendEvent(m_pGameContext, EntityReachedObjectiveEventType, &data);
				}
			}
		}

		// Update flags
		for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
		{
			XMMATRIX translationMatrix = XMMatrixTranslation(m_objectives[i].GetPosition().x, m_objectives[i].GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - m_objectives[i].GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(m_objectives[i].GetUniformScale(), m_objectives[i].GetUniformScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);

			switch(EntityTeam(i))
			{
			case TeamRed:
				pRenderContext.AddInstance(RedFlagType, transform);
			case TeamBlue:
				pRenderContext.AddInstance(BlueFlagType, transform);
			}
		}

		// Update obstacles
		for(std::list<Obstacle>::iterator it = m_obstacles.begin(); it != m_obstacles.end(); ++it)
		{
			XMMATRIX translationMatrix = XMMatrixTranslation(it->GetPosition().x, it->GetPosition().y, 0.0f);
			XMMATRIX rotationMatrix    = XMMatrixRotationZ(XMConvertToRadians(360.0f - it->GetRotation()));
			XMMATRIX scalingMatrix     = XMMatrixScaling(it->GetUniformScale(), it->GetUniformScale(), 1.0f);

			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, scalingMatrix * rotationMatrix * translationMatrix);
			pRenderContext.AddInstance(ObstacleType, transform);
		}

		// Update projectiles
		std::list<Projectile>::iterator it = m_projectiles.begin();
		while(it != m_projectiles.end())
		{
			XMFLOAT2 oldPos = it->GetPosition();

			if(!m_pGameContext->IsTerminated() && !m_isPaused)
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
			CollidableObject* pHitEntity = nullptr;

			if(((*it).GetFriendlyTeam() == TeamRed && CheckCollision(&(*it), oldPos, GroupTeamBlueAndObstacles, pHitEntity)) ||
			   ((*it).GetFriendlyTeam() == TeamBlue && CheckCollision(&(*it), oldPos, GroupTeamRedAndObstacles, pHitEntity)))
			{
				if(pHitEntity->GetCategory() == CategoryEntity)
				{
					// Find the soldier that shot the projectile to see if he's still alive
					Soldier* pDeadSoldier = std::find_if(std::begin(m_soldiers), std::end(m_soldiers), Soldier::FindSoldierById((*it).GetShooterId()));
						
					EntityHitEventData data(g_kProjectileDamage, it->GetShooterId(), pDeadSoldier->IsAlive(), it->GetOrigin());
					SendEvent(reinterpret_cast<Entity*>(pHitEntity), EntityHitEventType, &data);
				}

				it = m_projectiles.erase(it);
			}
			else
			{
				++it;
			}
		}

		if(!m_isPaused && !m_pGameContext->IsTerminated())
		{
			m_pGameContext->Update(deltaTime);
		}

	}
}

//--------------------------------------------------------------------------------------
// Releases resources used by the test environment.
//--------------------------------------------------------------------------------------
void TestEnvironment::Cleanup()
{
	CleanupGrid();
	if(m_pGameContext)
	{
		delete m_pGameContext;
		m_pGameContext = nullptr;
	}

	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		if(m_pTeamAI[i])
		{
			delete m_pTeamAI[i];
			m_pTeamAI[i] = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------------
// Processes a given message.
// Param1: A pointer to the message to process.
//--------------------------------------------------------------------------------------
void TestEnvironment::ProcessMessage(Message* pMessage)
{
	// Not expecting any messages, call the default implementation that simply marks all
	// messages as processed.
	Communicator::ProcessMessage(pMessage);
}

//--------------------------------------------------------------------------------------
// Process a given event. Default implementation.
// Param1: The type of event.
// Param2: A pointer to the event data.
//--------------------------------------------------------------------------------------
void TestEnvironment::ProcessEvent(EventType type, void* pEventData)
{
	switch(type)
	{
	case ProjectileFiredEventType:
		{
		ProjectileFiredEventData* pProjectileFiredData = reinterpret_cast<ProjectileFiredEventData*>(pEventData);
		m_pGameContext->AddShotFired(pProjectileFiredData->m_shooterTeam, 1);
		AddProjectile(pProjectileFiredData->m_shooterId, pProjectileFiredData->m_shooterTeam, pProjectileFiredData->m_origin, pProjectileFiredData->m_target);
		break;
		}
	case EntityDiedEventType:
		{
		EntityDiedEventData* pEntityDiedData = reinterpret_cast<EntityDiedEventData*>(pEventData);
		
		if(pEntityDiedData->m_team == TeamRed)
		{
			m_pGameContext->AddKill(TeamBlue, pEntityDiedData->m_team, pEntityDiedData->m_id, pEntityDiedData->m_shooterId);
		}else
		{
			m_pGameContext->AddKill(TeamRed, pEntityDiedData->m_team, pEntityDiedData->m_id, pEntityDiedData->m_shooterId);
		}

		AddDeadEntity(pEntityDiedData->m_id);

		// Broadcast a message to all other entities.
		for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
		{
			if(m_soldiers[i].GetId() != pEntityDiedData->m_id)
			{
				// Forward the message.
				EntityKilledMessageData data(pEntityDiedData->m_team, pEntityDiedData->m_id, pEntityDiedData->m_shooterId);
				SendMessage(&m_soldiers[i], EntityKilledMessageType, &data);
			}
		}
		
		break;
		}
	default:
		Communicator::ProcessEvent(type, pEventData);
	}
}

//--------------------------------------------------------------------------------------
// Prepares the test environment for simulation by creating dynamic objects, colliders
// and a node graph based on the objects placed by the user in edit mode. 
// Returns true if the simulation was prepared successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::PrepareSimulation(void)
{
	ResetNodeGraph();

	// Initialise simulation mode objects

	int soldierIndex = 0;

	for(std::vector<EditModeObject>::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); ++it)
	{
		switch(it->GetType())
		{
		case RedSoldierType:
			{

			SoldierProperties properties;
			properties.m_maxSpeed					= g_kSoldierMaxSpeed;
			properties.m_maxTotalForce				= g_kSoldierMaxForce;
			properties.m_maxCollisionSeeAhead		= g_kSoldierMaxSeeAhead;
			properties.m_maxCollisionAvoidanceForce = g_kSoldierMaxCollisionAvoidanceForce;
			properties.m_maxAvoidWallsForce			= g_kSoldierMaxAvoidWallsForce;
			properties.m_maxSeparationForce			= g_kSoldierMaxSeparationForce;
			properties.m_targetReachedRadius		= g_kSoldierTargetReachedRadius;
			properties.m_speedHandicap				= g_kSoliderSpeedHandicap;
			properties.m_avoidWallsRadius			= m_gridSpacing; 
			properties.m_separationRadius			= m_gridSpacing;
			properties.m_fieldOfView				= g_kSoldierFieldOfView;
			properties.m_viewingDistance			= g_kSoldierViewingDistance;
			properties.m_fireWeaponInterval         = g_kFireWeaponInterval;
			properties.m_maxHealth					= g_kSoldierMaxHealth;
			properties.m_lookAroundInterval         = g_kSoldierLookAroundInterval;
			properties.m_reportInterval             = g_kReportInterval;
			
			CircleColliderData colliderData(it->GetPosition(), m_gridSpacing * m_objectScaleFactors[RedSoldierType] * 0.5f);

			if(!m_soldiers[soldierIndex].Initialise(++m_id, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), CategoryEntity, CircleColliderType, 
												      &colliderData, this, TeamRed, properties))
			{
				return false;
			}

			// Set the team AI for the soldier
			m_soldiers[soldierIndex].SetTeamAI(m_pTeamAI[TeamRed]);

			m_pTeamAI[TeamRed]->AddTeamMember(&m_soldiers[soldierIndex]);

			++soldierIndex;
			break;
			}
		case BlueSoldierType:
			{
			SoldierProperties properties;
			properties.m_maxSpeed					= g_kSoldierMaxSpeed;
			properties.m_maxTotalForce				= g_kSoldierMaxForce;
			properties.m_maxCollisionSeeAhead		= g_kSoldierMaxSeeAhead;
			properties.m_maxCollisionAvoidanceForce = g_kSoldierMaxCollisionAvoidanceForce;
			properties.m_maxAvoidWallsForce			= g_kSoldierMaxAvoidWallsForce;
			properties.m_maxSeparationForce			= g_kSoldierMaxSeparationForce;
			properties.m_targetReachedRadius		= g_kSoldierTargetReachedRadius;
			properties.m_speedHandicap				= g_kSoliderSpeedHandicap;
			properties.m_avoidWallsRadius			= m_gridSpacing; 
			properties.m_separationRadius			= m_gridSpacing;
			properties.m_fieldOfView				= g_kSoldierFieldOfView;
			properties.m_viewingDistance			= g_kSoldierViewingDistance;
			properties.m_fireWeaponInterval         = g_kFireWeaponInterval;
			properties.m_maxHealth					= g_kSoldierMaxHealth;
			properties.m_lookAroundInterval         = g_kSoldierLookAroundInterval;
			properties.m_reportInterval             = g_kReportInterval;

			CircleColliderData colliderData(it->GetPosition(), m_gridSpacing * m_objectScaleFactors[BlueSoldierType] * 0.5f);

			if(!m_soldiers[soldierIndex].Initialise(++m_id, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), CategoryEntity, CircleColliderType, 
												      &colliderData, this, TeamBlue, properties))
			{
				return false;
			}

			// Set the team AI for the soldier
			m_soldiers[soldierIndex].SetTeamAI(m_pTeamAI[TeamBlue]);

			m_pTeamAI[TeamBlue]->AddTeamMember(&m_soldiers[soldierIndex]);

			++soldierIndex;
			break;
			}
		case ObstacleType:
			{
				AxisAlignedRectangleColliderData colliderData(it->GetPosition(), m_gridSpacing * m_objectScaleFactors[ObstacleType], m_gridSpacing * m_objectScaleFactors[ObstacleType], GetGridSize());
			
			m_obstacles.push_back(Obstacle());
			if(!m_obstacles.back().Initialise(++m_id, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), CategoryObstacle, AxisAlignedRectangleColliderType, &colliderData))
			{
				m_obstacles.pop_back();
				return false;
			}
			break;
			}
		case RedFlagType:
			{
			CircleColliderData colliderData(it->GetPosition(), m_gridSpacing * g_kPickupFlagRadiusRelative);
			if(!m_objectives[TeamRed].Initialise(++m_id, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), CategoryObjective, CircleColliderType, &colliderData, TeamRed))
			{
				return false;
			}

			ObjectiveAddedEventData data(&m_objectives[TeamRed]);
			SendEvent(m_pGameContext, ObjectiveAddedEventType, &data);

			break;
			}
		case BlueFlagType:
			{
			CircleColliderData colliderData(it->GetPosition(), m_gridSpacing * g_kPickupFlagRadiusRelative);
			if(!m_objectives[TeamBlue].Initialise(++m_id, it->GetPosition(), it->GetRotation(), it->GetUniformScale(), CategoryObjective, CircleColliderType, &colliderData, TeamBlue))
			{
				return false;
			}

			ObjectiveAddedEventData data(&m_objectives[TeamBlue]);
			SendEvent(m_pGameContext, ObjectiveAddedEventType, &data);

			break;
			}
		case RedBaseAreaType:
			m_baseFieldPositions[TeamRed].push_back(it->GetPosition());
			break;
		case BlueBaseAreaType:
			m_baseFieldPositions[TeamBlue].push_back(it->GetPosition());
			break;
		case RedSpawnPointType:
			m_spawnPoints[TeamRed].push_back(it->GetPosition());
			break;
		case BlueSpawnPointType:
			m_spawnPoints[TeamBlue].push_back(it->GetPosition());
			break;
		case RedAttackPositionType:
			{
			std::pair<std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator, bool> result =  m_attackPositions[TeamRed].insert(std::pair<Direction, std::vector<XMFLOAT2>>(GetAttackDirectionFromRotation(it->GetRotation()), std::vector<XMFLOAT2>()));
			result.first->second.push_back(it->GetPosition());

			break;
			}
		case BlueAttackPositionType:
			std::pair<std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator, bool> result =  m_attackPositions[TeamBlue].insert(std::pair<Direction, std::vector<XMFLOAT2>>(GetAttackDirectionFromRotation(it->GetRotation()), std::vector<XMFLOAT2>()));
			result.first->second.push_back(it->GetPosition());
			
			break;
		}
	}

	UpdateNodeGraph();

	// Prepare the team AIs for simulation.
	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		for(unsigned int k = 0; k < NumberOfTeams-1; ++k)
		{
			m_pTeamAI[i]->RegisterObjective(&m_objectives[k]);
		}

		m_pTeamAI[i]->PrepareForSimulation();
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

	if((type == RedSoldierType && m_soldierCount[TeamRed] >= g_kSoldiersPerTeam) || 
	   (type == BlueSoldierType && m_soldierCount[TeamBlue] >= g_kSoldiersPerTeam) ||
	   (type == RedFlagType && m_flagSet[TeamRed]) ||
	   (type == BlueFlagType && m_flagSet[TeamBlue]))
	{
		// Maximal number of flags/soldiers was already reached for a certain team.
		return false;
	}

	GridToWorldPosition(gridPosition, updatedPosition);

	// Get the ID for the grid field, that the entity was placed on
	unsigned long gridId = static_cast<int>(gridPosition.x) * m_numberOfGridPartitions + static_cast<int>(gridPosition.y);

	// Check if there are already static objects placed on this field
	std::vector<EditModeObject*> foundObjects;

	// Find all objects with the given grid id
	std::vector<EditModeObject>::iterator foundIt = std::find_if(m_staticObjects.begin(), m_staticObjects.end(), EditModeObject::FindEditModeObjectByGridId(gridId));
	while (foundIt != m_staticObjects.end()) {
		foundObjects.push_back(&(*foundIt));
		// Continue search from the position the last object was found.
		foundIt = std::find_if(++foundIt, m_staticObjects.end(), EditModeObject::FindEditModeObjectByGridId(gridId));
	}

	bool doAddObject = true;

	std::vector<EditModeObject*>::iterator it = foundObjects.begin();
	while(doAddObject && (it != foundObjects.end()))
	{
		// Make sure objects are only placed on top of each other if allowed
		doAddObject = !((type == (*it)->GetType()) ||
						((type != RedSoldierType || ((*it)->GetType() != RedBaseAreaType)) && 
						 (type != RedBaseAreaType || ((*it)->GetType() != RedSoldierType)) &&
						 (type != RedFlagType || ((*it)->GetType() != RedBaseAreaType)) &&
					     (type != RedBaseAreaType || ((*it)->GetType() != RedFlagType)) &&
						 (type != BlueSoldierType || ((*it)->GetType() != BlueBaseAreaType)) &&
					     (type != BlueBaseAreaType || ((*it)->GetType() != BlueSoldierType)) &&
					     (type != BlueFlagType || ((*it)->GetType() != BlueBaseAreaType)) &&
					     (type != BlueBaseAreaType || ((*it)->GetType() != BlueFlagType)) &&
						 (type != RedSpawnPointType || ((*it)->GetType() != RedBaseAreaType)) &&
					     (type != RedBaseAreaType || ((*it)->GetType() != RedSpawnPointType)) &&
						 (type != RedSpawnPointType || ((*it)->GetType() != RedSoldierType)) &&
					     (type != RedSoldierType || ((*it)->GetType() != RedSpawnPointType)) &&
						 (type != BlueSpawnPointType || ((*it)->GetType() != BlueBaseAreaType)) &&
					     (type != BlueBaseAreaType || ((*it)->GetType() != BlueSpawnPointType)) &&
						 (type != BlueSpawnPointType || ((*it)->GetType() != BlueSoldierType)) &&
					     (type != BlueSoldierType || ((*it)->GetType() != BlueSpawnPointType)) &&
						 (type != RedAttackPositionType || ((*it)->GetType() != RedSoldierType)) &&
						 (type != RedSoldierType || ((*it)->GetType() != RedAttackPositionType)) &&
						 (type != RedAttackPositionType || ((*it)->GetType() != BlueSoldierType)) &&
						 (type != BlueSoldierType || ((*it)->GetType() != RedAttackPositionType)) &&
						 (type != RedAttackPositionType || ((*it)->GetType() != RedBaseAreaType)) &&
						 (type != RedBaseAreaType || ((*it)->GetType() != RedAttackPositionType)) &&
						 (type != BlueAttackPositionType || ((*it)->GetType() != RedSoldierType)) &&
						 (type != RedSoldierType || ((*it)->GetType() != BlueAttackPositionType)) &&
						 (type != BlueAttackPositionType || ((*it)->GetType() != BlueSoldierType)) &&
						 (type != BlueSoldierType || ((*it)->GetType() != BlueAttackPositionType)) &&
						 (type != BlueAttackPositionType || ((*it)->GetType() != BlueBaseAreaType)) &&
						 (type != BlueBaseAreaType || ((*it)->GetType() != BlueAttackPositionType))
						));

		++it;
	}

	if(doAddObject)
	{
		// Add the new object
		m_staticObjects.push_back(EditModeObject());

		if(!m_staticObjects.back().Initialise(++m_id, updatedPosition, rotation, m_objectScaleFactors[type] * m_gridSpacing, gridId, type))
		{
			m_staticObjects.pop_back();
			return false;
		}

		switch(type)
		{
		case RedSoldierType:
			++m_soldierCount[TeamRed];
			break;
		case BlueSoldierType:
			++m_soldierCount[TeamBlue];
			break;
		case RedFlagType:
			m_flagSet[TeamRed] = true;
			break;
		case BlueFlagType:
			m_flagSet[TeamBlue] = true;
			break;
		case RedSpawnPointType:
			++m_spawnPointCount[TeamRed];
			break;
		case BlueSpawnPointType:
			++m_spawnPointCount[TeamBlue];
			break;
		case RedAttackPositionType:
			++m_attackPositionsCount[TeamRed];
			break;
		case BlueAttackPositionType:
			++m_attackPositionsCount[TeamBlue];
			break;
		}
	}

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

	std::vector<EditModeObject*> foundObjects;
	std::vector<EditModeObject>::iterator foundIt = std::find_if(m_staticObjects.begin(), m_staticObjects.end(), EditModeObject::FindEditModeObjectByGridId(gridId));
	while (foundIt != m_staticObjects.end()) {
		foundObjects.push_back(&(*foundIt));
		// Continue search from the position the last object was found.
		foundIt = std::find_if(++foundIt, m_staticObjects.end(), EditModeObject::FindEditModeObjectByGridId(gridId));
	}

	if(foundObjects.empty())
	{
		return false;
	}

	for(std::vector<EditModeObject*>::iterator it = foundObjects.begin(); it != foundObjects.end(); ++it)
	{
		// Update counts
		switch((*it)->GetType())
		{
		case RedSoldierType:
			--m_soldierCount[TeamRed];
			break;
		case BlueSoldierType:
			--m_soldierCount[TeamBlue];
			break;
		case RedFlagType:
			m_flagSet[TeamRed] = false;
			break;
		case BlueFlagType:
			m_flagSet[TeamBlue] = false;
			break;
		case RedSpawnPointType:
			--m_spawnPointCount[TeamRed];
			break;
		case BlueSpawnPointType:
			--m_spawnPointCount[TeamBlue];
			break;
		case RedAttackPositionType:
			--m_attackPositionsCount[TeamRed];
			break;
		case BlueAttackPositionType:
			--m_attackPositionsCount[TeamBlue];
			break;
		}

	}

	// Actual deletion of the objects
	std::vector<EditModeObject>::iterator removeIt = std::remove_if(m_staticObjects.begin(), m_staticObjects.end(), EditModeObject::FindEditModeObjectByGridId(gridId));
	m_staticObjects.erase(removeIt, m_staticObjects.end());
	return true;
}

//--------------------------------------------------------------------------------------
// Adds a new projectile entity to the test environment.
// Param1: The id of the entity that shot the projectile.
// Param2: The team that fired the projectile.
// Param3: The position from which the projectile is being fired.
// Param4: The target position towards which the projectile is being fired.
// Returns true if the projectile was successfully added and initialised, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::AddProjectile(unsigned long shooterId, EntityTeam friendlyTeam, const XMFLOAT2& origin, const XMFLOAT2& target)
{
	if(friendlyTeam == TeamBlue)
	{
		int s = 4;
	}

	if((origin.x == target.x) && (origin.y == target.y))
	{
		return false;
	}

	// Note: Instead of using a vector and dynamic allocation, it might make sense to use a pool of pre-allocated
	//       projectiles.

	m_projectiles.push_back(Projectile());

	XMFLOAT2 direction(0.0f, 0.0f);
	XMStoreFloat2(&direction, XMLoadFloat2(&target) - XMLoadFloat2(&origin));

	CircleColliderData colliderData(origin, m_gridSpacing * m_objectScaleFactors[ProjectileType] * 0.5f);
	if(!m_projectiles.back().Initialise(++m_id, origin, 0.0f, m_objectScaleFactors[ProjectileType] * m_gridSpacing, CategoryProjectile, CircleColliderType, &colliderData, origin, direction, g_kProjectileSpeed, shooterId, friendlyTeam))
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

		for(std::vector<EditModeObject>::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); ++it)
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

		m_staticObjects.clear();

		for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
		{
			m_flagSet[i] = false;
			m_soldierCount[i] = 0;
			m_spawnPointCount[i] = 0;
			m_attackPositionsCount[i] = 0;
		}

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
// area around a given position. This checks against the centres of entities. Does not include
// dead entities.
// Param1: A pointer to the entity, for which the nearby entities should be found.
// Param2: The radius of the circle area.
// Param3: Specifies which set of entities will be checked and returned.
// Param4: A multimap that will hold the entities positioned within the radius, 
//         sorted by the square distance to the entity as key elements.
//--------------------------------------------------------------------------------------
void TestEnvironment::GetNearbyObjects(const XMFLOAT2& position, float radius, EntityGroup entityGroup, std::multimap<float, CollidableObject*>& collisionObjects)
{
	float squareRadius = radius * radius;
	float squareDistance = 0.0f;
 
	if(entityGroup != GroupObstacles) 
	{
		if(entityGroup == GroupTeamRed || entityGroup == GroupTeamRedAndObstacles)
		{
			// Check all soldiers
			for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
			{
				if(m_soldiers[i].GetTeam() == TeamRed && m_soldiers[i].IsAlive())
				{
					XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&m_soldiers[i].GetPosition()) - XMLoadFloat2(&position)));
					if(squareDistance <= squareRadius)
					{
						collisionObjects.insert(std::pair<float, CollidableObject*>(squareDistance,(&(m_soldiers[i]))));
					}
				}
			}
		}else if(entityGroup == GroupTeamBlue || entityGroup == GroupTeamBlueAndObstacles)
		{
			// Check all soldiers
			for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
			{
				if(m_soldiers[i].GetTeam() == TeamBlue && m_soldiers[i].IsAlive())
				{
					XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&m_soldiers[i].GetPosition()) - XMLoadFloat2(&position)));
					if(squareDistance <= squareRadius)
					{
						collisionObjects.insert(std::pair<float, CollidableObject*>(squareDistance,(&(m_soldiers[i]))));
					}
				}
			}
		}else
		{
			// Check all soldiers
			for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
			{
				if(m_soldiers[i].IsAlive())
				{
					XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&m_soldiers[i].GetPosition()) - XMLoadFloat2(&position)));
					if(squareDistance <= squareRadius)
					{
						collisionObjects.insert(std::pair<float, CollidableObject*>(squareDistance,(&(m_soldiers[i]))));
					}
				}
			}
		}
	}

	if(entityGroup == GroupObstacles || entityGroup == GroupTeamRedAndObstacles || entityGroup == GroupTeamBlueAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		// Only check nearby obstacles for collision
		unsigned int maxGridDistance = static_cast<unsigned int>(radius / m_gridSpacing) + 1;

		// Get the current grid position of the entity
		XMFLOAT2 gridPos;
		WorldToGridPosition(position, gridPos);

		unsigned int startX = (gridPos.x > maxGridDistance) ? (static_cast<int>(gridPos.x) - maxGridDistance) : 0;
		unsigned int startY = (gridPos.y > maxGridDistance) ? (static_cast<int>(gridPos.y) - maxGridDistance) : 0;
		unsigned int endX = (gridPos.x + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.x) + maxGridDistance) : (m_numberOfGridPartitions - 1);
		unsigned int endY = (gridPos.y + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.y) + maxGridDistance) : (m_numberOfGridPartitions - 1);

		// Check the grid within that distance for colliding obstacles
		for(unsigned int i = startX; i <= endX; ++i)
		{
			for(unsigned int k = startY; k <= endY; ++k)
			{
				if(m_pNodes[i][k].IsObstacle())
				{
					int a = i;
					int b = k;

					XMStoreFloat(&squareDistance, XMVector2LengthSq(XMLoadFloat2(&m_pNodes[i][k].GetObstacle()->GetPosition()) - XMLoadFloat2(&position)));
					if(squareDistance <= squareRadius)
					{
						collisionObjects.insert(std::pair<float, CollidableObject*>(squareDistance, m_pNodes[i][k].GetObstacle()));
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// Randomly determines a position within the test environment that is not occupied by
// an obstacle.
// Param1: Out paramter that will hold the randomly determined target position.
// Returns true if a position was found false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::GetRandomUnblockedTarget(XMFLOAT2& outPosition) const
{
	if(m_obstacles.size() >= m_numberOfGridPartitions * m_numberOfGridPartitions)
	{
		return false;
	}

	unsigned int gridX = 0;
	unsigned int gridY = 0;

	do
	{
		gridX = rand() % m_numberOfGridPartitions;
		gridY = rand() % m_numberOfGridPartitions;
	}while(m_pNodes[gridX][gridY].IsObstacle());

	outPosition = m_pNodes[gridX][gridY].GetWorldPosition();

	return true;
}

//--------------------------------------------------------------------------------------
// Randomly determines a position within a certain area of the test environment that is 
// not occupied by an obstacle. 
// Param1: The centre of the area, in which the random position should lie.
// Param2: Radius around the centre of the area, in which the result position will lie.
// Param3: Out paramter that will hold the randomly determined target position.
// Returns true if a position was found false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::GetRandomUnblockedTargetInArea(const XMFLOAT2& centre, float radius, XMFLOAT2& outPosition) const
{
	XMFLOAT2 centreGridPos(0.0f, 0.0f);
	WorldToGridPosition(centre, centreGridPos);

	int gridRadius = static_cast<int>(radius / GetGridSpacing());

	int startX = (static_cast<int>(centreGridPos.x) - gridRadius >= 0) ? (static_cast<int>(centreGridPos.x) - gridRadius) : 0;
	int endX   = (static_cast<unsigned int>(centreGridPos.x) + gridRadius < m_numberOfGridPartitions) ? (static_cast<int>(centreGridPos.x) + gridRadius) : m_numberOfGridPartitions - 1;
	int startY = (static_cast<int>(centreGridPos.y) - gridRadius >= 0) ? (static_cast<int>(centreGridPos.y) - gridRadius) : 0;
	int endY   = (static_cast<unsigned int>(centreGridPos.y) + gridRadius < m_numberOfGridPartitions) ? (static_cast<int>(centreGridPos.y) + gridRadius) : m_numberOfGridPartitions - 1;

	// Note: Not sure if this is a good way of doing it, but some way is required from running into an infinite loop
	//       below in case the whole area is blocked.
	unsigned int maxTries = (endX - startX) * (endY - startY);
	unsigned int tryCount = 0;

	unsigned int x = 0;
	unsigned int y = 0;

	do
	{
		x = startX + rand() % (endX - startX);
		y = startY + rand() % (endY - startY);
		++tryCount;
	}while(m_pNodes[x][y].IsObstacle() && tryCount < maxTries);
	
	GridToWorldPosition(XMFLOAT2(static_cast<float>(x), static_cast<float>(y)), outPosition);
	
	return (tryCount < maxTries);
}

//--------------------------------------------------------------------------------------
// Checks for collisions between a moving entity and a specified group of other entities. Collision
// of an object with itself is excluded, as are collisions with dead entities.
// Param1: A pointer to the collider that should be checked for collision with other entities.
// Param2: The previous position of the entity (during the last frame).
// Param3: Specifies the group of entities that should be checked for collision with the given entity.
// Param4: Out parameter that will hold a pointer to the colliding object that is closest to the collidable object specified in Param1. 
//         Null if there is no collision at all. 
// Returns true if the entity is about to collide with an entity of the specified group, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::CheckCollision(const CollidableObject* pCollidableObject,  const XMFLOAT2& oldPosition, EntityGroup entityGroup, CollidableObject*& outCollisionObject)
{
	float shortestSquareDistance = std::numeric_limits<float>::max();
	outCollisionObject = nullptr;

	// Determine the points of the line that the colliders of other entities will be checked against
	
	// Current position of the entity
	XMFLOAT2 start = oldPosition;
	// The position, where the entity will at the end of the frame
	XMFLOAT2 end = pCollidableObject->GetPosition();

	if(entityGroup != GroupObstacles)
	{
		for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
		{
			if(m_soldiers[i].IsAlive() && (m_soldiers[i].GetId() != pCollidableObject->GetId()) &&
			   (entityGroup == GroupAllSoldiersAndObstacles ||
				entityGroup == GroupAllSoldiers ||
				(m_soldiers[i].GetTeam() == TeamRed && (entityGroup == GroupTeamRed || entityGroup == GroupTeamRedAndObstacles)) ||
				(m_soldiers[i].GetTeam() == TeamBlue && (entityGroup == GroupTeamBlue || entityGroup == GroupTeamBlueAndObstacles)))				)
			{
				if(m_soldiers[i].GetCollider()->CheckLineCollision(start, end))
				{
					float squareDistance = 0.0f;
					XMVECTOR vector = XMLoadFloat2(&m_soldiers[i].GetPosition()) - XMLoadFloat2(&pCollidableObject->GetPosition());
					XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));

					if(squareDistance < shortestSquareDistance)
					{
						shortestSquareDistance = squareDistance;
						outCollisionObject = &(m_soldiers[i]);
					}
				}
			}
		}
	}

	if(entityGroup == GroupObstacles || entityGroup == GroupTeamRedAndObstacles || entityGroup == GroupTeamBlueAndObstacles || entityGroup == GroupAllSoldiersAndObstacles)
	{
		float distance = 0.0f;
		XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&pCollidableObject->GetPosition()) - XMLoadFloat2(&oldPosition)));

		// Only check nearby obstacles for collision
		unsigned int maxGridDistance = static_cast<unsigned int>(distance / m_gridSpacing) + 1;

		// Get the current grid position of the entity
		XMFLOAT2 gridPos;
		WorldToGridPosition(pCollidableObject->GetPosition(), gridPos);

		unsigned int startX = (gridPos.x > maxGridDistance) ? (static_cast<int>(gridPos.x) - maxGridDistance) : 0;
		unsigned int startY = (gridPos.y > maxGridDistance) ? (static_cast<int>(gridPos.y) - maxGridDistance) : 0;
		unsigned int endX = (gridPos.x + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.x) + maxGridDistance) : (m_numberOfGridPartitions - 1);
		unsigned int endY = (gridPos.y + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.y) + maxGridDistance) : (m_numberOfGridPartitions - 1);

		// Check the grid within that distance for colliding obstacles
		for(unsigned int i = startX; i <= endX; ++i)
		{
			for(unsigned int k = startY; k <= endY; ++k)
			{
				if(m_pNodes[i][k].IsObstacle())
				{
					//if(m_pNodes[i][k].GetObstacle()->GetCollider()->CheckLineCollision(XMFLOAT2(start.x + 25.0f, start.y + 25.0f), XMFLOAT2(end.x + 25.0f, end.y + 25.0f)))
					if(m_pNodes[i][k].GetObstacle()->GetCollider()->CheckLineCollision(start, end))
					{
						float squareDistance = 0.0f;
						XMVECTOR vector = XMLoadFloat2(&m_pNodes[i][k].GetWorldPosition()) - XMLoadFloat2(&pCollidableObject->GetPosition());
						XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));

						if(squareDistance < shortestSquareDistance)
						{
							shortestSquareDistance = squareDistance;
							outCollisionObject = m_pNodes[i][k].GetObstacle();
						}
					}
				}
			}
		}
	}

	if(entityGroup == GroupObjectivesTeamRed)
	{
		if(m_objectives[TeamRed].GetCollider()->CheckLineCollision(start, end))
		{
			outCollisionObject = &(m_objectives[TeamRed]);
		}
	}

	if(entityGroup == GroupObjectivesTeamBlue)
	{
		if(m_objectives[TeamBlue].GetCollider()->CheckLineCollision(start, end))
		{
			outCollisionObject = &(m_objectives[TeamBlue]);
		}
	}

	if(entityGroup == GroupAllObjectives)
	{
		for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
		{
			if(m_objectives[i].GetCollider()->CheckLineCollision(start, end))
			{
				float squareDistance = 0.0f;
				XMVECTOR vector = XMLoadFloat2(&m_soldiers[i].GetPosition()) - XMLoadFloat2(&pCollidableObject->GetPosition());
				XMStoreFloat(&squareDistance, XMVector2Dot(vector, vector));

				if(squareDistance < shortestSquareDistance)
				{
					shortestSquareDistance = squareDistance;
					outCollisionObject = &(m_soldiers[i]);
				}
			}
		}
	}

	return (outCollisionObject != nullptr);
}

//--------------------------------------------------------------------------------------
// Update the respawn timers of all dead entites and respawn the ones, whose timer has
// surpassed the defined respawn time limit.
// Param1: The time passed since the last update.
//--------------------------------------------------------------------------------------
void TestEnvironment::UpdateRespawns(float deltaTime)
{
	std::list<std::pair<float, Entity*>>::iterator it = m_deadEntities.begin();

	while(it != m_deadEntities.end())
	{
		// Update the respawn timer
		it->first += deltaTime;

		if(it->first >= g_kRespawnTimer)
		{
			// Respawn the entity at a random respawn point and with a random rotation

			RespawnEventData data(m_spawnPoints[it->second->GetTeam()][rand() % m_spawnPoints[it->second->GetTeam()].size()]);
			SendEvent(it->second, RespawnEventType, &data);

			it = m_deadEntities.erase(it);
		}else
		{
			++it;
		}
	}
}

//--------------------------------------------------------------------------------------
// Returns the attack direction corresponding to a given rotation. Used to determine the
// attack directions for attack positions placed in the test environment. The attack direction 
// is opposite to the direction the attack point is actually facing. For instance, an attack
// position facing east (90 degree rotation) is a position suitable to attack the west side of 
// the enemy base, which is why west will be returned.
// Param1: The rotation, for which to get the direction.
// Returns the direction corresponding to the provided rotation.
//--------------------------------------------------------------------------------------
Direction TestEnvironment::GetAttackDirectionFromRotation(float rotation)
{
	// Note: Add additional code to return the closest direction for other
	//       provided rotations.

	if(rotation == 0.0f)
	{
		return South;
	}else if(rotation == 90.0f || rotation == -270.0f)
	{
		return West;
	}else if(rotation == 180.0f)
	{
		return North;
	}else if(rotation == 270.0f || rotation == -90.0f)
	{
		return East;
	}else
	{
		// As a default, return North (shouldn't be reached)
		return North;
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
bool TestEnvironment::CheckLineOfSightGrid(int startGridX, int startGridY, int endGridX, int endGridY)
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
		if((m_pNodes[y][x].IsObstacle() && steep) || (m_pNodes[x][y].IsObstacle() && !steep))
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
// Determines whether there is a direct line of sight between two points.
// Param1: The start position of the line.
// Param2: The end position of the line.
// Returns true if a direct line of sight exists, false if an obstacle obstructs the view.
//--------------------------------------------------------------------------------------
bool TestEnvironment::CheckLineOfSight(const XMFLOAT2& start, const XMFLOAT2& end)
{
	XMFLOAT2 line(0.0f, 0.0f);
	float distance = 0.0f;

	XMStoreFloat2(&line, XMLoadFloat2(&end) - XMLoadFloat2(&start));
	XMStoreFloat(&distance, XMVector2Length(XMLoadFloat2(&line)));

	// Only check nearby obstacles for collision
	unsigned int maxGridDistance = static_cast<unsigned int>(distance / m_gridSpacing) + 1;

	// Get the current grid position of the entity
	XMFLOAT2 gridPos;
	WorldToGridPosition(start, gridPos);

	unsigned int startX = 0;
	unsigned int endX   = 0;
	unsigned int startY = 0;
	unsigned int endY   = 0;

	if(line.x >= 0.0f)
	{
		startX = static_cast<unsigned int>(gridPos.x);
		endX = (gridPos.x + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.x) + maxGridDistance) : (m_numberOfGridPartitions - 1);
	}else
	{
		startX = (gridPos.x > maxGridDistance) ? (static_cast<int>(gridPos.x) - maxGridDistance) : 0;
		endX = static_cast<unsigned int>(gridPos.x);
	}

	if(line.y >= 0.0f)
	{
		startY = static_cast<unsigned int>(gridPos.y);
		endY = (gridPos.y + maxGridDistance < m_numberOfGridPartitions) ? (static_cast<int>(gridPos.y) + maxGridDistance) : (m_numberOfGridPartitions - 1);
	}else
	{
		startY = (gridPos.y > maxGridDistance) ? (static_cast<int>(gridPos.y) - maxGridDistance) : 0;
		endY = static_cast<unsigned int>(gridPos.y);
	}

	// Check the grid within that distance for colliding obstacles
	for(unsigned int i = startX; i <= endX; ++i)
	{
		for(unsigned int k = startY; k <= endY; ++k)
		{
			if(m_pNodes[i][k].IsObstacle())
			{
				if(m_pNodes[i][k].GetObstacle()->GetCollider()->CheckLineCollision(start, end))
				{
					return false;
				}
			}
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Resets the node graph as a preparation measure for the simulation.
//--------------------------------------------------------------------------------------
void TestEnvironment::ResetNodeGraph(void)
{
	for(unsigned int i = 0; i < m_numberOfGridPartitions; ++i)
	{
		for(unsigned int k = 0; k < m_numberOfGridPartitions; ++k)
		{
			m_pNodes[i][k].Reset();
		}
	}
}

//--------------------------------------------------------------------------------------
// Registers the death of an entity and broadcasts the event to all other entities. 
// Also prepares the respawn of the entity.
// Param1: The id of the entity that died.
//--------------------------------------------------------------------------------------
void TestEnvironment::AddDeadEntity(unsigned long id)
{
	// Find the soldier with the given id
	Soldier* pDeadSoldier = std::find_if(std::begin(m_soldiers), std::end(m_soldiers), Soldier::FindSoldierById(id));
	if (pDeadSoldier != std::end(m_soldiers))
	{
	   m_deadEntities.push_back(std::pair<float, Entity*>(0.0f, pDeadSoldier));
	}
}

//--------------------------------------------------------------------------------------
// Processes an event by writing it to a log file and/or updating the statistics for this
// simulation. Simply forwards the calls to the logger object of the test environment.
// Param1: A pointer to the first object involved in the event.
// Param2: A pointer to the second object involved in the event.
//--------------------------------------------------------------------------------------
void TestEnvironment::RecordEvent(LogEventType type, void* pObject1, void* pObject2)
{
	m_logger.LogEvent(type, pObject1, pObject2);
}

//--------------------------------------------------------------------------------------
// Tells whether a certain world position is blocked by an obstacle.
// Param1: The world position to check.
// Returns true if the position is blocked by an obstacle or if the position does not lie
// within the grid at all, false otherwise.
//--------------------------------------------------------------------------------------
bool TestEnvironment::IsBlocked(const XMFLOAT2 worldPos) const
{

	XMFLOAT2 gridPos(0.0f, 0.0f);
	WorldToGridPosition(worldPos, gridPos);

	// If the position is invalid, return blocked in any case
	if(static_cast<unsigned int>(gridPos.x) > GetNumberOfGridPartitions() || static_cast<unsigned int>(gridPos.y) > GetNumberOfGridPartitions())
	{
		return true;
	}

	return m_pNodes[static_cast<unsigned int>(gridPos.x)][static_cast<unsigned int>(gridPos.y)].IsObstacle();
}

//--------------------------------------------------------------------------------------
// Tells if and if so which team owns a certain tile of the test environment.
// Param1: The world position, for which the corresponding grid field should be checked for
//         ownership by the teams.
// Returns the team the grid field belongs to (part of the base of the team).
//--------------------------------------------------------------------------------------
EntityTeam TestEnvironment::GetTerritoryOwner(const XMFLOAT2 worldPos) const
{
	XMFLOAT2 gridPos(0.0f, 0.0f);
	WorldToGridPosition(worldPos, gridPos);

	return m_pNodes[static_cast<unsigned int>(gridPos.x)][static_cast<unsigned int>(gridPos.y)].GetTerritoryOwner();
}

//--------------------------------------------------------------------------------------
// Starts the simulation, switches from edit to simulation mode.
// Returns true if the simulation could be started successfully.
//--------------------------------------------------------------------------------------
bool TestEnvironment::StartSimulation(void)
{

	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		if(!m_flagSet[i] || m_soldierCount[i] < g_kSoldiersPerTeam || m_spawnPointCount[i] < 1 || m_attackPositionsCount[i] < 1)
		{
			return false;
		}
	}

	PrepareSimulation();

	for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
	{
		m_soldiers[i].Activate();
	}

	m_isInEditMode = false;
	m_isPaused = false;

#ifdef DEBUG
	m_logger.Open("Log.txt");
#endif
	

	return true;
}

//--------------------------------------------------------------------------------------
// Stops the simulation and resets the test environment into its starting state.
//--------------------------------------------------------------------------------------
void TestEnvironment::EndSimulation(void)
{
#ifdef DEBUG
	m_logger.Close();
#endif

	// Delete all projectiles
	m_projectiles.clear();
	m_obstacles.clear();
	m_deadEntities.clear();

	// Reset game context

	for(unsigned int i = 0; i < g_kSoldiersPerTeam * (NumberOfTeams-1); ++i)
	{
		m_soldiers[i].Reset();
	}

	for(unsigned int i = 0; i < NumberOfTeams-1; ++i)
	{
		m_spawnPoints[i].clear();
		m_pTeamAI[i]->Reset();
		m_baseEntrances[i].clear();
		m_attackPositions[i].clear();
		m_baseFieldPositions[i].clear();
	}

	m_id = 0;

	m_isInEditMode = true;
	m_isPaused = true;

	m_pGameContext->Reset();

	ResetCommunication();
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
// Updates the node graph underlying the test environment. During simulation the graph will 
// be used for pathfinding, some collision detection and other things.
//--------------------------------------------------------------------------------------
void TestEnvironment::UpdateNodeGraph(void)
{
	// Update cover positions
	for(std::list<Obstacle>::iterator it = m_obstacles.begin(); it != m_obstacles.end(); ++it)
	{
		UpdateCoverSpots(&(*it));
	}

	// Update team base territories
	for(std::vector<EditModeObject>::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); ++it)
	{
		if(it->GetType() == RedBaseAreaType)
		{
			XMFLOAT2 gridPos(0.0f, 0.0f);
			WorldToGridPosition(it->GetPosition(), gridPos);
			m_pNodes[static_cast<unsigned int>(gridPos.x)][static_cast<unsigned int>(gridPos.y)].SetTerritoryOwner(TeamRed);
		}else if(it->GetType() == BlueBaseAreaType)
		{
			XMFLOAT2 gridPos(0.0f, 0.0f);
			WorldToGridPosition(it->GetPosition(), gridPos);
			m_pNodes[static_cast<unsigned int>(gridPos.x)][static_cast<unsigned int>(gridPos.y)].SetTerritoryOwner(TeamBlue);
		}else if(it->GetType() == RedAttackPositionType)
		{
			XMFLOAT2 gridPos(0.0f, 0.0f);
			WorldToGridPosition(it->GetPosition(), gridPos);
			m_pNodes[static_cast<unsigned int>(gridPos.x)][static_cast<unsigned int>(gridPos.y)].SetAttackPosition(TeamRed);
		}else if(it->GetType() == BlueAttackPositionType)
		{
			XMFLOAT2 gridPos(0.0f, 0.0f);
			WorldToGridPosition(it->GetPosition(), gridPos);
			m_pNodes[static_cast<unsigned int>(gridPos.x)][static_cast<unsigned int>(gridPos.y)].SetAttackPosition(TeamBlue);
		}
	}

	// Update base entrances
	UpdateBaseEntrances();

}


//--------------------------------------------------------------------------------------
// Updates the nodes of the graph according to the new obstacle added to the environment.
// Nodes next to the new obstacle node are registered as providing cover towards some
// directions, based on the relative position of the obstacle.
// Param1: A pointer to the collidable object (obstacle) blocking this node for traversal.
//--------------------------------------------------------------------------------------
void TestEnvironment::UpdateCoverSpots(CollidableObject* obstacle)
{
	if(!obstacle)
	{
		return;
	}

	// Determine the node that the obstacle is placed in.
	XMFLOAT2 gridPos;
	WorldToGridPosition(obstacle->GetPosition(), gridPos);

	unsigned int gridX = static_cast<unsigned int>(gridPos.x);
	unsigned int gridY = static_cast<unsigned int>(gridPos.y);

	m_pNodes[gridX][gridY].SetObstacle(obstacle);

	// Update the adjacent nodes with regard to the added/deleted cover spot

	if(gridX > 0)
	{
		m_pNodes[gridX-1][gridY].SetCovered(East, true);
		if(gridY > 0)
		{
			m_pNodes[gridX-1][gridY-1].SetCovered(NorthEast, true);
		}
		if(gridY < m_numberOfGridPartitions-1)
		{
			m_pNodes[gridX-1][gridY+1].SetCovered(SouthEast, true);
		}
	}

	if(gridX < m_numberOfGridPartitions-1)
	{
		m_pNodes[gridX+1][gridY].SetCovered(West, true);
		if(gridY > 0)
		{
			m_pNodes[gridX+1][gridY-1].SetCovered(NorthWest, true);
		}
		if(gridY < m_numberOfGridPartitions-1)
		{
			m_pNodes[gridX+1][gridY+1].SetCovered(SouthWest, true);
		}
	}

	if(gridY > 0)
	{
		m_pNodes[gridX][gridY-1].SetCovered(North, true);
	}

	if(gridY < m_numberOfGridPartitions-1)
	{
		m_pNodes[gridX][gridY+1].SetCovered(South, true);
	}
}

//--------------------------------------------------------------------------------------
// Updates the nodes of the graph according to the base territories and marks nodes that
// are entrances into the territory of each team.
//--------------------------------------------------------------------------------------
void TestEnvironment::UpdateBaseEntrances(void)
{
	for(unsigned int x = 0; x < m_numberOfGridPartitions; ++x)
	{
		for(unsigned int y = 0; y < m_numberOfGridPartitions; ++y)
		{
			if(m_pNodes[x][y].GetTerritoryOwner() != EntityTeam::None)
			{
				// Check west direction
				if((x > 0) && (!m_pNodes[x-1][y].IsObstacle()) && (m_pNodes[x-1][y].GetTerritoryOwner() != m_pNodes[x][y].GetTerritoryOwner()))
				{
					// Entrance from the west
					std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator foundIt = m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].find(West);
					if(foundIt != m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].end())
					{
						// Add the entrance position to that direction
						foundIt->second.push_back(m_pNodes[x][y].GetWorldPosition());
					}else
					{
						// Add a new entry for that direction
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].insert(std::pair<Direction, std::vector<XMFLOAT2>>(West, std::vector<XMFLOAT2>()));
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()][West].push_back(m_pNodes[x][y].GetWorldPosition());
					}
					m_pNodes[x][y].SetEntranceToBase(true);
				}
				// Check east direction
				if((x < m_numberOfGridPartitions-1) && (!m_pNodes[x+1][y].IsObstacle()) && (m_pNodes[x+1][y].GetTerritoryOwner() != m_pNodes[x][y].GetTerritoryOwner()))
				{
					// Entrance from the west
					std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator foundIt = m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].find(East);
					if(foundIt != m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].end())
					{
						// Add the entrance position to that direction
						foundIt->second.push_back(m_pNodes[x][y].GetWorldPosition());
					}else
					{
						// Add a new entry for that direction
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].insert(std::pair<Direction, std::vector<XMFLOAT2>>(East, std::vector<XMFLOAT2>()));
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()][East].push_back(m_pNodes[x][y].GetWorldPosition());
					}
					m_pNodes[x][y].SetEntranceToBase(true);
				}
				// Check north direction
				if((y < m_numberOfGridPartitions-1) && (!m_pNodes[x][y+1].IsObstacle()) && (m_pNodes[x][y+1].GetTerritoryOwner() != m_pNodes[x][y].GetTerritoryOwner()))
				{
					// Entrance from the west
					std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator foundIt = m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].find(North);
					if(foundIt != m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].end())
					{
						// Add the entrance position to that direction
						foundIt->second.push_back(m_pNodes[x][y].GetWorldPosition());
					}else
					{
						// Add a new entry for that direction
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].insert(std::pair<Direction, std::vector<XMFLOAT2>>(North, std::vector<XMFLOAT2>()));
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()][North].push_back(m_pNodes[x][y].GetWorldPosition());
					}
					m_pNodes[x][y].SetEntranceToBase(true);
				}
				// Check south direction
				if((y > 0) && (!m_pNodes[x][y-1].IsObstacle()) && (m_pNodes[x][y-1].GetTerritoryOwner() != m_pNodes[x][y].GetTerritoryOwner()))
				{
					// Entrance from the west
					std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator foundIt = m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].find(South);
					if(foundIt != m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].end())
					{
						// Add the entrance position to that direction
						foundIt->second.push_back(m_pNodes[x][y].GetWorldPosition());
					}else
					{
						// Add a new entry for that direction
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].insert(std::pair<Direction, std::vector<XMFLOAT2>>(South, std::vector<XMFLOAT2>()));
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()][South].push_back(m_pNodes[x][y].GetWorldPosition());
					}
					m_pNodes[x][y].SetEntranceToBase(true);
				}

				// Not used at the moment
				/*
				// Check north-east direction
				if((x < m_numberOfGridPartitions-1) && (y < m_numberOfGridPartitions-1) && (!m_pNodes[x+1][y+1].IsObstacle()) && (m_pNodes[x+1][y+1].GetTerritoryOwner() != m_pNodes[x][y].GetTerritoryOwner()) && ((!m_pNodes[x][y+1].IsObstacle()) || (!m_pNodes[x+1][y].IsObstacle())))
				{
					// Entrance from the west
					std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator foundIt = m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].find(NorthEast);
					if(foundIt != m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].end())
					{
						// Add the entrance position to that direction
						foundIt->second.push_back(m_pNodes[x][y].GetWorldPosition());
					}else
					{
						// Add a new entry for that direction
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].insert(std::pair<Direction, std::vector<XMFLOAT2>>(NorthEast, std::vector<XMFLOAT2>()));
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()][NorthEast].push_back(m_pNodes[x][y].GetWorldPosition());
					}
					m_pNodes[x][y].SetEntranceToBase(true);
				}
				// Check North-west direction
				if((x > 0) && (y < m_numberOfGridPartitions-1) && (!m_pNodes[x-1][y+1].IsObstacle()) && (m_pNodes[x-1][y+1].GetTerritoryOwner() != m_pNodes[x][y].GetTerritoryOwner()) && ((!m_pNodes[x][y+1].IsObstacle()) || (!m_pNodes[x-1][y].IsObstacle())))
				{
					// Entrance from the west
					std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator foundIt = m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].find(NorthWest);
					if(foundIt != m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].end())
					{
						// Add the entrance position to that direction
						foundIt->second.push_back(m_pNodes[x][y].GetWorldPosition());
					}else
					{
						// Add a new entry for that direction
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].insert(std::pair<Direction, std::vector<XMFLOAT2>>(NorthWest, std::vector<XMFLOAT2>()));
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()][NorthWest].push_back(m_pNodes[x][y].GetWorldPosition());
					}
					m_pNodes[x][y].SetEntranceToBase(true);
				}
				// Check South-east direction
				if((x < m_numberOfGridPartitions-1) && (y > 0) && (!m_pNodes[x+1][y-1].IsObstacle()) && (m_pNodes[x+1][y-1].GetTerritoryOwner() != m_pNodes[x][y].GetTerritoryOwner()) && ((!m_pNodes[x][y-1].IsObstacle()) || (!m_pNodes[x+1][y].IsObstacle())))
				{
					// Entrance from the west
					std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator foundIt = m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].find(SouthEast);
					if(foundIt != m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].end())
					{
						// Add the entrance position to that direction
						foundIt->second.push_back(m_pNodes[x][y].GetWorldPosition());
					}else
					{
						// Add a new entry for that direction
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].insert(std::pair<Direction, std::vector<XMFLOAT2>>(SouthEast, std::vector<XMFLOAT2>()));
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()][SouthEast].push_back(m_pNodes[x][y].GetWorldPosition());
					}
					m_pNodes[x][y].SetEntranceToBase(true);
				}
				// Check South-west direction
				if((x > 0) && (y > 0) && (!m_pNodes[x-1][y-1].IsObstacle()) && (m_pNodes[x-1][y-1].GetTerritoryOwner() != m_pNodes[x][y].GetTerritoryOwner()) && ((!m_pNodes[x][y-1].IsObstacle()) || (!m_pNodes[x-1][y].IsObstacle())))
				{
					// Entrance from the west
					std::unordered_map<Direction, std::vector<XMFLOAT2>>::iterator foundIt = m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].find(SouthWest);
					if(foundIt != m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].end())
					{
						// Add the entrance position to that direction
						foundIt->second.push_back(m_pNodes[x][y].GetWorldPosition());
					}else
					{
						// Add a new entry for that direction
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()].insert(std::pair<Direction, std::vector<XMFLOAT2>>(SouthWest, std::vector<XMFLOAT2>()));
						m_baseEntrances[m_pNodes[x][y].GetTerritoryOwner()][SouthWest].push_back(m_pNodes[x][y].GetWorldPosition());
					}
					m_pNodes[x][y].SetEntranceToBase(true);
				}*/
			}
		}
	}
}

// Data access functions
	
bool TestEnvironment::IsPaused(void) const
{
	return m_isPaused;
}

const GameContext* TestEnvironment::GetGameContext(void) const
{
	return m_pGameContext;
}

const std::unordered_map<Direction, std::vector<XMFLOAT2>>& TestEnvironment::GetBaseEntrances(EntityTeam team) const
{
	return m_baseEntrances[team];
}

const std::unordered_map<Direction, std::vector<XMFLOAT2>>& TestEnvironment::GetAttackPositions(EntityTeam team) const
{
	return m_attackPositions[team];
}

const std::vector<XMFLOAT2>& TestEnvironment::GetBaseFieldPositions(EntityTeam team) const
{
	return m_baseFieldPositions[team];
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
