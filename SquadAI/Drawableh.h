/* 
*  Kevin Meergans, SquadAI, 2014
*  Drawable.h
*  Abstract base class for drawable objects.
*/

#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable
{
public:
	~Drawable(void);
	virtual bool Initialise(void) = 0;
	virtual bool InitialiseInstancing(void) = 0;
	virtual void Draw(void) = 0;
	virtual void DrawInstanced(void) = 0;
	virtual bool Cleanup(void) = 0;
private:
	m_pVertexBuffer( nullptr ), 
	m_pIndexBuffer( nullptr ), 
	instanceBuffer
	m_vertexCount( 0 ), 
	m_indexCount( 0 )
};

#endif // DRAWABLE_H