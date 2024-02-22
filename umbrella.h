#pragma once
#include<SimpleMath.h>
#include "gameObject.h"
#include"AnimationModel.h"
#include"player.h"
#include"collision.h"
#include"renderer.h"

class Player;

class Umbrella :public GameObject
{
private:
    //���f���̓ǂݍ���
    class Model_Load* m_Model;
    class AnimationModel* m_UDModel;
    
    Player* m_Player;//�v���C���[���
    BONE m_RightHandBone;  //�E��̃{�[�����

    std::string m_HandBoneName= "mixamorig:RightHand";  //�E��̃{�[����

    const aiScene* m_AnimPlayer;  //�v���C���[�̃A�j���[�V�������f�����
    aiMatrix4x4 m_RightHandMatrix;//�E��̃{�[�����

    DirectX::SimpleMath::Matrix m_WorldMatrix;
    DirectX::SimpleMath::Matrix RightHandMatrix;

    //���[���h�s��𕪉������Ƃ��ɑ�����镨
    DirectX::SimpleMath::Vector3 m_Worldscale;
    DirectX::SimpleMath::Vector3 m_Worldpos;
    DirectX::SimpleMath::Quaternion m_Worldrot;

    //�E��̃{�[���̍s��𕪉������Ƃ��ɑ������
    DirectX::SimpleMath::Vector3 m_Rightscale;
    DirectX::SimpleMath::Vector3 m_Rightpos;
    DirectX::SimpleMath::Quaternion m_Rightrot;

    //�N�H�[�^�j�I���̊p�x���I�C���[�p�ɕς������Ɋi�[����ꏊ
    DirectX::SimpleMath::Vector3 s_rot;
     
public:
    //�����蔻��
    Capsule UnbCol;

    void Init() override;
    void Update() override;

};