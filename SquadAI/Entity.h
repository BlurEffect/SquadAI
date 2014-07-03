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

using namespace DirectX;

class Entity
{
public:
	Entity(void);
	Entity::Entity(EntityType type, const XMFLOAT3& position, float rotation);
	virtual ~Entity(void) = 0;

	// Data access functions
	const XMFLOAT3& GetPosition(void) const;
	float           GetRotation(void) const;
	EntityType      GetType(void) const;

	void SetPosition(const XMFLOAT3& position);
	void SetRotation(float rotation);
	void SetType(EntityType type);
private:
	EntityType m_type;
	XMFLOAT3   m_position;
	float      m_rotation;
};

#endif // ENTITY_H