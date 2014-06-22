/* 
*  Kevin Meergans, SquadAI, 2014
*  RenderContext.h
*  Accumulates and prepares the render data.
*/

// Includes
#include "RenderContext.h"

RenderContext::RenderContext(void)
{
}

RenderContext::~RenderContext(void)
{
}

//--------------------------------------------------------------------------------------
// Adds a new instance of a certain entity type to be rendered.
// Param1: The type of the entity to be rendered.
// Param2: The matrix to transform the entity instance to world space.
//--------------------------------------------------------------------------------------
void RenderContext::AddInstance(EntityType type, const XMFLOAT4X4& transform)
{
	m_drawableInstances[g_cDrawableMappings[type]].push_back(Instance(transform, g_cColourMappings[type]));
}

//--------------------------------------------------------------------------------------
// Deletes the current instance data in preparation for a new frame.
//--------------------------------------------------------------------------------------
void RenderContext::Reset(void)
{
	for(int i = 0; i < NumberOfDrawableTypes; ++i)
	{
		m_drawableInstances[i].clear();
	}
}

// Data access functions

int RenderContext::GetDrawableCount(DrawableType type) const
{
	return m_drawableInstances[type].size();
}

const Instance* RenderContext::GetInstances(DrawableType type) const
{
	return &m_drawableInstances[type][0];
}








#include "RenderContextImplementation.h"

RenderContextImplementation::RenderContextImplementation() : m_studInstanceCount( 0 )
{
	for( int i = 0; i < NumberOfBrickTypes; ++i )
	{
		m_instanceCounts[i] = 0;
	}
}

//--------------------------------------------------------------------------------------
// Initialise the render context.
//--------------------------------------------------------------------------------------
HRESULT RenderContextImplementation::Initialise( void )
{
	// make sure the instance counts are set to zero.
	Reset();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Cleanup the render context.
//--------------------------------------------------------------------------------------
HRESULT RenderContextImplementation::Cleanup( void )
{
	// more of a placeholder
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Add a new brick to the bricks to be rendered. Actually, only the associated instance 
// data of the brick will be stored in order to be forwarded to the renderer later on.
//--------------------------------------------------------------------------------------
bool RenderContextImplementation::AddBrick( const LegoBrick& brick )
{
	if( m_instanceCounts[brick.m_type] < NUMBER_OF_BRICKS_PER_TYPE )
	{
		// get the instance data of the brick
		m_instances[brick.m_type][m_instanceCounts[brick.m_type]].m_world  = brick.m_transform;
		m_instances[brick.m_type][m_instanceCounts[brick.m_type]].m_colour = BrickColours[brick.m_colour];

		// increase brick count
		++m_instanceCounts[brick.m_type];

		return true;
	}else
	{
		// maximal brick count reached, do not add (that is render) any more bricks
		return false;
	}
	
}

//--------------------------------------------------------------------------------------
// Add a new studs to the studs to be rendered. Actually, only the associated instance 
// data of the stud will be stored in order to be forwarded to the renderer later on.
//--------------------------------------------------------------------------------------
bool RenderContextImplementation::AddStud( const LegoStud& stud )
{
	if( m_studInstanceCount < NUMBER_OF_STUDS )
	{
		// get the instance data of the brick
		m_studInstances[m_studInstanceCount].m_world  = stud.m_transform;
		m_studInstances[m_studInstanceCount].m_colour = BrickColours[stud.m_colour];

		// increase brick count
		++m_studInstanceCount;

		return true;
	}else
	{
		// maximal brick count reached, do not add (that is render) any more bricks
		return false;
	}
}


//--------------------------------------------------------------------------------------
// Reset the render context as preparation for the next frame.
//--------------------------------------------------------------------------------------
void RenderContextImplementation::Reset( void )
{
	// Reset the instance counts. No need to reset instances as these will simply be overwritten and only these
	// new instances are ever passed on to the actual rendering.
	ZeroMemory( m_instanceCounts, sizeof(int) * NumberOfBrickTypes );
	m_studInstanceCount = 0;
}

//--------------------------------------------------------------------------------------
// Given a brick type return the number of instances to render.
//--------------------------------------------------------------------------------------
int  RenderContextImplementation::GetInstanceCount( LegoBrickType type ) const
{
	return m_instanceCounts[type];
}

//--------------------------------------------------------------------------------------
// Given a brick type return a pointer to the instances of that brick type to be rendered.
//--------------------------------------------------------------------------------------
const Instance* const RenderContextImplementation::GetInstances( LegoBrickType type ) const
{
	return m_instances[type];
}

//--------------------------------------------------------------------------------------
// Return the number of stud instances to render.
//--------------------------------------------------------------------------------------
int RenderContextImplementation::GetStudInstanceCount( void ) const
{
	return m_studInstanceCount;
}

//--------------------------------------------------------------------------------------
// Returns a pointer to the instances of studs to be rendered.
//--------------------------------------------------------------------------------------
const Instance* const RenderContextImplementation::GetStudInstances( void ) const
{
	return m_studInstances;
}