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
};

cbuffer Cbuffer : register(b0)
{
	float4 time;
	matrix MVP;
	matrix MW;
	matrix MC;
	matrix MNorm;
	matrix MViewProj;
	float3 lightData;
	float4 Ambient;
	float4 CamWS;
	float4 PLightPos;
	float4 PLightCol;
}

VSOutput main(VSInput vIn)
{
	VSOutput pixOut;

	//Position
	float4 pos4 = float4(vIn.vPos, 1);
	pixOut.vPos = mul(MViewProj, pos4);
	pixOut.localPos = vIn.vPos;

	return pixOut;
}