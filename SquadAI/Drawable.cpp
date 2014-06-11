/* 
*  Kevin Meergans, SquadAI, 2014
*  Drawable.cpp
*  Abstract base class for drawable objects providing some default implementations.
*/

// Includes
#include "Drawable.h"

//--------------------------------------------------------------------------------------
// Default constructor.
//--------------------------------------------------------------------------------------
Drawable::Drawable(void) : m_vertexCount(0),
						   m_indexCount(0),
						   m_instancingSetUp(false),
						   m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
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
// Param2: Enum value indicating the desired usage for the instance buffer that will be created by this function
// Param3: A pointer to the instance data.
// Param4: The maximal number of instances for this Drawable.
// Returns true if instancing was initialised successfully, false otherwise.
//--------------------------------------------------------------------------------------
bool Drawable::InitialiseInstancing(ID3D11Device* pDevice, D3D11_USAGE usage, Instance* pInstances, UINT maxNumberOfInstances)
{
	// Check if the maximal number of instances is valid
	if(maxNumberOfInstances == 0)
	{
		return false;
	}

	m_maxNumberOfInstances = maxNumberOfInstances;

	if(m_instanceBuffer.Initialise(InstanceBuffer, usage, pInstances, maxNumberOfInstances))
	{
		m_instancingSetUp = true;
	}

	return m_instancingSetUp;
}

//--------------------------------------------------------------------------------------
// Renders the Drawable.
// Param1: The device context to use for rendering.
//--------------------------------------------------------------------------------------
void Drawable::Draw(ID3D11DeviceContext* pDeviceContext)
{
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext -> IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer(), m_vertexBuffer.GetStride(), m_vertexBuffer.GetOffset());

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext -> IASetIndexBuffer(*m_indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer.
	pDeviceContext -> IASetPrimitiveTopology(m_primitiveTopology);

	pDeviceContext -> DrawIndexed(m_indexCount, 0, 0);
}

//--------------------------------------------------------------------------------------
// Uses hardware instancing to draw a number of instances of this Drawable.
// Param1: The device context to use for rendering.
// Param2: A pointer to the instance data. Setting this to NULL will lead to the old instance data being used
// Param3: The number of instances to be rendered.
// Param4: The offset into the instance buffer, from which 
// Returns true if the drawcall was successful, false if instancing was not set up for this Drawable of if the number of
// instances set to be drawn is greater than the actual instance buffer
//--------------------------------------------------------------------------------------
bool Drawable::DrawInstanced(ID3D11DeviceContext* pDeviceContext, Instance* pInstances, UINT instanceCount, UINT offset)
{
	if((offset + instanceCount > m_maxNumberOfInstances) || !m_instancingSetUp)
	{
		// Invalid number of instances or instancing was not set up properly
		return false;
	}

	// If new instance data was sent, update the instance buffer, otherwise use the old instance data
	if(pInstances)
	{
		m_instanceBuffer.Update(pDeviceContext, pInstances, instanceCount, offset);
	}

	UINT strides[2];
	UINT offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set the buffer strides.
	strides[0] = *m_vertexBuffer.GetStride(); 
	strides[1] = *m_instanceBuffer.GetStride(); 

	// Set the buffer offsets.
	offsets[0] = *m_vertexBuffer.GetOffset();
	offsets[1] = *m_instanceBuffer.GetOffset();

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = *m_vertexBuffer.GetBuffer();	
	bufferPointers[1] = *m_instanceBuffer.GetBuffer();

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext -> IASetIndexBuffer(*m_indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	pDeviceContext -> IASetPrimitiveTopology(m_primitiveTopology);
	pDeviceContext -> DrawIndexedInstanced(m_indexCount, instanceCount, 0, 0, 0);

	return true;
}

//--------------------------------------------------------------------------------------
// Releases resources.
//--------------------------------------------------------------------------------------
void Drawable::Cleanup(void)
{
	// Release all the buffers associated to this Drawable.

	m_vertexBuffer.Cleanup();
	m_indexBuffer.Cleanup();
	m_instanceBuffer.Cleanup();
}