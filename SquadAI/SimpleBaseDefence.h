/* 
*  Kevin Meergans, SquadAI, 2014
*  SimpleBaseDefence.h
*  A team manoeuvre that orders the participating entities to defend their base. This is a 
*  basic small-scale behaviour without any actual coordination of the participants. It was 
*  mostly implemented as a fallback manoeuvre if other defense manoeuvres are currently not available.
*/

#ifndef SIMPLE_BASE_DEFENCE_H
#define SIMPLE_BASE_DEFENCE_H

// Includes
#include "TeamManoeuvre.h"

// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;


class SimpleBaseDefence : public TeamManoeuvre
{
public:
	SimpleBaseDefence(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI);
	~SimpleBaseDefence(void);

	// To be overwritten by derived manoeuvre classes

	BehaviourStatus Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access
	MultiflagCTFTeamAI* GetTeamAI(void);

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);

protected:
	
	virtual void ProcessMessage(Message* pMessage);

private:

	MultiflagCTFTeamAI* m_pTeamAI;   // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)
};

#endif // SIMPLE_BASE_DEFENCE_H