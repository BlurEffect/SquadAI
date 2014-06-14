/* 
*  Kevin Meergans, Squad, 2014
*  VS_SimpleInstanced.fx
*  Alternate version to VS_Simple.fx allowing for hardware instancing.
*/

cbuffer PerFrameBuffer
{
	row_major float4x4 ViewProjection;   // The combined view and projection matrices
};

struct VS_INPUT
{
	// vertex data
	float4 Position	 : POSITION;	// The position of the vertex

	// instance data
	row_major float4x4 World  : WORLD;   // The world matrix to use
	float4			   Colour : COLOUR;	 // The vertex colour 
};

struct VS_OUTPUT
{
	float4 Position	: SV_POSITION; // The position of the vertex in clip space
	float4 Colour	: COLOUR;	   // The colour to use for this vertex
};

VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output;

    // transform the vertex position
    output.Position = mul( input.Position, ( mul( input.World, ViewProjection) ) );

	// forward vertex colour
	output.Colour = input.Colour;

    return output;
}