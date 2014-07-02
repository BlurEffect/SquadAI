/* 
*  Kevin Meergans, SquadAI, 2014
*  Soldier.h
*  Represents a fighting entity in the test environment.
*/

#ifndef SOLDIER_H
#define SOLDIER_H

// Includes
#include <DirectXMath.h>
#include "Entity.h"

using namespace DirectX;

class Soldier : public Entity
{
public:
	Soldier(void);
	Soldier(EntityType type, const XMFLOAT3& position, float rotation);
	~Soldier(void);
private:
};

#endif // SOLDIER_H