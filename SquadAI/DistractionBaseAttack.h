/* 
*  Kevin Meergans, SquadAI, 2014
*  DistractionBaseAttack.h
*  A team manoeuvre that orders most participating entities to perform a distraction attack
*  on the enemy base while a single participant attempts to steal the flag by entering the
*  base from another direction while the defenders are ideally busy with fighting off the
*  distraction attack.
*/

#ifndef DISTRACTION_BASE_ATTACK_H
#define DISTRACTION_BASE_ATTACK_H

// Includes
#include <set>
#include "TeamManoeuvre.h"


// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;

//--------------------------------------------------------------------------------------
// Bundles additional data required to initialise a distraction attack manoeuvre.
//--------------------------------------------------------------------------------------
struct DistractionBaseAttackInitData
{
	DistractionBaseAttackInitData(unsigned int numberOfSneakers, float waitForParticipantsInterval) 
		: m_numberOfSneakers(numberOfSneakers),
		  m_waitForParticipantsInterval(waitForParticipantsInterval)
	{}

	unsigned int m_numberOfSneakers;			// Determines how many of the participants will be tasked with stealing the flag
	float		 m_waitForParticipantsInterval; // Determines after what time the participants will start the actual attack
};

class DistractionBaseAttack : public TeamManoeuvre
{
public:
	DistractionBaseAttack(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, unsigned int numberOfSneakers, float waitForParticipantsInterval);
	~DistractionBaseAttack(void);

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

#endif // DISTRACTION_BASE_ATTACK_H