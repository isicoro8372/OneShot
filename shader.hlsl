

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer ConstantBuffer : register( b0 )
{
	matrix WorldViewProjection;
}

// マテリアルバッファ
cbuffer MaterialBuffer : register( b1 )
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
}

cbuffer ColorBuffer : register( b2 )
{
	float4 color;
}

cbuffer LightBuffer : register( b3 )
{
    float4		LocalVector;
    float4		lightColor;
    float4		ambientColor;
    //float4		Diffsue;
}

//=============================================================================
// 頂点シェーダ
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0 )
{
	outPosition = mul( inPosition, WorldViewProjection );

	outNormal = inNormal;
	outTexCoord = inTexCoord;

	outDiffuse = inDiffuse * Diffuse;
	//outDiffuse = color; //行列の場合は使用しない
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,

						 out float4 outDiffuse		: SV_Target )
{
    //float light = saturate(dot(normalize(inNormal.xyz), -LocalVector.xyz));
	//float4 diffuse = float4(inDiffuse.rgb * lightColor.rgb * light + ambientColor.rgb, inDiffuse.a);
    //outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord) * Diffuse;

    float light = (dot(normalize(inNormal.xyz), -LocalVector.xyz) + 1.0f) * 0.5f;
    float3 diffuse = (inDiffuse.rgb * lightColor.rgb * light + ambientColor.rgb);
    outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord) * float4(diffuse, inDiffuse.a);

    //float4 Diffuse = float4(inDiffuse.rgb + light, inDiffuse.a);
    //float4 Diffuse = float4(inDiffuse.rgb * saturate(dot(inNormal, normalize(LocalVector))), inDiffuse.a);
	//outDiffuse = inDiffuse;
}
