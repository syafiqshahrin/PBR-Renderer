
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
	float3 B : BITANWS;
};

cbuffer Cbuffer : register(b0)
{
	float4 time;
	matrix MVP;
	matrix MW;
	matrix MC;
	matrix MNorm;
	float3 lightData;
	float4 Ambient;
	float4 CamWS;
	float4 PLightPos;
	float4 PLightCol;

}

Texture2D DiffuseTex: register(t0);
Texture2D NormTex: register(t1);
Texture2D RMATex: register(t2);
TextureCube Cubemap: register(t3);
SamplerState samplerTest: register(s0);

static const float PI = 3.14159265f;

float3 FresnelSchlick(float3 baseC, float metal, float3 v, float3 h)
{
	// FresnelFactor using Fresnel-Schlick: F = F0 + (1 - F0) (1 - (dot(V, H))^5
	float3 F0 = lerp(0.04, baseC.rgb, metal);
	float p = pow(1 - max(dot(v, h), 0.0001), 5.0);
	float3 F = F0 + (float3(1,1,1) - F0) * p;
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
	float ndoth = max(dot(n, h), 0.0001);
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
	float ndotx = max(dot(n, x), 0.0001);
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
	float ndotv = max(dot(n, v), 0.0001);
	float ndotl = max(dot(n, l), 0.0001);
	float denom = 4 * ndotv * ndotl;
	float3 Specular = numerator / denom;
	return Specular;

}

float4 main(VSOutput pIN) : SV_TARGET
{
	//Time
	float timeScaled = time.x * 0.01;
	
	//Can move to cbuffer later on
	float lightIntensity = 3;
	float4 lightCol = float4(1,1,1,1) * lightIntensity;

	//Normal and Tangent
	//float3x3 NormalMatrix = (float3x3)MNorm;
	//float3 normalWS = mul(NormalMatrix, pIN.norm);
	
	float4 tan = pIN.tangent;
	float3 bitan = cross(pIN.norm, tan.xyz) * tan.w;
	//float3 B = normalize(mul(MW, float4(bitan.xyz, 0)).xyz);
	float3 B = normalize(mul(MNorm, bitan));



	float3 T = normalize(pIN.T);
	float3 Nv = normalize(pIN.N);
	//float3 B = normalize(pIN.B);

	float3x3 Mtbn = {
		T.x, T.y, T.z,
		B.x, B.y, B.z,
		Nv.x, Nv.y, Nv.z
	};

	//Textures
	float2 uv = pIN.texcoord0 * 2;
	float4 baseColor = DiffuseTex.Sample(samplerTest, uv);
	float4 RMA = RMATex.Sample(samplerTest, uv);
	float4 texNorm = NormTex.Sample(samplerTest, uv) * 2.0f - 1.0f;


	//Lighting Vectors
	float3 N = normalize(mul(transpose(Mtbn), texNorm.xyz));
	float3 V = normalize(CamWS.rgb - pIN.posWS.xyz);

	//environment map temp
	float3 R = reflect(-V, N);
	float4 Sky = Cubemap.Sample(samplerTest, R.xyz);

	//for Directional Light
	float3 L0 = normalize(lightData * -1) ;
	float3 H0 = normalize(L0 + V);
	float NDL0 = saturate(dot(N, L0));

	//for Point Light
	float3 L1 = normalize(PLightPos.xyz - pIN.posWS.xyz);
	float3 H1 = normalize(L1 + V);
	float NDL1 = saturate(dot(N, L1));
	float dist1 = length(PLightPos.xyz - pIN.posWS.xyz);
	float3 lightCol1 = PLightCol.rgb * (PLightCol.a * (1 / pow(dist1, 2)));

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

	//float r = RMA.r;
	float3 FinalLight;

	//for directional light
	float r = max(RMA.r, 0.001);
	//float r = 0.1;
	float m = RMA.g;
	//float m = 1;

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
	//FinalLight += final + pow(((Sky.rgb/PI) * (1-r)), 1); //with environment map hack/test/temp

	FinalLight = pow(FinalLight.rgb, (1.0 / 2.2));

	//Final Color
	float4 color = float4(FinalLight.rgb,1);
	float4 test = float4(Sky.rgb,1);
	return test;

}

