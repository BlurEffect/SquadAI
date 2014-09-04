/* 
*  Kevin Meergans, SquadAI, 2014
*  InterceptFlagCarrier.h
*  A team manoeuvre that orders the participating entities prevent the enemy flag 
*  carrier from reaching his destination and capturing the flag of the team.
*/

#ifndef INTERCEPT_FLAG_CARRIER_H
#define INTERCEPT_FLAG_CARRIER_H

// Includes
#include "TeamManoeuvre.h"

// Forward declarations
class MultiflagCTFTeamAI;
enum BehaviourStatus;

//--------------------------------------------------------------------------------------
// Bundles additional data required to initialise a intercept flag carrier manoeuvre.
//--------------------------------------------------------------------------------------
struct InterceptFlagCarrierInitData
{
	InterceptFlagCarrierInitData(float searchRadius, float updateCarrierPositionInterval) 
		: m_searchRadius(searchRadius),
		  m_updateCarrierPositionInterval(updateCarrierPositionInterval)
	{}

	float m_searchRadius;				   // Determines how close the protectors stay to the flag carrier
	float m_updateCarrierPositionInterval; // Determines how often the position of the enemy flag carrier is updated with the current
										   // value held by the associated team AI
};

class InterceptFlagCarrier : public TeamManoeuvre
{
public:
	InterceptFlagCarrier(unsigned int minNumberParticipants, unsigned int maxNumberParticipants, MultiflagCTFTeamAI* pTeamAI, float searchRadius, float updateCarrierPositionInterval);
	~InterceptFlagCarrier(void);

	// To be overwritten by derived manoeuvre classes

	BehaviourStatus Initiate(void);
	BehaviourStatus Update(float deltaTime);
	void			Terminate(void);

	void Reset(void);

	// Data access
	const MultiflagCTFTeamAI* GetTeamAI(void) const;
	float GetSearchRadius(void) const;
	float GetUpdateCarrierPositionInterval(void) const;

	void SetTeamAI(MultiflagCTFTeamAI* pTeamAI);
	void SetSearchRadius(float radius);
	void SetUpdateCarrierPositionInterval(float interval);

protected:
	
	virtual void ProcessMessage(Message* pMessage);

private:

	//--------------------------------------------------------------------------------------
	// Available manoeuvre phases for this manoeuvre.
	//--------------------------------------------------------------------------------------
	enum ManoeuvrePhase
	{
		HuntPhase,		// The position of the enemy flag carrier is known (or believed to be known) and entities are sent out to attack
		SearchPhase     // The position of the enemy flag carrier is unknown, entities are sent to the enemy base to intercept him anyway
	};

	bool SendOutAttackOrders(const XMFLOAT2& attackPosition);
	void UpdateAttackOrders(const XMFLOAT2& attackPosition, OrderPriority priority);

	ManoeuvrePhase      m_currentPhase;       // The current phase of the manoeuvre
	MultiflagCTFTeamAI* m_pTeamAI;			  // The team AI able to use this manoeuvre (in this case a specific Multiflag Team AI is required)
	float m_timer;					     // Keeps track of the time passed
	unsigned long		m_enemyFlagCarrierId; // The id of the enemy flag carrier

	float m_searchRadius;				   // Determines how close the protectors stay to the flag carrier
	float m_updateCarrierPositionInterval; // Determines how often the position of the enemy flag carrier is updated with the current
										   // value held by the associated team AI
};

#endif // INTERCEPT_FLAG_CARRIER_H