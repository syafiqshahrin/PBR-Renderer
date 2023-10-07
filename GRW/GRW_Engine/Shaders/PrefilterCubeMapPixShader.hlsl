
struct VSOutput
{
	float4 vPos : SV_POSITION;
	float3 localPos : LPOSITION;
};

cbuffer Cbuffer : register(b1)
{
	matrix MViewProj;
}

TextureCube genCubeMap : register(t3);
SamplerState samplerTest: register(s0);

static const float PI = 3.14159265f;



float4 main(VSOutput pIN) : SV_TARGET
{
	float3 SampleDirection = normalize(pIN.localPos);
	//float3 SampledColor = genCubeMap.Sample(samplerTest, SampleDirection);
	float3 IrradianceColor = float3(0, 0, 0);

	float3 Up = float3(0, 1, 0);
	float3 Right = normalize(cross(Up, SampleDirection));
	Up = normalize(cross(SampleDirection, Right));

	float sampleDelta = 0.005;
	float nrSamples = 0.0;
	for (float p = 0.0; p < 2.0 * PI; p += sampleDelta)
	{
		for (float t = 0.0; t < 0.5 * PI; t += sampleDelta)
		{
			float3 tangentDir = float3(sin(t) * cos(p), sin(t) * sin(p), cos(t));
			float3 worldDir = (tangentDir.x * Right) + (tangentDir.y * Up) + (tangentDir.z * SampleDirection);
			IrradianceColor += genCubeMap.Sample(samplerTest, worldDir).rgb * cos(t) * sin(t) ;
			nrSamples+= 1.0;
		}
	}
	IrradianceColor = PI * IrradianceColor * (1.0 / float(nrSamples));
	IrradianceColor = pow(IrradianceColor.rgb, (1.0 / 2.2));
	return float4(IrradianceColor.rgb, 1);
}

