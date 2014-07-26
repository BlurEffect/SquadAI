/* 
*  Kevin Meergans, SquadAI, 2014
*  Entity.h
*  This is the abstract base class for all objects that can be present
*  within a test environment.
*/

#ifndef ENTITY_H
#define ENTITY_H

// Includes
#include <DirectXMath.h>
#include "EntityData.h"
#include "Collider.h"
#include "CircleCollider.h"
#include "AxisAlignedRectangleCollider.h"

// Forward declarations
class TestEnvironment;

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Bundles the data defining the properties for an entity.
//--------------------------------------------------------------------------------------
struct EntityInitData
{
	EntityInitData(void) : m_id(0),
						   m_team(EntityTeam(None)),
						   m_type(ObjectType(0)),
						   m_position(0.0f, 0.0f),
						   m_rotation(0.0f),
					 	   m_scale(1.0f),
						   m_colliderType(ColliderType(0)),
					  	   m_pCollider(nullptr),
						   m_pEnvironment(nullptr)
	{}
	
	EntityInitData(unsigned long id, EntityTeam team, ObjectType type, const XMFLOAT2& position, float rotation, float scale, ColliderType colliderType, Collider* pCollider, TestEnvironment* pEnvironment)
						 : m_id(id), 
						   m_team(team),
						   m_type(type),
						   m_position(position),
						   m_rotation(rotation),
						   m_scale(scale),
						   m_colliderType(colliderType),
						   m_pCollider(pCollider),
						   m_pEnvironment(pEnvironment)
	{}

	unsigned long	 m_id;			 // A unique identifier for the entity, 0 is an invalid value
	EntityTeam       m_team;         // Identifies the team this entity belongs to
	ObjectType		 m_type;         // Identifies the type of this entity
	XMFLOAT2		 m_position;     // The position, where this entity is placed
	float			 m_rotation;     // The z-axis rotation of the entity
	float            m_scale;        // The uniform scale factor for the entity
	ColliderType     m_colliderType; // The type of the collider for this entity.
	Collider*        m_pCollider;	 // The pointer to the collider that should be used to initialise this entity's collider
	TestEnvironment* m_pEnvironment; // A pointer to the test environment the entity is "living" in
};


class Entity
{
public:
	Entity(void);
	virtual ~Entity(void) = 0;

	bool Initialise(const EntityInitData& initData);
	virtual void Update(float deltaTime);
	virtual void Activate(void);
	virtual void Reset(void);

	// Data access functions
	unsigned long	 GetId(void) const;
	EntityTeam       GetTeam(void) const;
	ObjectType		 GetType(void) const;
	const XMFLOAT2&	 GetPosition(void) const;
	float			 GetRotation(void) const;
	float	         GetScale(void) const;
	Collider*        GetCollider(void);
	TestEnvironment* GetTestEnvironment(void);

	void SetId(unsigned long id);
	void SetTeam(EntityTeam team);
	void SetType(ObjectType type);
	void SetPosition(const XMFLOAT2& position);
	void SetRotation(float rotation);
	void SetScale(float rotation);
	void SetTestEnvironment(TestEnvironment* pEnvironment);

private:

	bool CreateCollider(ColliderType type, Collider* pCollider);

	unsigned long	 m_id;				  // A unique identifier for this entity, 0 is an invalid value
	EntityTeam       m_team;              // The team the entity belongs to
	ObjectType		 m_type;			  // Identifies the type of this entity
	XMFLOAT2		 m_position;		  // The position, where this entity is placed
	float			 m_rotation;		  // The z-axis rotation of the entity
	float            m_scale;			  // The uniform scale factor for the entity
	Collider*        m_pCollider;         // The collider associated to this entity
	TestEnvironment* m_pEnvironment;	  // A pointer to the test environment the entity is "living" in
};

#endif // ENTITY_H