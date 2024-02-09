#include "umbrella.h"
#include"shader.h"
#include "scene.h"
#include"manager.h"
#include"camera.h"
#include"player.h"
#include"enemy.h"
#include"Model_Load.h"

#include"input.h"

#include<iostream>

using namespace DirectX::SimpleMath;

void Umbrella::Init()
{

    AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
   
    //���f���ƃe�N�X�`���̓ǂݍ���
    m_Model = AddComponent<Model_Load>();
    m_UDModel = AddComponent<AnimationModel>();
    m_Model->LoadModel ("asset/model/close.fbx");
    m_Model->LoadTex("asset/model/WAGASA_RED.png");

    //�A�j���[�V�����̓ǂݍ���
    m_Model->LoadAnimation("asset/model/Open.fbx", "Open");
    m_Model->LoadAnimation("asset/model/close.fbx", "Close");

   this->m_Rotation = Vector3(2.4f, 0.0f, 0.0f);

    //�ǂݍ��񂾃A�j���[�V�������擾����
    m_UDModel->GetAnim(m_Model);

    SetScale(Vector3(1.7, 1.7, 1.7));

}

void Umbrella::Update()
{
    Scene* scene = Manager::GetScene();  //�V�[���̎擾
    m_Player = scene->GetGameObject<Player>();  //�v���C���[�̎擾

    //�����蔻��̐ݒ�
    SetCapsule(UnbCol,GetPosition(), GetScale(), m_Model->m_Vertices);

    //��̃|�W�V������������
    m_AnimPlayer = m_Player->GetComponent<Model_Load>()->m_AiScene;  //�v���C���[�̃A�j���[�V�������f���̏��
    m_RightHandBone = m_Player->m_Model->m_Bone[m_HandBoneName];  //�v���C���[�̉E��̃{�[�����

    //��̕ϊ��s��~��̋t�{�[���I�t�Z�b�g�s��
    aiMatrix4x4 rootMatrix = m_RightHandBone.Matrix*m_RightHandBone.OffsetMatrix.Inverse();
   

    //�v���C���[�̃��[���h�s����擾
    m_WorldMatrix = m_Player->GetWorldMatrix();  
    
    RightHandMatrix = AnimationModel::ChangeMatrix(RightHandMatrix, rootMatrix);  //aiMatrix��Matrix�ɕϊ�
    RightHandMatrix.Decompose(m_Rightscale, m_Rightrot, m_Rightpos);
    RightHandMatrix *= m_WorldMatrix;
    RightHandMatrix.Decompose(m_Worldscale, m_Worldrot, m_Worldpos);  //�s��𕪉�
    
    
    //�����N�H�[�^�j�I���ɂ���
    //s_rot.x = -s_rot.x;
    //s_rot.y = -s_rot.y;
    //s_rot.z = -s_rot.z;

    s_rot = m_Rightrot.ToEuler();//�N�H�[�^�j�I�����I�C���[�p�ɕϊ�

    //���
    SetPosition(m_Worldpos);
    SetRotation({ s_rot.y,s_rot.x,s_rot.z });  //�X�̌v�Z����������

}
