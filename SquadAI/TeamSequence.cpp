/* 
*  Kevin Meergans, SquadAI, 2014
*  TeamSequence.cpp
*  Composite behaviour that represents a sequence of team behaviours, which are
*  executed in order as long as each of them succeeds. If one of the child behaviours
*  fails, the sequence fails as well. The sequence succeeds if all children are
*  executed successfully.
*/

// Includes
#include "TeamSequence.h"
#include "TeamAI.h"

TeamSequence::TeamSequence(const char* name, TeamAI* pTeamAI) : TeamComposite(name, pTeamAI)
{
}

TeamSequence::~TeamSequence(void)
{
}

//--------------------------------------------------------------------------------------
// Used to calculate the characteristic values for this behaviour and all children of it.
// Should be called for initialisation and when characteristics of certain behaviour 
// change or when behaviours are added respectively removed from the tree.
//--------------------------------------------------------------------------------------
void TeamSequence::CalculateCharacteristicValues(void)
{
	// Forward the call further down the tree.
	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->CalculateCharacteristicValues();
	}

	
	// Calculate the new values as the sum of the values of all children (as all are executed in sequence)
	float aggressiveness = 0;
	float defensiveness  = 0;

	for(std::vector<TeamBehaviour*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		aggressiveness += (*it)->GetAggressiveness();
		defensiveness  += (*it)->GetDefensiveness();
	}

	// The decorator has the same characteristic values as its single children.
	SetAggressiveness(aggressiveness);
	SetDefensiveness(defensiveness);
}

//--------------------------------------------------------------------------------------
// Initialises the sequence.
//--------------------------------------------------------------------------------------
void TeamSequence::OnInitialise(void)
{
	// Set the first child as the one to start the sequence with.
	m_currentChild = m_children.begin();
}

//--------------------------------------------------------------------------------------
// Updates the sequence.
// Param1: The time in seconds passed since the last frame.
// Returns the state of the sequence.
//--------------------------------------------------------------------------------------
BehaviourStatus TeamSequence::Update(float deltaTime)
{
	const char* name = GetName();

	if(m_children.size() < 1)
	{
		return StatusInvalid;
	}

	// Keep executing child behaviours until one returns a running
	// status. This is required as some children, such as conditions,
	// might complete within a single update.
	while(true)
	{
		BehaviourStatus status = (*m_currentChild)->Tick(deltaTime);

		// If the current child failed or is still running, this equals the 
		// current state of the whole sequence.
		if(status != StatusSuccess)
		{
			return status;
		}

		// Move on to the next child and check if the end of the sequence was
		// reached. In latter case, the sequence as a whole succeeded.
		if(++m_currentChild == m_children.end())
		{
			return StatusSuccess;
		}
	}

	// Shouldn't be reached, an error occurred.
	return StatusInvalid;
}