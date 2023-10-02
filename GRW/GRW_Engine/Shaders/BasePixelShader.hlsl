
struct VSOutput
{
	float4 vPos : SV_POSITION;
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

}

Texture2D DiffuseTex: register(t0);
Texture2D NormTex: register(t1);
SamplerState samplerTest: register(s0);


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
	float4 tex = DiffuseTex.Sample(samplerTest, uv);
	float4 texNorm = NormTex.Sample(samplerTest, uv) * 2.0f - 1.0f;

	float3 testNorm = normalize(mul(transpose(Mtbn), texNorm.xyz));

	//Lighting

	float NDL = saturate(dot(testNorm, normalize(lightData) * -1));
	float4 Light = lightCol * NDL + Ambient;


	//Final Color
	float4 color = saturate(tex * Light);
	float4 test = float4(testNorm.xyz, 1);
	return color;

}