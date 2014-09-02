/* 
*  Kevin Meergans, SquadAI, 2014
*  PickUpDroppedFlag.h
*  A team manoeuvre that orders the participating entities to move onto the position
*  of the dropped enemy flag with the goal to pick it up again in order to capture it.
*/

#ifndef PICK_UP_DROPPED_FLAG_H
#define PICK_UP_DROPPED_FLAG_H

// Includes
#include "TeamManoeuvre.h"

// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;


class PickUpDroppedFlag : public TeamManoeuvre
{
public:
	PickUpDroppedFlag(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI);
	~PickUpDroppedFlag(void);

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

#endif // PICK_UP_DROPPED_FLAG_H