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
	bool Initialise(BufferType bufferType, ID3D11Device* pDevice, D3D11_USAGE bufferUsage, BufferElementType* pElements, UINT maxNumberOfElements);
	bool Update(ID3D11DeviceContext* pDeviceContext, BufferElementType* pElements, UINT numberOfElements, UINT offset);
	void Cleanup(void);
	
	// Data access
	inline ID3D11Buffer* const* GetBuffer() const;
	inline BufferType GetType(void) const;
	inline const UINT* GetStride(void) const;
	inline const UINT* GetOffset(void) const;

private:
	BufferType    m_bufferType;			 // The type of this buffer
	ID3D11Buffer* m_pBuffer;			 // The underlying buffer of this object
	UINT		  m_stride;				 // The stride for elements in the buffer
	UINT		  m_offset;				 // The offset used
	UINT          m_maxNumberOfElements; // The maximum amount of elements the buffer can hold
};

// Implementations of the template class

template <class BufferElementType>
Buffer<BufferElementType>::Buffer() : m_pBuffer(nullptr),
								      m_stride(sizeof(BufferElementType)),
								      m_offset(0)		 
{
}

template <class BufferElementType>
Buffer<BufferElementType>::~Buffer() 	 
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Prepares the Drawable for hardware instancing by setting up an instance buffer.
// Param1: An enum value indicating the type of buffer to be created.
// Param2: A pointer to the device that should be used to create the buffer.
// Param2: An enum value specifying the desired usage for the buffer to be created
// Param3: A pointer to the initial data for the buffer. Can be NULL if no initial data should be written to the buffer.
// Param4: The maximal number of elements that the buffer should be able to hold. Determines the size of the buffer.
// Returns true if the buffer was set up and initialised correctly, false if an error occurred.
//--------------------------------------------------------------------------------------
template <class BufferElementType>
bool Buffer<BufferElementType>::Initialise(BufferType bufferType, ID3D11Device* pDevice, D3D11_USAGE bufferUsage, BufferElementType* pElements, UINT maxNumberOfElements)
{
	if(maxNumberOfElements == 0)
	{
		return false;
	}

	m_maxNumberOfElements = maxNumberOfElements;

	if(m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = nullptr;
	}
		
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = bufferUsage;
	bufferDesc.ByteWidth = sizeof(BufferElementType) * maxNumberOfElements;

	switch(bufferType)
	{
	case VertexBuffer:
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
	case IndexBuffer:
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		break;
	case InstanceBuffer:
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
	case ConstantBuffer:
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		break;
	}

	m_bufferType = bufferType;

	if(bufferUsage == D3D11_USAGE_DYNAMIC)
	{
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}else
	{
		bufferDesc.CPUAccessFlags = 0;
	}

	bufferDesc.MiscFlags = 0;

	if(pElements)
	{
		// Write initial data to the buffer
		D3D11_SUBRESOURCE_DATA bufferData; 
		ZeroMemory(&bufferData, sizeof(bufferData));

		// Give the subresource structure a pointer to the vertex data.
		bufferData.pSysMem = pElements;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		return SUCCEEDED(pDevice->CreateBuffer(&bufferDesc, &bufferData, &m_pBuffer));
	}

	return SUCCEEDED(pDevice->CreateBuffer(&bufferDesc, NULL, &m_pBuffer));
}

//--------------------------------------------------------------------------------------
// Updates the underlying buffer by overwriting its contents.
// Param1: A pointer to the device context that should be used for updating.
// Param2: A pointer to the data that should be written to the buffer.
// Param3: The number of elements that should be written to the buffer.
// Param4: The offset into the current buffer, from which overwriting with the new data should be started.
// Returns true if the buffer was updated successfully, false otherwise.
//--------------------------------------------------------------------------------------
template <class BufferElementType>
bool Buffer<BufferElementType>::Update(ID3D11DeviceContext* pDeviceContext, BufferElementType* pElements, UINT numberOfElements, UINT offset)
{
	// Attempt to write more data to the buffer than it can hold
	if(numberOfElements > m_maxNumberOfElements)
	{
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE resource;

	if(FAILED(pDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource)))
	{
		return false;
	}

	memcpy(resource.pData, &m_pBuffer[offset], sizeof(BufferElementType) * numberOfElements );
	pDeviceContext->Unmap(m_pBuffer, 0);

	return true;
}

//--------------------------------------------------------------------------------------
// Releases resources allocated by the buffer.
//--------------------------------------------------------------------------------------
template <class BufferElementType>
void Buffer<BufferElementType>::Cleanup()
{
	if(m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = nullptr;
	}
}

// Data access

template <class BufferElementType>
BufferType Buffer<BufferElementType>::GetType(void) const
{
	return m_bufferType;
}

template <class BufferElementType>
ID3D11Buffer* const* Buffer<BufferElementType>::GetBuffer() const
{
	return &m_pBuffer;
}

template <class BufferElementType>
const UINT* Buffer<BufferElementType>::GetStride() const
{
	return &m_stride;
}

template <class BufferElementType>
const UINT* Buffer<BufferElementType>::GetOffset() const
{
	return &m_offset;
}

#endif // BUFFER_H