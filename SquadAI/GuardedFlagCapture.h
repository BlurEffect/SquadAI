/* 
*  Kevin Meergans, SquadAI, 2014
*  GuardedFlagCapture.h
*  A team manoeuvre that orders the flag-carrying participant to run to the home base in 
*  order to capture the flag while the rest of the participating entities attempts to 
*  protect the flag carrier from any enemies trying to intercept him.
*/

#ifndef GUARDED_FLAG_CAPTURE_H
#define GUARDED_FLAG_CAPTURE_H

// Includes
#include <set>
#include "TeamManoeuvre.h"


// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;

//--------------------------------------------------------------------------------------
// Bundles additional data required to initialise a guarded flag capture manoeuvre.
//--------------------------------------------------------------------------------------
struct GuardedFlagCaptureInitData
{
	GuardedFlagCaptureInitData(float guardRadius, float updateMovementTargetsInterval) 
		: m_guardRadius(guardRadius),
		  m_updateMovementTargetsInterval(updateMovementTargetsInterval)
	{}

	float m_guardRadius;				  // Determines how close the protectors stay to the flag carrier
	float m_updateMovementTargetsInterval; // Determines how often the movement target is changed for the protectors
};

class GuardedFlagCapture : public TeamManoeuvre
{
public:
	GuardedFlagCapture(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float guardRadius, float updateMovementTargetsInterval);
	~GuardedFlagCapture(void);

	// To be overwritten by derived manoeuvre classes

	BehaviourStatus Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access functions

	const MultiflagCTFTeamAI* GetTeamAI(void) const ;
	float GetGuardRadius(void) const;
	float GetUpdateMovementTargetsInterval(void) const;

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);
	void SetGuardRadius(float radius);
	void SetUpdateMovementTargetsInterval(float interval);

protected:
	
	virtual void ProcessMessage(Message* pMessage);

private:

	void UpdateMovementTargets(void);

	float m_timer;					     // Keeps track of the time passed
	MultiflagCTFTeamAI* m_pTeamAI;       // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)

	float m_guardRadius;			// Determines how close the protectors stay to the flag carrier
	float m_updateMovementTargetsInterval; // Determines how often the movement target is changed for the protectors

	unsigned int m_flagCarrierId; // The number of entities taking part in the sneak attack
};

#endif // GUARDED_FLAG_CAPTURE_H