#pragma once

static const float PI = 3.14159265f;

float3 FresnelSchlick(float3 baseC, float metal, float3 v, float3 h)
{
	// FresnelFactor using Fresnel-Schlick: F = F0 + (1 - F0) (1 - (dot(V, H))^5
	float3 F0 = lerp(0.04, baseC.rgb, metal);
	float p = pow(clamp(1 - dot(v, h), 0, 1) , 5.0);
	float3 F = F0 + (float3(1, 1, 1) - F0) * p;
	return F;
}

float3 FresnelSchlickRoughness(float3 baseC, float metal, float roughness, float3 v, float3 n)
{
	// FresnelFactor using Fresnel-Schlick: F = F0 + (1 - F0) (1 - (dot(V, H))^5
	float3 F0 = lerp(0.04, baseC.rgb, metal);
	float p = pow(1 - clamp(dot(n, v), 0.0, 1.0), 5.0);
	float3 f3 = max(float3(1 - roughness, 1 - roughness, 1 - roughness), F0);
	float3 F = F0 + (f3 - F0) * p;
	return F;
}

float3 DiffuseFract(float f, float metal)
{
	// K = (1 - F) * (1 - m)
	float3 K = (float3(1, 1, 1) - f) * (1 - metal);
	return K;
}

float3 DiffuseBRDF(float3 baseC)
{
	//Diffuse = (BaseColor / PI)
	float3 dif = baseC / PI;
	return dif;
}

float NormalDistributionFunction(float roughness, float3 n, float3 h)
{
	//Normal distribution function: (GGX Trowbridge-Reitz) D = (a ^ 2) / PI * (dot(N,H)^2 * (a^2 - 1) + 1)^2
	float a2 = roughness * roughness;
	float ndoth = max(dot(n, h), 0.00001);
	float ndoth2 = ndoth * ndoth;
	float denom1 = (ndoth2 * (a2 - 1) + 1);
	float denom2 = denom1 * denom1;
	float fullDenom = PI * denom2;
	float D = a2 / fullDenom;
	return D;
}

float GeometrySelfShadowingSchlick(float roughness, float3 n, float3 x)
{
	//Schlick GGX
	// k = a/2
	// G1(N,X) = dot(N,X) / (dot(N, X)) * 1 - k) + k
	float k = roughness / 2;
	float ndotx = max(dot(n, x), 0.0);
	float denom = ndotx * (1 - k) + k;
	float G1 = ndotx / denom;
	return G1;
}

float GeometrySelfShadowingSmith(float roughness, float3 n, float3 v, float3 l)
{
	//Geometry shadowing function: (Schlick GGX = Combination of Schlick-Beckman model and the Smith model) G = G1(L,N) * G1(V,N)
	float G = GeometrySelfShadowingSchlick(roughness, n, l) * GeometrySelfShadowingSchlick(roughness, n, v);
	return G;
}

float3 SpecularBRDF(float3 baseC, float roughness, float metallic, float3 n, float3 v, float3 l, float3 h)
{
	//Specular = (D * G * F) / (4 * (dot(V, N)) * (dot(L, N)))
	float D = NormalDistributionFunction(roughness, n, h);
	float G = GeometrySelfShadowingSmith(roughness, n, v, l);
	float3 F = FresnelSchlick(baseC, metallic, v, h);

	float3 numerator = D * G * F;
	float ndotv = max(dot(n, v), 0);
	float ndotl = max(dot(n, l), 0);
	float denom = 4 * ndotv  + 0.0001;
	float3 Specular = numerator / denom;
	return Specular;

}

//Assigned at runtime
TextureCube IrradianceMap: register(t3);
TextureCube SpecularEnvMap: register(t4);
Texture2D SpecIntBRDF: register(t5);