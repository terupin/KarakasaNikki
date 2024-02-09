#include "common.hlsl"

void main(in VSONESKIN_IN In,out PS_IN Out)
{
    //�����X�L�����_�u�����h�̏���
    float4x4 comb = (float4x4) 0;
    for (int i = 0; i < 4; i++)
    {
       //�d�݂��v�Z���Ȃ���s�񐶐�
        comb += BoneMatrix[In.BoneIndex[i]] * In.BoneWeight[i];
    }
    
    float4 Pos;
    Pos = mul(comb,In.Position);
    In.Position = Pos;
    
    //�@���x�N�g����␳
    float4 Normal;
    Normal = float4(In.Normal.xyz, 0.0);
    
    //�ړ�����������
    comb._41 = 0.0f;
    comb._42 = 0.0f;
    comb._43 = 0.0f;
    comb._44 = 1.0f;
    
    Normal = mul(comb, In.Normal);
    normalize(Normal);
    In.Normal = Normal;
    
    //���[���h�ϊ��s��𔽉f������
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0f);
    
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    
    float light = -dot(Light.Direction.xyz, worldNormal.xyz) * 0.5f + 0.5f;
    light = saturate(light);
    
    Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
    Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
    Out.Diffuse += Material.Emission;
    Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

    Out.Position = mul(In.Position, wvp);
    Out.TexCoord = In.TexCoord;
}





