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
	RushBaseAttackManoeuvre,
	RunTheFlagHomeManoeuvre,
	ReturnDroppedFlagManoeuvre,
	CoordinatedBaseAttackManoeuvre,
	DistractionBaseAttackManoeuvre,
	SimpleBaseAttackManoeuvre,
	PickUpDroppedFlagManoeuvre
};

//--------------------------------------------------------------------------------------
// The team AI is only able to execute one manoeuvre of a category at a time. These
// are the currently available categories.
//--------------------------------------------------------------------------------------
enum TeamManoeuvreCategory
{
	AttackEnemyFlagCategory, // Contains all manoeuvres that attack the enemy flag
	ProtectOwnFlagCategory,  // Contains all manoeuvres that protect the own flag of the team
	NumberOfManoeuvreCategories
};


class TeamManoeuvre : public Communicator
{
public:
	TeamManoeuvre(TeamManoeuvreType type, TeamManoeuvreCategory category, unsigned int minNumberParticipants, unsigned int maxNumberParticipants);
	~TeamManoeuvre(void);

	void AddParticipant(Entity* pEntity);
	void RemoveParticipant(unsigned long id);

	// To be overwritten by derived manoeuvre classes

	virtual BehaviourStatus Initiate(void) = 0;
	virtual BehaviourStatus Update(float deltaTime) = 0;
	virtual void			Terminate(void) = 0;

	virtual void Reset(void);
	virtual void ProcessEvent(EventType type, void* pEventData);

	bool IsParticipant(unsigned long id) const;

	static const char* GetManoeuvreNameFromType(TeamManoeuvreType type);

	// Data access functions

	bool						IsActive(void) const;
	bool						HasSucceeded(void) const;
	bool                        HasFailed(void) const;
	TeamManoeuvreType			GetType(void) const;
	TeamManoeuvreCategory       GetCategory(void) const;
	unsigned int				GetMinNumberOfParticipants(void) const;
	unsigned int				GetMaxNumberOfParticipants(void) const;
	const std::vector<Entity*>& GetParticipants(void) const;
	const Entity*               GetParticipant(unsigned long id) const;
	unsigned int                GetNumberOfParticipants(void) const;

	void SetActive(bool active);
	void SetSucceeded(bool succeeded);
	void SetFailed(bool failed);
	void SetType(TeamManoeuvreType type);
	void SetCategory(TeamManoeuvreCategory category);
	void SetMinNumberOfParticipants(unsigned int minNumberOfParticipants);
	void SetMaxNumberOfParticipants(unsigned int maxNumberOfParticipants);

protected:
	virtual void UpdateAttackOrders(unsigned long enemyId, const XMFLOAT2 newPosition);

	virtual void ProcessMessage(Message* pMessage);

	void CancelOrder(Entity* pEntity);
	void CancelOrder(unsigned long id);
	void ClearOrders(void);

	std::vector<Entity*>				      m_participants; // The entities currently participating in the manoeuvre
	std::unordered_map<unsigned long, Order*> m_activeOrders; // The active orders currently being carried out by the entities participating in the manoeuvre
	
private:
	bool				  m_active;                  // Tells whether the manoeuvre is currently being executed or inactive
	bool				  m_succeeded;               // Tells whether the manoeuvre succeeded 
	bool                  m_failed;					 // Tells whether the manoeuvre failed
	TeamManoeuvreType	  m_type;					 // Identifies the type of the manoeuvre
	TeamManoeuvreCategory m_category;				 // The category this manoeuvre is associated to
	unsigned int		  m_minNumberOfParticipants; // The minimal number of entities required to execute the manoeuvre
	unsigned int		  m_maxNumberOfParticipants; // The maximal number of entities allowed to execute the manoeuvre
};

#endif // TEAM_MANOEUVRE_H