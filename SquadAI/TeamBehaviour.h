/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamBehaviour.h
*  Abstract base class for all behaviours that can be used by team AIs.
*/

#ifndef TEAM_BEHAVIOUR_H
#define TEAM_BEHAVIOUR_H

// Includes
#include "Behaviour.h"

// Forward declarations
#include "TeamAI.h"

class TeamBehaviour : public Behaviour
{
public:
	TeamBehaviour(const char* name, TeamAI* pTeamAI, float aggressiveness = 0.0f, float defensiveness = 0.0f);
	virtual ~TeamBehaviour(void);
	
	virtual void CalculateCharacteristicValues(void);

	// Data access functions

	TeamAI*		 GetTeamAI(void);
	float GetAggressiveness(void) const;
	float GetDefensiveness(void) const;

	void SetAggressiveness(float aggressiveness);
	void SetDefensiveness(float defensiveness);

private:
	TeamAI*		 m_pTeamAI;		   // The team AI that this behaviour belongs to
	float m_aggressiveness; // The aggressiveness value associated to this behaviour
	float m_defensiveness;  // The defensiveness value associated to this behaviour
};


//--------------------------------------------------------------------------------------
// Functor used to find an entity within a container based on its id.
//--------------------------------------------------------------------------------------
class SortTeamBehavioursByCharacteristic
{
public:
	SortTeamBehavioursByCharacteristic(TeamAICharacteristic characteristic) : m_characteristic(characteristic){}
	bool operator()(const TeamBehaviour* pLeft, const TeamBehaviour* pRight)
	{
		switch(m_characteristic)
		{
		case CharNone:
			return (rand() % 2 == 0); // random order
			break;
		case CharAggressive:
			return pLeft->GetAggressiveness() < pRight->GetAggressiveness();
			break;
		case CharDefensive:
			return pLeft->GetDefensiveness() < pRight->GetDefensiveness();
			break;
		default:
			return false;
		}
	}
private:
	TeamAICharacteristic m_characteristic; // The characteristic to sort the behaviours by
};


#endif // TEAM_BEHAVIOUR_H