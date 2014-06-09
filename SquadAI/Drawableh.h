/* 
*  Kevin Meergans, SquadAI, 2014
*  Drawable.h
*  Abstract base class for drawable objects providing some default implementations.
*/

#ifndef DRAWABLE_H
#define DRAWABLE_H

// Includes
#include <d3d11.h>
#include "VertexData.h"

class Drawable
{
public:
	Drawable(void);
	virtual ~Drawable(void);
	virtual bool Initialise(ID3D11Device* pDevice) = 0;
	virtual bool InitialiseInstancing(ID3D11Device* pDevice, int maxNumberOfInstances, bool isDynamic);
	virtual bool Draw(void) = 0;
	virtual bool DrawInstanced(void) = 0;
	virtual bool Cleanup(void) = 0;
private:
	ID3D11Buffer*	m_pVertexBuffer;	// Holds the vertices for this drawable object
	ID3D11Buffer*	m_pIndexBuffer;		// Holds the indices for this drawable object
	ID3D11Buffer*	m_pInstanceBuffer;	// Holds the instance data for this drawable object (only used for instanced drawing)
	int				m_vertexCount;		// The number of vertices making up this drawable
	int				m_indexCount;		// The number of indices for this drawable
	int				m_maxNumberOfInstances;	// The maximal number of instances for this Drawable
	bool			m_usesDynamicInstanceBuffer; // Is the instance buffer used dynamic or static
};

#endif // DRAWABLE_H