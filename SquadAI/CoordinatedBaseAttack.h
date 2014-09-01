/* 
*  Kevin Meergans, SquadAI, 2014
*  CoordinatedBaseAttack.h
*  A team manoeuvre that splits the participating entities into equally strong groups
*  that will attack the enemy base from different directions.
*/

#ifndef COORDINATED_BASE_ATTACK_H
#define COORDINATED_BASE_ATTACK_H

// Includes
#include <set>
#include "TeamManoeuvre.h"


// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;

//--------------------------------------------------------------------------------------
// Bundles additional data required to initialise a distraction attack manoeuvre.
//--------------------------------------------------------------------------------------
struct CoordinatedBaseAttackInitData
{
	CoordinatedBaseAttackInitData(unsigned int numberOfAttackGroups, float waitForParticipantsInterval) 
		: m_numberOfAttackGroups(numberOfAttackGroups),
		  m_waitForParticipantsInterval(waitForParticipantsInterval)
	{}

	unsigned int m_numberOfAttackGroups;        // The number of groups, into which the participants should be split up for the distributed attack
	float		 m_waitForParticipantsInterval; // Determines after what time the participants will start the actual attack
};

class CoordinatedBaseAttack : public TeamManoeuvre
{
public:
	CoordinatedBaseAttack(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, unsigned int numberOfAttackGroups, float waitForParticipantsInterval);
	~CoordinatedBaseAttack(void);

	// To be overwritten by derived manoeuvre classes

	BehaviourStatus Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access
	MultiflagCTFTeamAI* GetTeamAI(void);
	float GetWaitForParticipantsInterval(void) const;
	const XMFLOAT2& GetAssemblyPoint(unsigned int group) const;
	unsigned int GetNumberOfGroups(void) const;

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);
	void SetWaitForParticipantsInterval(float interval);
	void SetAssemblyPoint(unsigned int group, const XMFLOAT2& assemblyPoint);

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

	void SetupGroups(void);
	void DetermineAssemblyPoints(void);
	void StartAttack(void);

	ManoeuvrePhase m_currentPhase;       // The phase the manoeuvre is currently in
	float m_waitForParticipantsInterval; // Determines after what time the participants will start the actual attack
	float m_timer;					     // Keeps track of the time passed since the initiation of the manoeuvre
	MultiflagCTFTeamAI* m_pTeamAI;       // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)

	std::vector<XMFLOAT2> m_assemblyPoints; // The assembly points for the different groups
	unsigned int m_numberOfGroups;     // The number of groups taking part in the coordinated attack

	std::set<unsigned long> m_arrivedEntities; // Keeps track of the entities that have arrived at their assembly point

	std::unordered_map<unsigned long, unsigned int> m_entityGroupMap; // Keeps track of which entity belongs to which attack group
};

#endif // DISTRACTION_BASE_ATTACK_H