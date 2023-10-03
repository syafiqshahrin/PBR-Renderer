
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

}

Texture2D DiffuseTex: register(t0);
Texture2D NormTex: register(t1);
Texture2D RMATex: register(t3);
SamplerState samplerTest: register(s0);

static const float PI = 3.14159265f;


float4 main(VSOutput pIN) : SV_TARGET
{
	//Time
	float timeScaled = time.x * 0.01;
	
	//Can move to cbuffer later on
	float lightIntensity = 1;
	float4 lightCol = float4(1,1,1,1) * lightIntensity;

	//Normal and Tangent
	//float3x3 NormalMatrix = (float3x3)MNorm;
	//float3 normalWS = mul(NormalMatrix, pIN.norm);
	
	//float4 tan = pIN.tangent;
	//float3 bitan = cross(pIN.norm, tan.xyz) * tan.w;
	//float3 B = normalize(mul(MW, float4(bitan.xyz, 0)).xyz);
	//float3 B = normalize(mul(MNorm, bitan));



	float3 T = normalize(pIN.T);
	float3 N = normalize(pIN.N);
	float3 B = normalize(pIN.B);

	float3x3 Mtbn = {
		T.x, T.y, T.z,
		B.x, B.y, B.z,
		N.x, N.y, N.z
	};

	//Textures
	float2 uv = pIN.texcoord0 * 3;
	float4 baseColor = DiffuseTex.Sample(samplerTest, uv);
	float4 RMA = RMATex.Sample(samplerTest, uv);
	float4 texNorm = NormTex.Sample(samplerTest, uv) * 2.0f - 1.0f;

	float3 testNorm = normalize(mul(transpose(Mtbn), texNorm.xyz));

	//Blinn Phong + Lambert Lighting
	float3 L = normalize(lightData) * -1;
	float3 V = normalize(CamWS.xyz - pIN.posWS.xyz);
	float3 H = normalize(L + V);
	float BlinnPhongSpec = pow(saturate(dot(H, testNorm)), 1000.0);
	float NDL = saturate(dot(testNorm, L));
	float4 Spec = BlinnPhongSpec * lightCol * 0;
	float4 Diffuse = NDL * lightCol;
	//float4 Light = NDL * (lightCol + (lightCol * BlinnPhongSpec)) + Ambient;
	float4 Light = Diffuse + Spec + Ambient;
	float4 color = saturate(baseColor * Light);


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

	float m = RMA.r;
	float a = RMA.g;
	float3 F0 = lerp(0.04, baseColor.rgb, m);
	//F-Schlick
	float F = F0 + (1 - F0) * pow((1 - saturate(dot(V, H))), 5.0);
	float K = (1 - F) * (1 - m);
	float Diffuse = (baseColor.rgb / PI);
	float D = (a * a) / PI * ((saturate(dot(N, H)) * saturate(dot(N, H)) * (a * a - 1) + 1) * (saturate(dot(N, H)) * saturate(dot(N, H)) * (a * a - 1) + 1)); //can be optimized more
	float k = a / 2;
	float G = (saturate(dot(N, L)) / saturate(max(dot(N, L), 0.0001)) * (1 - k) + k) * (saturate(dot(N, V)) / saturate(max(dot(N, V), 0.0001)) * (1 - k) + k);
	float Specular = (D * G * F) / (4 * saturate(max(dot(V, N), 0.0001)) * saturate(max(dot(L, N), 0.0001)));
	float3 BRDF = (K * Diffuse) + Specular;

	float3 final = BRDF * lightCol * NDL;


	//Final Color
	color = saturate(baseColor * Light);
	float4 test = float4(BlinnPhongSpec.xxxx);
	return color;

}

