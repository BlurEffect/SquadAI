/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamComposite.h
*  Abstract base class for team composite behaviours.
*/

#ifndef TEAM_COMPOSITE_H
#define TEAM_COMPOSITE_H

// Includes
#include <vector>
#include <algorithm>
#include "TeamBehaviour.h"

// Forward declarations
class TeamAI;

class TeamComposite : public TeamBehaviour
{
public:
	TeamComposite(const char* name, TeamAI* pTeamAI);
	virtual ~TeamComposite(void);

	void AddChild(TeamBehaviour* pChild);
	void RemoveChild(TeamBehaviour* pChild);
	void ClearChildren(void);
	
protected:
	void ResetChildren(void);
	void AbortChildren(void);
	virtual void OnTerminate(BehaviourStatus status);

	std::vector<TeamBehaviour*> m_children; // The child behaviours encompassed by this composite behaviour

};

#endif // TEAM_COMPOSITE_H