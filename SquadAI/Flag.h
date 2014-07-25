/* 
*  Kevin Meergans, SquadAI, 2014
*  Flag.h
*  Encapsulates a flag object.
*/

#ifndef FLAG_H
#define FLAG_H

// Includes
#include <DirectXMath.h>
#include "EntityData.h"
#include "Entity.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Identifies the current state of a flag.
//--------------------------------------------------------------------------------------
enum FlagState
{
	InBase, // The flag is placed at its original position within the base of the team it belongs to
	Stolen, // The flag was stolen and is being carried by a player of the opposing team
	Dropped // The flag was stolen but the carrier was killed and the flag was dropped
};

class Flag
{
public:
	Flag(void);
	~Flag(void);

	void Initialise(EntityTeam team, const XMFLOAT2& position, float rotation, float uniformScale, float resetInterval);

	bool OnPickUp(Entity* pEntity);
	void Update(float deltaTime);
	void OnDrop(void);
	void OnReset(void);

	// Data access functions

	EntityTeam      GetTeam(void) const;
	const XMFLOAT2& GetPosition(void) const;
	float           GetRotation(void) const;
	float			GetUniformScale(void) const;
	float           GetResetInterval(void) const;
	FlagState       GetState(void) const;
	const XMFLOAT2& GetResetPosition(void) const;
	float           GetResetTimePassed(void) const;
	const Entity*   GetCarrier(void) const;

	void SetTeam(EntityTeam team);
	void SetPosition(const XMFLOAT2& position);
	void SetRotation(float rotation);
	void SetUniformScale(float uniformScale);
	void SetResetInterval(float resetInterval);
	void SetResetPosition(const XMFLOAT2& position);
	void SetCarrier(Entity* pCarrier);

private:
	EntityTeam m_team;				// The team this flag belongs to
	XMFLOAT2   m_position;			// The current position of the flag
	float      m_rotation;			// The rotation of the flag
	float      m_uniformScale;		// The scale of the flag
	float      m_resetInterval;		// The flag will be reset when it was dropped for this long
	FlagState  m_currentState;		// The current state of the flag
	XMFLOAT2   m_resetPosition;		// The position, to which the flag will be reset to when dropped too long or after scoring
	float      m_resetTimePassed;   // A timer keeping track of how long the flag was dropped
	Entity*    m_pCarrier;			// A pointer to the entity carrying the flag, nullptr if flag is not being carried
};

#endif // FLAG_H