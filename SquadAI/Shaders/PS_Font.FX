/* 
*  Kevin Meergans, SquadAI, 2014
*  PS_Font.fx
*  Performs a texture lookup on a given font texture and return either a transparent
*  pixel or a pixel using the current font colour set in the constant buffer.
*/

// The texture holding all the characters for the font
Texture2D	 FontTexture;
// The sampler state used to sample the font texture
SamplerState FontSamplerState;

cbuffer PerObjectBuffer
{
	float4 Colour; // The current font colour
};

struct PS_INPUT
{
	float4 Position : SV_POSITION; 
    float2 TexCoord : TEXCOORD;    // The texture coordinates into the font texture at this pixel
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    // Sample the font texture and extract the colour value for the pixel
    float4 colour = FontTexture.Sample(FontSamplerState, input.TexCoord);
	
	// The font in the texture is white characters on a black background, so if the sampled
	// value is black, make the pixel transparent (with the characters being white, it is
	// sufficient to check a single colour component to see whether the sample belongs to
	// a character or the background of the texture)
    if(colour.r == 0.0f)
    {
        colour.a = 0.0f;
		return colour;
    }
	
    // If the sample belongs to a character, get the final colour by multiplying it
	// with the current font colour
	return colour * Colour;
}

