
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

//buffer 0 should be saved for things updated once per frame
// - CamWS (Position)
// - MCam (Camera World Matrix)
// - MView (View Matrix)
// - Time 
// - DirectionalLightPositionWS/Intensity
// - DirectionalLightColor
//buffer 1 should be saved per object
// - MVP
// - MWorld
// - MNorm
//buffer 2 should be saved for per material

#include "../Shaders/Buffers.hlsli"
#include "../Shaders/Lighting.hlsli"

cbuffer MaterialCBuffer : register(b2)
{
	float2 Tiling;
}

//Define by Material
Texture2D DiffuseTex: register(t0);
Texture2D NormTex: register(t1);
Texture2D RMATex: register(t2);

SamplerState samplerTest: register(s0);


float4 main(VSOutput pIN) : SV_TARGET
{
	//Time
	float timeScaled = time.x * 0.01;
	
	//Can move to cbuffer later on
	float lightIntensity = DirectionaLightWS.w;
	float3 lightCol = DirectionaLightColor.rgb * lightIntensity;

	//Normal and Tangent
	float sign = pIN.tangent.w < 0 ? -1 : 1;
	float3 T = pIN.T;
	float3 Nv = pIN.N;
	float3 B = cross(Nv, T);


	//Textures
	float2 uv = pIN.texcoord0;
	uv.y *= Tiling.x;
	uv.x *= Tiling.y;
	float4 baseColor = DiffuseTex.Sample(samplerTest, uv);
	float4 RMA = RMATex.Sample(samplerTest, uv);
	float4 texNorm = NormTex.Sample(samplerTest, uv) * 2.0f - 1.0f;


	//Lighting Vectors
	float3 N = GetTangentToWorlNormals(texNorm, T, Nv, B);
	float3 V = normalize(CamWS.rgb - pIN.posWS.xyz);

	//environment map temp
	float3 R = reflect(-V, N);

	//float4 Sky = Cubemap.Sample(samplerTest, R.xyz);
	//float4 Ambient = IrradianceMap.Sample(samplerTest, Nv.xyz);

	//for Directional Light
	float3 L0 = normalize(DirectionaLightWS.rgb * -1) ;
	float3 H0 = normalize(L0 + V);
	float NDL0 = max(dot(N, L0), 0.0);

	//for Point Light
	float3 L1 = normalize(PLightPos.xyz - pIN.posWS.xyz);
	float3 H1 = normalize(L1 + V);
	float NDL1 = max(dot(N, L1), 0.0);
	float dist1 = length(PLightPos.xyz - pIN.posWS.xyz);
	
	float d2 = dist1 * dist1;
	float rad = PLightPos.w;
	float r2 = rad * rad;
	
	//testing different attenuation formulas
	//float att1 = PLightCol.a * ( (2 / r2) * (1 - (dist1 / sqrt(d2 + r2) ) ) );
	float att1 = PLightCol.a * (1 / d2);
	float3 lightCol1 = PLightCol.rgb * att1;

	//Blinn Phong + Lambert Lighting
	/*
	float BlinnPhongSpec = pow(saturate(dot(H, testNorm)), 1000.0);
	float4 Spec = BlinnPhongSpec * lightCol * 0;
	float4 Diffuse = NDL * lightCol;
	//float4 Light = NDL * (lightCol + (lightCol * BlinnPhongSpec)) + Ambient;
	float4 Light = Diffuse + Spec + Ambient;
	float4 color = saturate(baseColor * Light);
	*/

	//PBR Lighting
	//general rendering equation: FinalColor =  sum (BRDF * Light * NDOTL)
	//Using cook torrance brdf
	// (K * diffuse) + spec = 1
	// L = light vector
	// V = view vector
	// H = half vector
	// a = surface roughness
	// m = metallic
	// FresnelFactor using Fresnel-Schlick: F = F0 + (1 - F0) (1 - (dot(V, H))^5
	// F0 = lerp(0.04, baseColor.rgb, m)
	// K = (1 - F) * (1 - m)
	//Diffuse = (BaseColor/ PI) * (dot(N, L)) --- ndotl is cut out?
	//Specular = (D * G * F) / (4 * (dot(V, N)) * (dot(L, N)))
	//Normal distribution function: (GGX Trowbridge-Reitz) D = (a ^ 2) / PI * (dot(N,H)^2 * (a^2 - 1) + 1)^2
	//Geometry shadowing function: (Schlick GGX = Combination of Schlick-Beckman model and the Smith model) G = G1(L,N) * G1(V,N)
	// G1(N,X) = dot(N,X) / (dot(N, X)) * 1 - k) + k
	// k = a/2
	//F = Fresnel Schlick

	float r = max(RMA.r, 0.0001);
	//float r = 0.0001;
	float m = RMA.g;
	float ao = RMA.b;
	//float m = 0;
	
	
	//Lighting
	float3 FinalLight;
	//for directional light
	float3 F = FresnelSchlick(baseColor.rgb, m, V, H0);
	float3 K = DiffuseFract(F, m);

	float3 Diffuse = DiffuseBRDF(baseColor.rgb);
	float3 Specular = SpecularBRDF(baseColor.rgb, r, m, N, V, L0, H0);

	float3 BRDF = (K * Diffuse) + Specular;

	float3 final = BRDF * lightCol.rgb * NDL0;
	FinalLight = final;
	
	//for point light
	
	F = FresnelSchlick(baseColor.rgb, m, V, H1);
	K = DiffuseFract(F, m);

	Diffuse = DiffuseBRDF(baseColor.rgb);
	Specular = SpecularBRDF(baseColor.rgb, r, m, N, V, L1, H1);

	BRDF = (K * Diffuse) + Specular;

	final = BRDF * lightCol1.rgb * NDL1;
	FinalLight += final; 



	//Gamma Correction	
	FinalLight = pow(FinalLight.rgb, (1.0 / 2.2));

	//Diffuse Ambient term
	float3 kS = FresnelSchlickRoughness(baseColor.rgb, m, r, V, N);
	float3 kD = 1.0 - kS;
	kD *= 1 - m;
	float3 irradiance = IrradianceMap.Sample(samplerTest, N.xyz);
	float3 iblDiffuse = irradiance * baseColor.rgb;

	//Spec ambient term
	//const float MAX_REFLECTION_LOD = 4.0;
	float3 specEnvMap = SpecularEnvMap.SampleLevel(samplerTest, R, r * MAX_REFLECTION_LOD).rgb;
	float2 envBRDF = SpecIntBRDF.Sample(samplerTest, float2(max(dot(N, -V), 0.0), r)).rg;
	float3 specular = specEnvMap * (kS * envBRDF.x + envBRDF.y);

	//Final Ambient term
	float3 ambient = (kD * iblDiffuse + specular) * ao;
	
	//Final Light Color
	FinalLight += ambient;

	//Final Color
	float4 color = float4(FinalLight.rgb,1);
	float4 test = float4(BRDF.rgb,1);
	return color;

}

