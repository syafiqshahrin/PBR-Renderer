struct VSInput
{
	float3 vPos : POSITION;
	float3 vNorm : NORMAL;
	float2 vTexCoord0 : TEXCOORD;
	float4 vCol : COLOR;
	float4 vTangent : TANGENT;

};
struct VSOutput
{
	float4 vPos : SV_POSITION;
	float3 localPos : LPOSITION;
	float3 worldPos : WPOSITION;
};

#include "../Shaders/Buffers.hlsli"

VSOutput main(VSInput vIn)
{
	VSOutput pixOut;

	//Position
	float4 pos4 = float4(vIn.vPos, 1);
	pixOut.vPos = mul(MViewProj, pos4).xyww;
	pixOut.localPos = vIn.vPos;
	pixOut.worldPos = mul(MW, pos4).xyz;

	return pixOut;
}