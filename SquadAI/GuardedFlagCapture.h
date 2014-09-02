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
// Bundles additional data required to initialise a distraction attack manoeuvre.
//--------------------------------------------------------------------------------------
struct GuardedFlagCaptureInitData
{
	GuardedFlagCaptureInitData(unsigned int numberOfSneakers, float waitForParticipantsInterval) 
		: m_numberOfSneakers(numberOfSneakers),
		  m_waitForParticipantsInterval(waitForParticipantsInterval)
	{}

	unsigned int m_numberOfSneakers;			// Determines how many of the participants will be tasked with stealing the flag
	float		 m_waitForParticipantsInterval; // Determines after what time the participants will start the actual attack
};

class GuardedFlagCapture : public TeamManoeuvre
{
public:
	GuardedFlagCapture(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, unsigned int numberOfSneakers, float waitForParticipantsInterval);
	~GuardedFlagCapture(void);

	// To be overwritten by derived manoeuvre classes

	BehaviourStatus Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access
	MultiflagCTFTeamAI* GetTeamAI(void);
	float GetWaitForParticipantsInterval(void) const;
	const XMFLOAT2& GetDistractionAssemblyPoint(void) const;
	const XMFLOAT2& GetSneakAssemblyPoint(void) const;
	unsigned int GetNumberOfSneakers(void) const;

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);
	void SetWaitForParticipantsInterval(float interval);
	void SetDistractionAssemblyPoint(const XMFLOAT2& assemblyPoint);
	void SetSneakAssemblyPoint(const XMFLOAT2& assemblyPoint);

protected:
	
	virtual void ProcessMessage(Message* pMessage);

private:

	//--------------------------------------------------------------------------------------
	// Available manoeuvre phases for this manoeuvre.
	//--------------------------------------------------------------------------------------
	enum ManoeuvrePhase
	{
		AssemblePhase,
		DistractionAttackPhase,
		SneakAttackPhase
	};

	void SetupGroups(void);
	bool DetermineAssemblyPoints(void);
	void StartDistractionAttack(void);
	void StartSneakAttack(void);

	ManoeuvrePhase m_currentPhase;       // The phase the manoeuvre is currently in
	float m_waitForParticipantsInterval; // Determines after what time the participants will start the actual attack
	float m_timer;					     // Keeps track of the time passed since the initiation of the manoeuvre
	MultiflagCTFTeamAI* m_pTeamAI;       // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)

	XMFLOAT2 m_distractionAssemblyPoint;            // The position, at which to assemble the participants before starting the distraction attack
	XMFLOAT2 m_sneakAssemblyPoint;   // The position, at which to assemble the participants before trying to sneak into the enemy base

	unsigned int m_numberOfSneakers; // The number of entities taking part in the sneak attack

	std::set<unsigned long> m_arrivedEntities; // Keeps track of the entities that have arrived at their assembly point

	std::set<unsigned long> m_distractionParticipants; // Keeps track of which participants take part in the distraction manoeuvre
	std::set<unsigned long> m_sneakParticipants;   // Keeps track of which participants take part in the flag steal manoeuvre
};

#endif // GUARDED_FLAG_CAPTURE_H