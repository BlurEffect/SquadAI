/* 
*  Kevin Meergans, SquadAI, 2014
*  DefendBaseEntrances.h
*  A team manoeuvre that orders the participating entities to move to the entrances to
*  their home base and defend it against any intruders.
*/

#ifndef DEFEND_BASE_ENTRANCES_H
#define DEFEND_BASE_ENTRANCES_H

// Includes
#include <unordered_map>
#include "TeamManoeuvre.h"
#include "TestEnvironmentData.h"


// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;

//--------------------------------------------------------------------------------------
// Bundles additional data required to initialise a defend base entrances manoeuvre.
//--------------------------------------------------------------------------------------
struct DefendBaseEntrancesInitData
{
	DefendBaseEntrancesInitData(float switchPositionsInterval) : m_switchPositionsInterval(switchPositionsInterval) {}

	float m_switchPositionsInterval;
};

//--------------------------------------------------------------------------------------
// The data associated to an entity guarding an entrance.
//--------------------------------------------------------------------------------------
struct GuardData
{
	GuardData(Direction entranceDirection, const XMFLOAT2& entrancePosition) : m_entranceDirection(entranceDirection),
																			   m_entrancePosition(entrancePosition)
	{}

	Direction m_entranceDirection;
	XMFLOAT2  m_entrancePosition;
};

class DefendBaseEntrances : public TeamManoeuvre
{
public:
	DefendBaseEntrances(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float switchPositionsInterval);
	~DefendBaseEntrances(void);

	// To be overwritten by derived manoeuvre classes

	void			Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access
	const MultiflagCTFTeamAI* GetTeamAI(void) const;
	float GetSwitchPositionsInterval(void) const;

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);
	void SetSwitchPositionsInterval(float interval);

protected:
	
	virtual bool ProcessMessage(Message* pMessage);

private:

	void DistributeEntities(void);
	bool IsGuarded(Direction direction, const XMFLOAT2& entrance) const;

	float m_switchPositionsInterval; // Determines after what time the participants will move to new defend positions (set to 0.0f to prevent any switches)
	float m_timer;					 // Keeps track of the time passed since the last position switch
	MultiflagCTFTeamAI* m_pTeamAI;   // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)

	std::unordered_map<unsigned long, GuardData> m_guardedEntrances; // The entrances currently being guarded by participants of the manoeuvre

};

#endif // DEFEND_BASE_ENTRANCES_H