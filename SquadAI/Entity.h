/* 
*  Kevin Meergans, SquadAI, 2014
*  Entity.h
*  This is the abstract base class for AI controlled entities that are able
*  to move and fight.
*/

#ifndef ENTITY_H
#define ENTITY_H

// Includes
#include <DirectXMath.h>
#include "CollidableObject.h"
#include "ObjectTypes.h"

// Forward declarations
class TestEnvironment;

using namespace DirectX;

class Entity : public CollidableObject
{
public:
	Entity(void);
	virtual ~Entity(void) = 0;

	bool         Initialise(unsigned long id, const XMFLOAT2& position, float rotation, float uniformScale, ObjectCategory category, ColliderType colliderType, void* pColliderData, EntityTeam team);
	virtual void Update(float deltaTime) = 0;
	virtual void Activate(void) = 0;
	virtual void Reset(void) = 0;

	// Data access functions
	EntityTeam GetTeam(void) const;
	void SetTeam(EntityTeam team);

private:
	EntityTeam m_team; // The team the entity belongs to
};

#endif // ENTITY_H
