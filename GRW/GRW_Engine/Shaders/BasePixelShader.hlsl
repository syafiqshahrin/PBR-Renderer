
struct VSOutput
{
	float4 vPos : SV_POSITION;
	float4 col : COLOR;
};
float4 main(VSOutput pIN) : SV_TARGET
{
	float4 color = pIN.col;
	return color;
	//return float4(1,0,0,1);
}