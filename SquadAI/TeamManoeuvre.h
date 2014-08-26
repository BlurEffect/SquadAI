/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamManoeuvre.h
*  Abstract base class for all team manoeuvres.
*/

#ifndef TEAM_MANOEUVRE_H
#define TEAM_MANOEUVRE_H

// Includes
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Communicator.h"

// Forward declarations
class Entity;
class Order;
enum BehaviourStatus;

//--------------------------------------------------------------------------------------
// Lists the available manoeuvres that team AIs can execute. Some make only sense for
// specific game modes.
//--------------------------------------------------------------------------------------
enum TeamManoeuvreType
{
	// Test manoeuvres
	TestAllAttackManoeuvre,
	TestAllMoveManoeuvre,
	TestAllDefendManoeuvre,
	// Actual manoeuvres
	DefendBaseEntrancesManoeuvre,
	RushBaseAttackManoeuvre
};


class TeamManoeuvre : public Communicator
{
public:
	TeamManoeuvre(TeamManoeuvreType type, unsigned int minNumberParticipants, unsigned int maxNumberParticipants);
	~TeamManoeuvre(void);

	void AddParticipant(Entity* pEntity);
	void RemoveParticipant(unsigned long id);

	// To be overwritten by derived manoeuvre classes

	virtual void 			Initiate(void) = 0;
	virtual BehaviourStatus Update(float deltaTime) = 0;
	virtual void			Terminate(void) = 0;

	virtual void Reset(void);
	virtual void ProcessEvent(EventType type, void* pEventData);

	bool IsParticipant(unsigned long id) const;

	// Data access functions

	bool						IsActive(void) const;
	bool						HasSucceeded(void) const;
	TeamManoeuvreType			GetType(void) const;
	unsigned int				GetMinNumberOfParticipants(void) const;
	unsigned int				GetMaxNumberOfParticipants(void) const;
	const std::vector<Entity*>& GetParticipants(void) const;
	const Entity*               GetParticipant(unsigned long id) const;
	unsigned int                GetNumberOfParticipants(void) const;

	void SetActive(bool active);
	void SetSucceeded(bool succeeded);
	void SetType(TeamManoeuvreType type);
	void SetMinNumberOfParticipants(unsigned int minNumberOfParticipants);
	void SetMaxNumberOfParticipants(unsigned int maxNumberOfParticipants);

protected:
	virtual void UpdateAttackOrders(unsigned long enemyId, const XMFLOAT2 newPosition);

	virtual bool ProcessMessage(Message* pMessage);

	void CancelOrder(Entity* pEntity);
	void CancelOrder(unsigned long id);
	void ClearOrders(void);

	std::vector<Entity*>				      m_participants; // The entities currently participating in the manoeuvre
	std::unordered_map<unsigned long, Order*> m_activeOrders; // The active orders currently being carried out by the entities participating in the manoeuvre
	
private:
	bool              m_active;                  // Tells whether the manoeuvre is currently being executed or inactive
	bool              m_succeeded;               // Tells whether the manoeuvre succeeded or failed
	TeamManoeuvreType m_type;					 // Identifies the type of the manoeuvre
	unsigned int      m_minNumberOfParticipants; // The minimal number of entities required to execute the manoeuvre
	unsigned int	  m_maxNumberOfParticipants; // The maximal number of entities allowed to execute the manoeuvre
};

#endif // TEAM_MANOEUVRE_H