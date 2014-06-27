/* 
*  Kevin Meergans, SquadAI, 2014
*  VS_Font.fx
*  Vertex shader used for font rendering that performs standard transformation on the vertex position 
*  and forwards it together with the unaltered texture coordinates to the next stage.
*/

cbuffer PerFrameBuffer
{
	row_major float4x4 ViewProjection;	// The combined view and projection matrices (identity matrix is assumed as world matrix for text rendering)
};

struct VS_INPUT
{
	float4 Position : POSITION; // The position of the vertex
    float2 TexCoord : TEXCOORD; // The texture coordinates of the vertex
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION; // The position of the vertex in clip space
	float2 TexCoord : TEXCOORD;    // The texture coordinates
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

	// Transform the vertex position
    output.Position = mul(input.Position, ViewProjection);

	// Forward texture coordinate unchanged
    output.TexCoord = input.TexCoord;

    return output;
}

