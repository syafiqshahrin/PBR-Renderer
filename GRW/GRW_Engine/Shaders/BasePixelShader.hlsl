
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

	float4 color = pIN.col + float4(sin(timeScaled), sin(timeScaled), sin(timeScaled),1);
	return color;
	//return float4(1,0,0,1);
}