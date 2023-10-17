cbuffer FrameCBuffer : register(b0)
{
	float4 time;
	matrix MCam; //Mview
	matrix MViewProj;
	float4 DirectionaLightWS;
	float4 DirectionaLightColor;
	float4 CamWS;
	float4 PLightPos;
	float4 PLightCol;
}

cbuffer ObjectCBuffer : register(b1)
{
	matrix MVP;
	matrix MW;
	matrix MNorm;
}