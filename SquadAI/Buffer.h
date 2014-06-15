/* 
*  Kevin Meergans, SquadAI, 2014
*  Buffer.h
*  The Buffer class wraps a buffer and provides functions for initialisation, update and
*  release of the associated data. The class is templated and can be used
*  with any kind of element type. At the moment it can be used for vertex,
*  index and instance buffers.
*/

#ifndef BUFFER_H
#define BUFFER_H

// Includes
#include <d3d11.h>

//--------------------------------------------------------------------------------------
// Indicates the type of a buffer. Maps to D3D11_USAGE.
//--------------------------------------------------------------------------------------
enum BufferType
{
	VertexBuffer,
	IndexBuffer,
	InstanceBuffer,
	ConstantBuffer
};

template <class BufferElementType>
class Buffer
{
public:
	Buffer(void);	 
	~Buffer(void);
	bool Initialise(BufferType bufferType, D3D11_USAGE bufferUsage, BufferElementType* pElements, UINT maxNumberOfElements);
	bool Update(ID3D11DeviceContext* pDeviceContext, BufferElementType* pElements, UINT numberOfElements, UINT offset);
	void Cleanup(void);
	
	// Data access
	inline ID3D11Buffer* const* GetBuffer() const;
	inline BufferType GetType(void) const;
	inline const UINT* GetStride(void) const;
	inline const UINT* GetOffset(void) const;

private:
	BufferType    m_bufferType; // The type of this buffer
	ID3D11Buffer* m_pBuffer;	// The underlying buffer of this object
	UINT		  m_stride;		// The stride for elements in the buffer
	UINT		  m_offset;		// The offset used
};

// Include the file containing the implementations for the functions of the templated
// Buffer class.
#include "Buffer.cpp"

#endif // BUFFER_H