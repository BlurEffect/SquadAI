/* 
*  Kevin Meergans, SquadAI, 2014
*  InterceptFlagCarrier.h
*  A team manoeuvre that orders the participating entities prevent the enemy flag 
*  carrier from reaching his destination and capturing the flag of the team.
*/

#ifndef RETURN_DROPPED_FLAG_H
#define RETURN_DROPPED_FLAG_H

// Includes
#include "TeamManoeuvre.h"

// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;


class InterceptFlagCarrier : public TeamManoeuvre
{
public:
	InterceptFlagCarrier(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI);
	~InterceptFlagCarrier(void);

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

	bool SendOutPursuers(void);

	MultiflagCTFTeamAI* m_pTeamAI;   // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)
};

#endif // RETURN_DROPPED_FLAG_H