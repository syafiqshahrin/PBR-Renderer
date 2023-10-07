struct VSOutput
{
	float4 vPos : SV_POSITION;
	float3 localPos : LPOSITION;
	float3 worldPos : WPOSITION;
};

cbuffer Cbuffer : register(b1)
{
	matrix MViewProj;
	matrix MW;
}


TextureCube genCubeMap: register(t3);
SamplerState samplerTest: register(s0);


float4 main(VSOutput pIN) : SV_TARGET
{

	float3 Skybox = genCubeMap.Sample(samplerTest, pIN.localPos).rgb;

	return float4(Skybox.rgb, 1);
}