#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "player.h"
#include "input.h"
#include "audio.h"
#include "shadow.h"
#include "goal.h"
#include "collision.h"
#include "camera.h"

#include"game.h"
#include"PlayerState.h"

#include<iostream>

using namespace DirectX::SimpleMath;

void Player::Init()
{
	//�V�F�[�_�[�̓ǂݍ���
	AddComponent<Shader>()->Load("shader\\LightingOneskinVS.cso", "shader\\vertexLightingPS.cso");

	//���f���ƃe�N�X�`���̓ǂݍ���
	m_Model = AddComponent<Model_Load>();
	m_UDModel = AddComponent<AnimationModel>();

	m_Model->LoadModel("asset/model/Player/T-Pose.fbx");
	m_Model->LoadTex("asset/model/Player/ALLTEX.png");

	//�A�j���[�V�����̓ǂݍ���
	m_Model->LoadAnimation("asset/model/Player/Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset/model/Player/Walk.fbx", "Walk");
	m_Model->LoadAnimation("asset/model/Player/Run.fbx", "Run");
	m_Model->LoadAnimation("asset/model/Player/Idle_Jump57.fbx", "Jump");
	m_Model->LoadAnimation("asset/model/Player/Falling.fbx", "Fall");
	m_Model->LoadAnimation("asset/model/Player/Attack_45.fbx", "Attack");

	AddComponent<Shadow>()->SetSize(1.5f);//�e�̑傫���̐ݒ�

	m_SE = AddComponent<Audio>();
	m_SE->Load("asset\\audio\\wan.wav");

	 SetScale(Vector3(1.3f, 1.3f, 1.3f));  //�v���C���[�̃T�C�Y�ݒ�

	 //�ǂݍ��񂾃A�j���[�V�������擾����
	 m_UDModel->GetAnim(m_Model);

	 SetState();

}

void Player::SetState()
{
	//�X�e�[�g�}�V���̎擾
	m_StateMachine = AddComponent<StateMachine<Player>>();

	//�g�p�����Ԃ̒ǉ�
	m_StateMachine->AddState<PlayerState>();
	m_StateMachine->SetStartState<PlayerState>();

}

void Player::Update()
{
	m_OldPosition = m_Position;  //�O�̈ʒu���擾

	Scene* scene = Manager::GetScene();   //���݂̃V�[���擾
	Camera* cameraobj = scene->GetGameObject<Camera>();//�J�����̏����擾
	bool hit_static = false;
	bool hit_book = false;

	//�����蔻��̍X�V
	SetCapsule(PlayerCol,GetPosition(), GetScale(), m_Model->m_Vertices);

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Camlock = !m_Camlock;
	}

	//�J�����Ǐ]
	if (Input::GetKeyPress(VK_LEFT)&&m_Camlock)
	{
		m_Rotation.y += DirectX::XM_PI * 0.01f;

	}
	if (Input::GetKeyPress(VK_RIGHT)&&m_Camlock)
	{
		m_Rotation.y -= DirectX::XM_PI * 0.01f;
	}


	//�X�e�[�g�Ŏ��s�����Ă���
	m_StateMachine->StateUpdate();
	
	//�ÓI�I�u�W�F�N�g�̓����蔻��
	for (auto* s :scene->List_Static)
	{		//�����蔻��
		hit_static = HitCapsuleAABB_xz(PlayerCol, *s, m_Position);
			if (hit_static)
			{
				//��ɍڂ��Ă��Ȃ��ꍇ
				m_Position.x = m_OldPosition.x;
				m_Position.z = m_OldPosition.z;
			}
	}

	//�A�j���[�V�����t���[��
	m_ToFrame++;
	m_FromFrame++;
	m_BlendRate += 0.1f;

	//�A�j���[�V�����u�����h
	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

	//�d��
	m_Velocity.y -= 0.015;

	//�ړ�
	m_Position += m_Velocity;

	// �ʒu���O�ȉ��Ȃ�n�ʈʒu�ɃZ�b�g����
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	if (Input::GetKeyTrigger('R'))
	{
		std::cout << "�v���C���[��X��]" << m_Rotation.x << std::endl;
		std::cout << "�v���C���[��Y��]" << m_Rotation.y << std::endl;
		std::cout << "�v���C���[��Z��]" << m_Rotation.z << std::endl;
		std::cout << "�J������X��]" << cameraobj->GetRotation().x << std::endl;
		std::cout << "�J������Y��]" << cameraobj->GetRotation().y << std::endl;
		std::cout << "�J������Z��]" << cameraobj->GetRotation().z << std::endl;
	}

}

void Player::Draw()
{
	m_UDModel->Update(From_Anim, m_FromFrame, To_Anim, m_ToFrame, m_BlendRate);
}
