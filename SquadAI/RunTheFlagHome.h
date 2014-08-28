/* 
*  Kevin Meergans, SquadAI, 2014
*  RunTheFlagHome.h
*  A flag carrier behaviour that has the corresponding entity hurrying to its home base
*  in order to capture the flag.
*/

#ifndef RUN_THE_FLAG_HOME_H
#define RUN_THE_FLAG_HOME_H

// Includes
#include "TeamManoeuvre.h"

// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;


class RunTheFlagHome : public TeamManoeuvre
{
public:
	RunTheFlagHome(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI);
	~RunTheFlagHome(void);

	// To be overwritten by derived manoeuvre classes

	void			Initiate(void);
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

#endif // RUN_THE_FLAG_HOME_H