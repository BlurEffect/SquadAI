/* 
*  Kevin Meergans, Improved Lego Editor, 2014
*  SentenceImplementation.cpp
*  Contains the function definitions for the SentenceImplementation class.
*/


#include "SentenceImplementation.h"

SentenceImplementation::SentenceImplementation() : m_pVertexBuffer( nullptr ), 
												   m_pIndexBuffer( nullptr ), 
												   m_vertexCount( 0 ), 
												   m_indexCount( 0 ), 
												   m_maxLength( 0 ),
												   m_colourRGB( 0, 0, 0 )
{
}

//--------------------------------------------------------------------------------------
// Initialise the sentence.
//--------------------------------------------------------------------------------------
HRESULT SentenceImplementation::Initialise( ID3D11Device* pDevice, int maxLength )
{
	HRESULT result;

	m_maxLength     = maxLength;
	m_vertexCount   = 6 * maxLength;
	m_indexCount    = m_vertexCount;

	// Create the vertex array
	TextVertex* pVertices = new TextVertex[m_vertexCount];
	if( !pVertices )
	{
		return E_FAIL;
	}

	// Create the index array
	unsigned long* pIndices = new unsigned long[m_indexCount];
	if( !pIndices )
	{
		return E_FAIL;
	}

	// Initialize vertex array to zeros
	memset(pVertices, 0, ( sizeof( TextVertex ) * ( m_vertexCount ) ) );

	// Initialize the index array
	for( int i = 0; i < m_indexCount; ++i)
	{
		pIndices[i] = i;
	}

	// Create vertex buffer for the sentence (contents might change)

	// todo: allow for static and dynamic sentences depending on a flag
	// todo: put all static sentences into one vertex buffer

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof( TextVertex ) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = pDevice -> CreateBuffer( &vertexBufferDesc, &vertexData, &m_pVertexBuffer );
	if( FAILED( result ) )
	{
		return result;
	}

	// Create index buffer for the sentence (won't change)

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = pDevice -> CreateBuffer( &indexBufferDesc, &indexData, &m_pIndexBuffer );
	if( FAILED( result ) )
	{
		return result;
	}

	// Release the vertex array as it is no longer needed.
	delete [] pVertices;
	pVertices = nullptr;

	// Release the index array as it is no longer needed.
	delete [] pIndices;
	pIndices = nullptr;

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Release allocated resources.
//--------------------------------------------------------------------------------------
HRESULT SentenceImplementation::Cleanup( void )
{
	if( m_pVertexBuffer )
	{
		m_pVertexBuffer -> Release();
		m_pVertexBuffer = nullptr;
	}
	if( m_pIndexBuffer )
	{
		m_pIndexBuffer -> Release();
		m_pIndexBuffer = nullptr;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Update the content and look of the sentence.
//--------------------------------------------------------------------------------------
HRESULT SentenceImplementation::Update( ID3D11DeviceContext* pDeviceContext, Font* pFont, const SentenceData& sentenceData )
{
	HRESULT result;
	
	// Set the sentence colour
	m_colourRGB = sentenceData.m_colourRGB;

	// Make sure the given text fits into the sentence
	if( ( int )strlen( sentenceData.m_text ) > m_maxLength )
	{
		return false;
	}

	// Create and initialise the vertex array

	TextVertex* pVertices = new TextVertex[m_vertexCount];
	if( !pVertices )
	{
		return false;
	}
	memset( pVertices, 0, ( sizeof( TextVertex ) * m_vertexCount ) );

	// Build the vertex array for the given text
	pFont -> BuildVertexArray( ( void* )pVertices, sentenceData.m_text, static_cast<float>( sentenceData.m_drawX ), static_cast<float>( sentenceData.m_drawY ) );

	// Copy the vertex array into the sentence vertex buffer

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	TextVertex* pSentenceVertices;
	result = pDeviceContext->Map( m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if( FAILED( result) )
	{
		return false;
	}
	pSentenceVertices = ( TextVertex* )mappedResource.pData;
	memcpy( pSentenceVertices, ( void* )pVertices, ( sizeof( TextVertex ) * m_vertexCount ) );
	pDeviceContext->Unmap( m_pVertexBuffer, 0 );

	// Release the vertex array 
	delete [] pVertices;
	pVertices = nullptr;

	return true;
}

//--------------------------------------------------------------------------------------
// Render the sentence.
//--------------------------------------------------------------------------------------
void SentenceImplementation::Render( ID3D11DeviceContext* pDeviceContext )
{
	// Set vertex buffer stride and offset
	unsigned int stride = sizeof( TextVertex ); 
	unsigned int offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Render the triangles.
	pDeviceContext -> DrawIndexed(m_indexCount, 0, 0);
}



