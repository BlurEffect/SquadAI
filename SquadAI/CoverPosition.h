/* 
*  Kevin Meergans, SquadAI, 2014
*  CoverSpot.h
*  Represents a cover spot in the test environment.
*/

#ifndef COVER_POSITION_H
#define COVER_POSITION_H

// Includes
#include "Entity.h"

class CoverPosition : public Entity
{
public:
	CoverPosition(void);
	CoverPosition(unsigned long id, EntityType type, const XMFLOAT2& position, float rotation);
	~CoverPosition(void);
private:
};

#endif // COVER_POSITION_H