
struct VSOutput
{
	float4 vPos : SV_POSITION;
	float4 col : COLOR;
};

cbuffer Cbuffer : register(b0)
{
	float4 time;
	matrix MVP;

}
float4 main(VSOutput pIN) : SV_TARGET
{
	float timeScaled = time.x * 0.01;

	float4 color = lerp(pIN.col, float4(sin(timeScaled + pIN.col.x) + 1 * 0.5, sin(timeScaled + pIN.col.y) + 1 * 0.5, sin(timeScaled + pIN.col.z) + 1 * 0.5, 1), sin(timeScaled + pIN.col.x) + 1 * 0.5);
	return color;
	//return float4(1,0,0,1);
}