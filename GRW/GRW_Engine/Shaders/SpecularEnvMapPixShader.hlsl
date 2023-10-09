
struct VSOutput
{
	float4 vPos : SV_POSITION;
	float3 localPos : LPOSITION;
};

cbuffer Cbuffer : register(b1)
{
	matrix MViewProj;
	float4 Mip;
}

TextureCube genCubeMap : register(t3);
SamplerState samplerTest: register(s0);

static const float PI = 3.14159265359f;

float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
float2 Hammersley(uint i, uint N)
{
	return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

//generate random sampling directions biased towards sampling normal
float3 ImportanceSampleGGX(float2 Xi, float3 N, float roughness)
{
	float a = roughness * roughness;
	float phi = 2.0 * PI * Xi.x;
	float cosT = sqrt((1 - Xi.y) / (1 + ((a*a) - 1) * Xi.y));
	float sinT = sqrt(1 - (cosT*cosT));

	float3 H;
	H.x = cos(phi) * sinT;
	H.y = sin(phi) * sinT;
	H.z = cosT;

	float3 up = abs(N.z) < 0.999 ? float3(0, 0, 1.0) : float3(1.0, 0, 0);
	//float3 up = float3(0,0,N.z);
	float3 tangent = normalize(cross(up, N));
	float3 bitangent = cross(N, tangent);

	float3 sampleVec = (tangent * H.x) + (bitangent * H.y) + (N * H.z);
	return normalize(sampleVec);
}
float DistributionGGX(float3 N, float3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}
float4 main(VSOutput pIN) : SV_TARGET
{
	float3 N = normalize(pIN.localPos);
	float3 R = N;
	float3 V = R;

	const uint SAMPLE_COUNT = 1024u;
	float totalWeight = 0.0;
	float3 prefilteredColor = float3(0, 0, 0);

	for (uint i = 0; i < SAMPLE_COUNT; ++i)
	{
		float2 Xi = Hammersley(i, SAMPLE_COUNT);
		float3 H = ImportanceSampleGGX(Xi, N, Mip.x);
		float3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = max(dot(N, L), 0.0);
		if (NdotL > 0.0)
		{
			prefilteredColor += genCubeMap.Sample(samplerTest, L).rgb * NdotL;
			totalWeight += NdotL;
		}

	}
	prefilteredColor = prefilteredColor / totalWeight;
	return float4(prefilteredColor.rgb, 1.0);
}

