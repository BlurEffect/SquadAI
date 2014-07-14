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

class TestEnvironment;

using namespace DirectX;

class Entity
{
public:
	Entity(void);
	Entity::Entity(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation, float scale, float radius, TestEnvironment* pEnvironment);
	virtual ~Entity(void) = 0;

	// Data access functions
	unsigned long	 GetId(void) const;
	EntityType		 GetType(void) const;
	const XMFLOAT2&	 GetPosition(void) const;
	float			 GetRotation(void) const;
	float	         GetScale(void) const;
	float		     GetRadius(void) const;
	TestEnvironment* GetTestEnvironment(void);

	void SetId(unsigned long id);
	void SetType(EntityType type);
	void SetPosition(const XMFLOAT2& position);
	void SetRotation(float rotation);
	void SetScale(float rotation);
	void SetRadius(float rotation);
	void SetTestEnvironment(TestEnvironment* pEnvironment);

private:
	unsigned long	 m_id;			 // A unique identifier for this entity, 0 is an invalid value
	EntityType		 m_type;         // Identifies the type of this entity
	XMFLOAT2		 m_position;     // The position, where this entity is placed
	float			 m_rotation;     // The z-axis rotation of the entity
	float            m_scale;        // The uniform scale factor for the entity
	float		     m_radius;       // The radius of a circle with centre in the entity's position that contains the entity
	TestEnvironment* m_pEnvironment; // A pointer to the test environment the entity is "living" in
};

#endif // ENTITY_H