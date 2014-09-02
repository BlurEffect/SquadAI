/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleBaseAttack.h
*  A team manoeuvre that orders the participating entities to a small-scale attack on the enemy base in order
*  to steal the flag. This is a basic behaviour without any actual coordination of the
*  participants. It was mostly implemented as a fallback manoeuvre if other attack manoeuvres are
*  currently not available.
*/

#ifndef SIMPLE_BASE_ATTACK_H
#define SIMPLE_BASE_ATTACK_H

// Includes
#include "TeamManoeuvre.h"

// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;

class SimpleBaseAttack : public TeamManoeuvre
{
public:
	SimpleBaseAttack(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI);
	~SimpleBaseAttack(void);

	// To be overwritten by derived manoeuvre classes

	BehaviourStatus Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access
	const MultiflagCTFTeamAI* GetTeamAI(void) const;

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);

protected:
	
	virtual void ProcessMessage(Message* pMessage);

private:

	MultiflagCTFTeamAI* m_pTeamAI; // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)
};

#endif // SIMPLE_BASE_ATTACK_H