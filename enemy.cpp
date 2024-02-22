#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "enemy.h"
#include "shader.h"
#include"player.h"
#include"umbrella.h"
#include"EnemyState.h"
#include"AnimationModel.h"
#include"shadow.h"

#include"collision.h"

#include<iostream>

using namespace DirectX::SimpleMath;

void Enemy::Init()
{
	//�V�F�[�_�[�̓ǂݍ���
	AddComponent<Shader>()->Load("shader\\LightingOneskinVS.cso", "shader\\vertexLightingPS.cso");

	//���f���ƃe�N�X�`���̓ǂݍ���
	m_Model = AddComponent<Model_Load>();
	m_UDModel = AddComponent<AnimationModel>();

	m_Model->LoadModel("asset/model/Ninja Idle.fbx");

	//�A�j���[�V�����̓ǂݍ���
	m_Model->LoadAnimation("asset/model/Ninja Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset/model/Sneak Walk.fbx", "Walk");
	m_Model->LoadAnimation("asset/model/Run.fbx", "Run");
	m_Model->LoadAnimation("asset/model/Enemy/Death.fbx", "Death");

	SetPosition(Vector3(3.0f, 0.0f, 7.0f));
	SetRotation(Vector3(0.0f, 180.0f, 0.0f));
	SetScale(Vector3(0.01f, 0.01f, 0.01f));  //�v���C���[�̃T�C�Y�ݒ�

	AddComponent<Shadow>()->SetSize(1.5f);

	//�ǂݍ��񂾃A�j���[�V�������擾����
	m_UDModel->GetAnim(m_Model);

	SetState();
}

void Enemy::SetState()
{
	//�X�e�[�g�}�V���̎擾
	m_StateMachine = AddComponent<StateMachine<Enemy>>();

	//�g�p�����Ԃ̒ǉ�
	m_StateMachine->AddState<EnemyState>();
	m_StateMachine->SetStartState<EnemyState>();
}

void Enemy::Update()
{
	// ���݃V�[�����擾
	Scene* scene = Manager::GetScene();
	Player* PlayerPos = scene->GetGameObject<Player>();
	Umbrella* umbrella = scene->GetGameObject<Umbrella>();

	//�����蔻��̐ݒ�
	SetCapsule(EnemyCol, GetPosition(), GetScale(), m_Model->m_Vertices);
	
	//�X�e�[�g�Ŏ��s�����Ă���
	m_StateMachine->StateUpdate();

	//�A�j���[�V�����t���[��
	m_ToFrame++;
	m_FromFrame++;
	m_BlendRate += 0.1f;

	//�A�j���[�V�����u�����h
	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

}

void Enemy::Draw()
{
	m_UDModel->Update(From_Anim, m_FromFrame, To_Anim, m_ToFrame, m_BlendRate);
}


