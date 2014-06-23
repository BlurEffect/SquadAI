/* 
*  Kevin Meergans, SquadAI, 2014
*  PS_Simple.fx
*  Simple pixel shader that returns the vertex colour passed through from
*  the vertex shader as pixel colour.
*/

struct VS_OUTPUT
{
	float4 Position	: SV_POSITION;
	float4 Colour	: COLOUR;
};

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return input.Colour;
}
