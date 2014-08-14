/* 
*  Kevin Meergans, SquadAI, 2014
*  Behaviour.h
*  Abstract base class for all behaviours.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
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
	StatusRunning, // Indicates that the behaviour is still being executed
	StatusAborted  // The behaviour was aborted
};


class Behaviour
{
public:
	Behaviour(const char* name);
	Behaviour(const Behaviour& sourceBehaviour);
	virtual ~Behaviour(void);

	Behaviour& Behaviour::operator= (const Behaviour& sourceBehaviour);

	BehaviourStatus Tick(float deltaTime);

	void Abort(void);
	void Reset(void);

	bool IsTerminated(void) const;
	bool IsRunning(void) const;

	unsigned long   GetId(void) const;
	const char*     GetName(void) const;
	BehaviourStatus GetStatus(void) const;
	
	//--------------------------------------------------------------------------------------
	// A functor that can be used to find a behaviour based on its unique id.
	//--------------------------------------------------------------------------------------
	class FindBehaviourById
	{
	public:

		FindBehaviourById(unsigned long id) : m_id(id){}
		bool operator()(Behaviour* pBehaviour) const
		{
			return m_id == pBehaviour->GetId();
		}
	private:
		unsigned long m_id; // The id that is being looked for
	};

protected:
	virtual BehaviourStatus Update(float deltaTime) = 0;

	virtual void OnInitialise(void);
	virtual void OnTerminate(BehaviourStatus status);

private:

	static unsigned long s_BehaviourId; // This id is incremented with each created object and assigned to the new behaviour, 0 is an invalid value

	unsigned long   m_id;      // Each behaviour is assigned a unique id
	const char*     m_name;    // The name of this behaviour
	BehaviourStatus m_status;  // The current state of the behaviour

};

#endif // BEHAVIOUR_H