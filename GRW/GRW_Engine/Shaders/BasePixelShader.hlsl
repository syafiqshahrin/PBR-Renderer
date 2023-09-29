
struct VSOutput
{
	float4 vPos : SV_POSITION;
	float4 col : COLOR;
	float3 norm : NORMAL;
	float3 normWS : NORMAL1;
	float2 texcoord0 : TEXCOORD0;
};

cbuffer Cbuffer : register(b0)
{
	float4 time;
	matrix MVP;
	matrix MW;
	matrix MC;
	matrix MNorm;
	float3 lightData;

}

Texture2D testTex: register(t0);
SamplerState samplerTest: register(s0);


float4 main(VSOutput pIN) : SV_TARGET
{
	float3x3 NormalMatrix = (float3x3)MNorm;
	float3 fixedNormals = mul(NormalMatrix, pIN.normWS);
	float lighting = saturate(dot(pIN.normWS, normalize(lightData) * -1));
	float2 uv = pIN.texcoord0;
	float timeScaled = time.x * 0.01;
	float4 tex = testTex.Sample(samplerTest, uv);
	//float4 color = lerp(float4(pIN.norm, 1), float4(sin(timeScaled + pIN.col.x) + 1 * 0.5, sin(timeScaled + pIN.col.y) + 1 * 0.5, sin(timeScaled + pIN.col.z) + 1 * 0.5, 1), 1);
	float4 color = float4(pIN.normWS, 1) * lighting;
	//float4 color = float4(pIN.normWS, 0);
	return color;
	//return float4(1,0,0,1);
}