/* 
*  Kevin Meergans, SquadAI, 2014
*  Drawable.cpp
*  Abstract base class for drawable objects providing some default implementations.
*/

// Includes
#include "Drawableh.h"

//--------------------------------------------------------------------------------------
// Default constructor.
//--------------------------------------------------------------------------------------
Drawable::Drawable(void) : m_pVertexBuffer(nullptr),
						   m_pIndexBuffer(nullptr),
		                   m_pInstanceBuffer(nullptr),
						   m_vertexCount(0),
						   m_indexCount(0)
{

}

//--------------------------------------------------------------------------------------
// Destructor.
//--------------------------------------------------------------------------------------
Drawable::~Drawable(void)
{
	Cleanup();
}

//--------------------------------------------------------------------------------------
// Pure virtual initialisation function to be implemented by derived classes in order to
// set up specific meshes.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Returns true if the initialisation succeeded, false otherwise.
//--------------------------------------------------------------------------------------
//bool Initialise(ID3D11Device* pDevice);

//--------------------------------------------------------------------------------------
// Prepares the Drawable for hardware instancing by setting up an instance buffer.
// Param1: A pointer to the device that should be used for the creation of this Drawable.
// Param2: The maximal number of instances for this mesh, set zero or negative if no instancing needed.
// Param3: Determines whether a static or dynamic instance buffer will be created.
// Returns true if instancing was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Drawable::InitialiseInstancing(ID3D11Device* pDevice, int maxNumberOfInstances, bool isDynamic, const Instance* const pInstances, int instanceCount)
{
	// Check if the maximal number of instances is valid
	if(maxNumberOfInstances < 1)
	{
		return false;
	}

	m_maxNumberOfInstances = maxNumberOfInstances;
	m_usesDynamicInstanceBuffer = isDynamic;

	// Create the instance buffer
	D3D11_BUFFER_DESC instanceBufferDesc;	
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));

	if(isDynamic)
	{
		instanceBufferDesc.Usage		  = D3D11_USAGE_DYNAMIC;
	}else
	{
		instanceBufferDesc.Usage		  = D3D11_USAGE_DEFAULT;		
	}

	instanceBufferDesc.ByteWidth      = sizeof(Instance) * m_maxNumberOfInstances;
	instanceBufferDesc.BindFlags	  = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags	  = 0;

	D3D11_SUBRESOURCE_DATA		instanceData;
	ZeroMemory(&instanceData, sizeof(instanceData));
	
	if(pInstances != nullptr)
	{
		instanceData.pSysMem = pInstances;
		instanceData.SysMemPitch = 0;
		instanceData.SysMemSlicePitch = 0;

		return SUCCEEDED(pDevice -> CreateBuffer( &instanceBufferDesc, &instanceData, &m_pInstanceBuffer));
	}

	return SUCCEEDED(pDevice -> CreateBuffer( &instanceBufferDesc, nullptr, &m_pInstanceBuffer));
}

//--------------------------------------------------------------------------------------
// The structure of a vertex.
//--------------------------------------------------------------------------------------
bool Drawable::Draw(void)
{

}

//--------------------------------------------------------------------------------------
// The structure of a vertex.
//--------------------------------------------------------------------------------------
bool Drawable::DrawInstanced(void)
{
	
	D3D11_MAPPED_SUBRESOURCE resource;
    pDeviceContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, &pInstances[0], sizeof(Instance) * instanceCount ); //NUMBER_OF_BRICKS_PER_TYPE);
   
	// check for contents
	//Instance* dataView = reinterpret_cast<Instance*>(resource.pData);

	pDeviceContext->Unmap(m_pInstanceBuffer, 0);
	



	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set the buffer strides.
	strides[0] = sizeof(Vertex); 
	strides[1] = sizeof(Instance); 

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_pVertexBuffer;	
	bufferPointers[1] = m_pInstanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	//pDeviceContext -> IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext -> IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext -> IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pDeviceContext -> DrawIndexedInstanced( m_indexCount, instanceCount, 0, 0, 0);
}

//--------------------------------------------------------------------------------------
// Releases resources.
// Returns true if clean up was successful, false otherwise.
//--------------------------------------------------------------------------------------
bool Drawable::Cleanup(void)
{
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if(m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	if(m_pInstanceBuffer)
	{
		m_pInstanceBuffer->Release();
		m_pInstanceBuffer = nullptr;
	}
}