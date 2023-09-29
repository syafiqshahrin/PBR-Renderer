struct VSInput
{
	float3 vPos : POSITION;
	float3 vNorm : NORMAL;
	float2 vTexCoord0 : TEXCOORD;
	float4 vCol : COLOR;
};

struct VSOutput
{
	float4 vPos : SV_POSITION;
	float4 col : COLOR;
	float3 norm : NORMAL;
	float2 texcoord0 : TEXCOORD0;
};
cbuffer Cbuffer : register(b0)
{
	float4 time;
	matrix MVP;
}
VSOutput main( VSInput vIn)
{
	VSOutput pixOut;
	//float3 p = vIn.vPos + normalize(vIn.vPos) * (sin(time.x * 0.001));
	//float4 p4 = float4(p,1);
	float4 p4 = float4(vIn.vPos, 1);
	pixOut.vPos = mul(MVP, p4);
	pixOut.col = vIn.vCol;
	pixOut.norm = vIn.vNorm;
	pixOut.texcoord0 = vIn.vTexCoord0;
	return pixOut;
}