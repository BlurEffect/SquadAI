/* 
*  Kevin Meergans, SquadAI, 2014
*  TestEnvironment.h
*  Encapsulates a test setup and contains all entities
*  that are part of it.
*/

#ifndef TEST_ENVIRONMENT_H
#define TEST_ENVIRONMENT_H

// Includes
#include "RenderContext.h"
#include <DirectXMath.h>

using namespace DirectX;

class TestEnvironment
{
public:
	// Setup, load level data, init structure with number of soldiers start position etc
	bool Initialise(void);
	void Update(RenderContext& pRenderContext);
	void Cleanup(void);
private:

};

#endif // TEST_ENVIRONMENT_H
