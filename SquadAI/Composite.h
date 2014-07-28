/* 
*  Kevin Meergans, SquadAI, 2014
*  Composite.h
*  Abstract base class for composite behaviours.
*  Based on Alex Champandard's presentation "Understanding the Second-Generation of Behavior Trees"
*/

#ifndef COMPOSITE_H
#define COMPOSITE_H

// Includes
#include <vector>
#include "Behaviour.h"

class Composite : public Behaviour
{
public:
	Composite(void);
	virtual ~Composite(void);

protected:
	std::vector<Behaviour*> m_children; // The child behaviours encompassed by this composite behaviour

};

#endif // COMPOSITE_H