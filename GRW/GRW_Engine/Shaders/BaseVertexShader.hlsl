struct VSInput
{
	float3 vPos : POSITION;
	float4 vCol : COLOR;
};

struct VSOutput
{
	float4 vPos : SV_POSITION;
	float4 col : COLOR;
};
cbuffer Cbuffer : register(b0)
{
	matrix MVP;

}
VSOutput main( VSInput vIn)
{
	VSOutput pixOut;
	float4 p = float4(vIn.vPos,1);
	pixOut.vPos = mul(MVP, p);
	pixOut.col = vIn.vCol;
	return pixOut;
}