
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
Texture2D testTexNorm: register(t1);
SamplerState samplerTest: register(s0);


float4 main(VSOutput pIN) : SV_TARGET
{
	
	
	float lighting = saturate(dot(pIN.normWS, normalize(lightData) * -1))  * 1;
	float2 uv = pIN.texcoord0;
	float timeScaled = time.x * 0.01;
	float4 tex = testTexNorm.Sample(samplerTest, uv);
	float4 color = tex * lighting;
	return color;
	//return float4(1,0,0,1);
}