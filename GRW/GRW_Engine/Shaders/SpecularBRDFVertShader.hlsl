
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
	float3 localPos : LPOSITION;
	float2 uv : TEXCOORD0;
};



VSOutput main(VSInput vIn)
{
	VSOutput pixOut;

	//Position
	float4 pos4 = float4(vIn.vPos.xy, 1, 1);
	pixOut.vPos = pos4;
	pixOut.localPos = vIn.vPos;
	pixOut.uv = vIn.vTexCoord0;
	return pixOut;
}