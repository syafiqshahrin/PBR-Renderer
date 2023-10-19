struct VSOutput
{
	float4 vPos : SV_POSITION;
	float3 posWS : POSITIONWS;
	float4 col : COLOR;
	float3 norm : NORMAL;
	float4 tangent : TANGENT;
	float2 texcoord0 : TEXCOORD0;
	float3 T : TANGENTWS;
	float3 N : NORMALWS;
};


cbuffer MaterialCBuffer : register(b2)
{
	float4 Tiling;
	float SpecularStrength;
	float4 Ambient;
}

#include "../Shaders/Buffers.hlsli"
#include "../Shaders/Lighting.hlsli"


Texture2D DiffuseTex: register(t0);
Texture2D NormTex: register(t1);
Texture2D SpecularTex: register(t2);

SamplerState baseSampler: register(s0);

float4 main(VSOutput pIN) : SV_TARGET
{
	float lightIntensity = DirectionaLightWS.w;
	float3 lightCol = DirectionaLightColor.rgb * lightIntensity;

	float sign = pIN.tangent.w < 0 ? -1 : 1;
	float3 T = pIN.T;
	float3 Nv = pIN.N;
	float3 B = cross(Nv, T);

	

	float2 uv = pIN.texcoord0;
	uv.y *= Tiling.x;
	uv.x *= Tiling.y;

	float4 baseColor = DiffuseTex.Sample(baseSampler, uv);
	float4 Specular = SpecularTex.Sample(baseSampler, uv);
	float4 texNorm = NormTex.Sample(baseSampler, uv) * 2.0f - 1.0f;

	float3 N = GetTangentToWorlNormals(texNorm, T, Nv, B);
	float3 V = normalize(CamWS.rgb - pIN.posWS.xyz);

	float3 L = normalize(DirectionaLightWS.rgb * -1);
	float3 H = normalize(L + V);

	float NDL = max(dot(N, L), 0.0);

	float BlinnPhongSpec = pow(saturate(dot(H, N)), SpecularStrength);
	float4 Spec = BlinnPhongSpec * Specular.r;
	float4 Diffuse = baseColor;
	//float4 Light = NDL * (lightCol + (lightCol * BlinnPhongSpec)) + Ambient;
	float4 Light = ((Diffuse + Spec) * NDL) + (Diffuse * Ambient * (1 - NDL));
	//float4 color = saturate(baseColor * Light);

	return Light;
}