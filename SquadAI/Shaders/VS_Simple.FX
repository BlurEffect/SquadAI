/* 
*  Kevin Meergans, SquadAI, 2014
*  VS_Simple.fx
*  Performs standard transformation of the vertex position and forwards
*  the result together with the object colour to the next stage.
*/

cbuffer PerObjectBuffer
{
	row_major float4x4 WorldViewProjection; // The combined world, view and projection matrices
	float4 Colour;							// The colour to use for this vertex
};

struct VS_INPUT
{
	float4 Position : POSITION; // The position of the vertex
};

struct VS_OUTPUT
{
	float4 Position	: SV_POSITION; // The position of the vertex in clip space
	float4 Colour	: COLOUR;	   // The colour to use for this vertex
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    // transform the vertex position
    output.Position = mul(input.Position, WorldViewProjection);

	// forward object colour
	output.Colour = Colour;

    return output;
}