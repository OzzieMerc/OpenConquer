/*
-------------------------------------------------------------------------------------------------------
	File: psTriangle.hlsl
	Author: Ozzie Mercado
	Created: December 13, 2020
	Modified: December 14, 2020
	Description: Draws each pixel.
-------------------------------------------------------------------------------------------------------
*/

struct VSOut
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
};

struct PS_OUTPUT 
{
	float4 color : SV_TARGET;
};

PS_OUTPUT main(VSOut _input)
{
	PS_OUTPUT output;

	output.color = _input.color;

	return output;
}