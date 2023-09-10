struct VSInput
{
	float4 vPos : POSITION;
};

struct VSOutput
{
	float4 vPos : SV_POSITION;
};

VSOutput main( VSInput vIn)
{
	VSOutput pixOut;
	pixOut.vPos = vIn.vPos;
	return pixOut;
}