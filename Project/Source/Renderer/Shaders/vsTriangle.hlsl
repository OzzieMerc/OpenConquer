/*
-------------------------------------------------------------------------------------------------------
	File: vsTriangle.hlsl
	Author: Ozzie Mercado
	Created: December 13, 2020
	Modified: December 15, 2020
	Description: Creates 3 vertices in the shape of a triangle.
-------------------------------------------------------------------------------------------------------
*/

#include "ConstantBufferStructs.hlsli"

struct VSIn
{
	uint vertId : SV_VertexID;
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
};

static const float2 verts[3] =
{
    float2(0.9f, 0.9f),
	float2(0.9f, -0.9f),
	float2(-0.9f, -0.9f)
};

static const float3 colors[3] =
{
    float3(1.0f, 1.0f, 0.0f),
	float3(0.0f, 1.0f, 1.0f),
	float3(1.0f, 0.0f, 1.0f)
};

static const float3 norm = float3(0.0f, 0.0f, -1.0f);

VSOut main(VSIn _input)
{
	VSOut output;

    output.pos = float4(verts[_input.vertId], 0, 1);
    output.color = float4(colors[_input.vertId], 1.0f);
    output.normal = float4(norm, 0.0f);
    
    return output;
}
	