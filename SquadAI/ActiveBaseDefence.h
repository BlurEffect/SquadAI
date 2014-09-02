/* 
*  Kevin Meergans, SquadAI, 2014
*  ActiveBaseDefence.h
*  A team manoeuvre that orders the participating entities to patrol the area around the
*  base and attack any enemies that might be spotted. When one of the defenders is killed,
*  the other ones will investigate the position, where he died (if they are not involved in
*  fights themselves).
*/

#ifndef ACTIVE_BASE_DEFENCE_H
#define ACTIVE_BASE_DEFENCE_H

// Includes
#include "TeamManoeuvre.h"

// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;

//--------------------------------------------------------------------------------------
// Bundles additional data required to initialise a active base defence manoeuvre.
//--------------------------------------------------------------------------------------
struct ActiveBaseDefenceInitData
{
	ActiveBaseDefenceInitData(float patrolRadius) : m_patrolRadius(patrolRadius) {}

	float m_patrolRadius; // Determines the size of the area around the base flag position that the participants will patrol
};

class ActiveBaseDefence : public TeamManoeuvre
{
public:
	ActiveBaseDefence(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float patrolRadius);
	~ActiveBaseDefence(void);

	// To be overwritten by derived manoeuvre classes

	BehaviourStatus Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access
	const MultiflagCTFTeamAI* GetTeamAI(void) const;
	float GetPatrolRadius(void) const;

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);
	void SetPatrolRadius(float radius);

protected:
	
	virtual void ProcessMessage(Message* pMessage);

private:

	void InvestigatePosition(const XMFLOAT2& position);

	MultiflagCTFTeamAI* m_pTeamAI;      // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)
	float			    m_patrolRadius; // Determines the size of the area around the base flag position that the participants will patrol
};

#endif // ACTIVE_BASE_DEFENCE_H