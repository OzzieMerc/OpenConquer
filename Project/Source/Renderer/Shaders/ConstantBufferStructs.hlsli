/*
-------------------------------------------------------------------------------------------------------
	File: ConstantBufferStructs.hlsli
	Author: Ozzie Mercado
	Created: December 11, 2020
	Modified: December 15, 2020
	Description: Defines 16-byte aligned DirectX11 compatible structures for the constant buffer and 
        others for the time being. For use inside C++ source or HLSL shader file.
-------------------------------------------------------------------------------------------------------
*/

#ifdef __cplusplus
#pragma once

#include <DirectXMath.h>

#define cbuffer struct
#define matrix DirectX::XMMATRIX
#define float4 DirectX::XMVECTOR

namespace OC
{
#endif

cbuffer MVP
{
    matrix model;
    matrix view;
    matrix projection;
};

cbuffer ColorVert
{
    float4 pos = { 0.0f, 0.0f, 0.0f, 0.0f };
    float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
};

#ifdef __cplusplus
}
#endif