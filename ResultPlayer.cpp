#include "ResultPlayer.h"
#include "shader.h"
#include "renderer.h"
#include"shadow.h"

using namespace DirectX::SimpleMath;

void ResultPlayer::Init()
{
	//�V�F�[�_�[�̓ǂݍ���
	AddComponent<Shader>()->Load("shader\\LightingOneskinVS.cso", "shader\\vertexLightingPS.cso");

	//���f���ƃe�N�X�`���̓ǂݍ���
	m_Model = AddComponent<Model_Load>();
	m_UDModel = AddComponent<AnimationModel>();

	m_Model->LoadTex("asset/model/Player/ALLTEX.png");
	m_Model->LoadModel("asset/model/Player/T-Pose.fbx");

	//�֎q�ɍ����Ă���A�j���[�V����
	m_Model->LoadAnimation("asset/model/Player/goal.fbx", "goal");

	AddComponent<Shadow>()->SetSize(1.5f);//�e�̑傫���̐ݒ�

	SetPosition(Vector3(1.0f, 0.0f, 3.0f));
	SetScale(Vector3(1.3f, 1.3f, 1.3f));  //�v���C���[�̃T�C�Y�ݒ�
	SetRotation(Vector3(0.0f, 3.0f, 0.0f));
	//�ǂݍ��񂾃A�j���[�V�������擾����
	m_UDModel->GetAnim(m_Model);

	AddComponent<Shadow>()->SetSize(1.5f);//�e�̑傫���̐ݒ�

}

void ResultPlayer::Update()
{
	//�A�j���[�V�����t���[��
	m_ToFrame++;
}

void ResultPlayer::Draw()
{
	//�A�j���[�V������������Ȃ�����
	m_UDModel->Update(To_Anim, m_ToFrame, To_Anim, m_ToFrame, 0);
}
