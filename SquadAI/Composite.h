/* 
*  Kevin Meergans, SquadAI, 2014
*  Composite.h
*  Abstract base class for composite behaviours.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*  and the associated "Behavior Tree Starter Kit" (BTSK).
*/

#ifndef COMPOSITE_H
#define COMPOSITE_H

// Includes
#include <vector>
#include <algorithm>
#include "Behaviour.h"

class Composite : public Behaviour
{
public:
	Composite(const char* name);
	virtual ~Composite(void);

	void AddChild(Behaviour* pChild);
	void RemoveChild(Behaviour* pChild);
	void ClearChildren(void);

protected:
	std::vector<Behaviour*> m_children; // The child behaviours encompassed by this composite behaviour

};

#endif // COMPOSITE_H