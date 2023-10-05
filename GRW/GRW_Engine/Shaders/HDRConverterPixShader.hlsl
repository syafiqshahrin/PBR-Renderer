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


Texture2D DiffuseTex: register(t0);
TextureCube genCubeMap: register(t3);
SamplerState samplerTest: register(s0);

static const float PI = 3.14159265f;
//static const float2 invAtan = float2(0.1591, 0.3183);
static const float2 invAtan = float2(1/(2*PI), 1/(PI));

float2 GetSphericalMapUV(float3 pos)
{
	float2 uv = float2(atan2(pos.z, pos.x), asin(pos.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}


float4 main(VSOutput pIN) : SV_TARGET
{
	float2 uv = GetSphericalMapUV(normalize(pIN.localPos));
	float3 col = DiffuseTex.Sample(samplerTest, uv).rgb;
	//float3 test = genCubeMap.Sample(samplerTest, pIN.localPos);

	return float4(col.rgb, 1);
}