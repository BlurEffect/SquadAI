/* 
*  Kevin Meergans, SquadAI, 2014
*  Behaviour.h
*  Abstract base class for entity behaviours.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*/

#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

//--------------------------------------------------------------------------------------
// Possible states for behaviours. Used as return codes.
//--------------------------------------------------------------------------------------
enum BehaviourStatus
{
	StatusInvalid, // Indicates that an error occurred or that the initialisation failed
	StatusSuccess, // Indicates that the behaviour terminated successfully
	StatusFailure, // Indiactes that the execution of the behaviour failed
	StatusRunning  // Indicates that the behaviour is still being executed
};

class Behaviour
{
public:
	Behaviour(void);
	virtual ~Behaviour(void);

	BehaviourStatus Tick(void);

protected:
	virtual BehaviourStatus Update() = 0;

	virtual void OnInitialise(void);
	virtual void OnTerminate(BehaviourStatus status);

private:
	BehaviourStatus m_status; // The current state of the behaviour
};

#endif // BEHAVIOUR_H