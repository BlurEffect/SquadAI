/* 
*  Kevin Meergans, SquadAI, 2014
*  RushBaseAttack.h
*  A team manoeuvre that orders the participating entities to assemble and perform an
*  all-out attack on the enemy flag trying to steal it by brute force/sheer number.
*/

#ifndef RUSH_BASE_ATTACK_H
#define RUSH_BASE_ATTACK_H

// Includes
#include <set>
#include "TeamManoeuvre.h"


// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;

//--------------------------------------------------------------------------------------
// Bundles additional data required to initialise a rush base attack manoeuvre.
//--------------------------------------------------------------------------------------
struct RushBaseAttackInitData
{
	RushBaseAttackInitData(float waitForParticipantsInterval) 
		: m_waitForParticipantsInterval(waitForParticipantsInterval)
	{}

	float m_waitForParticipantsInterval; // Determines after what time the participants will start the actual attack
};

class RushBaseAttack : public TeamManoeuvre
{
public:
	RushBaseAttack(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float waitForParticipantsInterval);
	~RushBaseAttack(void);

	// To be overwritten by derived manoeuvre classes

	BehaviourStatus Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access
	MultiflagCTFTeamAI* GetTeamAI(void);
	float GetWaitForParticipantsInterval(void) const;
	const XMFLOAT2& GetAssemblyPoint(void) const;

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);
	void SetWaitForParticipantsInterval(float interval);
	void SetAssemblyPoint(const XMFLOAT2& assemblyPoint);

protected:
	
	virtual void ProcessMessage(Message* pMessage);

private:

	//--------------------------------------------------------------------------------------
	// Available manoeuvre phases for this manoeuvre.
	//--------------------------------------------------------------------------------------
	enum ManoeuvrePhase
	{
		AssemblePhase,
		AttackPhase
	};

	void DetermineAssemblyPoint();
	void StartAttack(void);

	ManoeuvrePhase m_currentPhase;       // The phase the manoeuvre is currently in
	float m_waitForParticipantsInterval; // Determines after what time the participants will start the actual attack
	XMFLOAT2 m_assemblyPoint;            // The position, at which to assemble the participants before starting the actual attack
	float m_timer;					     // Keeps track of the time passed since the initiation of the manoeuvre
	MultiflagCTFTeamAI* m_pTeamAI;       // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)

	std::set<unsigned long> m_arrivedEntities; // Keeps track of the entities that have arrived at the assembly point

};

#endif // RUSH_BASE_ATTACK_H