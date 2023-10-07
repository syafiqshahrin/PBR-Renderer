struct VSInput
{
	float3 vPos : POSITION;
	float3 vNorm : NORMAL;
	float2 vTexCoord0 : TEXCOORD;
	float4 vCol : COLOR;
	float4 vTangent : TANGENT;
	
};

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
VSOutput main( VSInput vIn)
{
	VSOutput pixOut;

	//Position
	float4 pos4 = float4(vIn.vPos, 1);
	pixOut.vPos = mul(MVP, pos4);
	pixOut.posWS = mul(MW, pos4);

	//UVs
	pixOut.texcoord0 = vIn.vTexCoord0;

	//Normal and Tangent
	//float3 t = mul(MW, float4(vIn.vTangent.xyz, 0)).xyz;
	//float3 n = mul(MW, float4(vIn.vNorm.xyz, 0)).xyz;
	float3 t = mul(MNorm, vIn.vTangent.xyz);
	float3 n = mul(MNorm, vIn.vNorm);

	pixOut.T = t;
	pixOut.N = n;

	pixOut.tangent = vIn.vTangent;
	pixOut.norm = vIn.vNorm;


	return pixOut;
}