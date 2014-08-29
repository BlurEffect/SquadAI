/* 
*  Kevin Meergans, SquadAI, 2014
*  ReturnDroppedFlag.h
*  A team manoeuvre that orders the participating entities to move onto the position
*  of the dropped flag of the team with the goal to return it.
*/

#ifndef RETURN_DROPPED_FLAG_H
#define RETURN_DROPPED_FLAG_H

// Includes
#include "TeamManoeuvre.h"

// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;


class ReturnDroppedFlag : public TeamManoeuvre
{
public:
	ReturnDroppedFlag(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI);
	~ReturnDroppedFlag(void);

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

	MultiflagCTFTeamAI* m_pTeamAI;   // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)
};

#endif // RETURN_DROPPED_FLAG_H