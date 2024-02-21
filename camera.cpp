#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include"input.h"

using namespace DirectX::SimpleMath;

void Camera::Init()
{
	//�J�����ƒ����_�̋��������߂�

	m_Position = Vector3(0.0f, 2.0f, -6.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);

	m_TargetLength = (m_Position - m_Target).Length();

}

void Camera::Uninit()
{
}

void Camera::Update()
{
	//�V�[���ƃv���C���[�̏����擾
	Scene* nowscene = Manager::GetScene();
	Player* playerobj = nowscene->GetGameObject<Player>();
	Vector3 playerpos = playerobj->GetPosition();
	Vector3 playerrot = playerobj->GetRotation();
	
	m_Target = playerobj->GetPosition();//�^�[�Q�b�g���v���C���[�ɂ���

	//�J�����Ǐ]��ON�ɂȂ�����A�v���C���[�̌��Ɉړ�
	if (playerobj->m_Camlock == true)
	{
		float  player_degree = (180 / DirectX::XM_PI) * playerrot.y;  //�v���C���[�̉�]�i�x�ɕϊ��j
		float camera_degree = (180 / DirectX::XM_PI) * m_Rotation.y;  //�J�����̉�]�i�x�ɕϊ��j

		float degree = camera_degree - player_degree;  //�v���C���[��0�x�Ƃ����ꍇ�̃J�����̉�](�P�ʁ@�x)
		degree = fmod(degree, 360);  //360�Ŋ������]����o��(��]�񐔂�0�ɂ���)

		//�v���C���[�ƃJ�����̊p�x�̍���1�x�ȏ�̏ꍇ
		if (degree > 1 || degree < -1)
		{
			//�p�x�����̏ꍇ
			if (degree > 0)
			{
				//�p�x��180�����傫���ꍇ(0�x�̎��ɂ͉������Ȃ�����)
				if (degree >= 180)
				{
					m_Rotation.y += DirectX::XM_PI * 0.01f;
				}
				else if (degree < 180)
				{
					m_Rotation.y -= DirectX::XM_PI * 0.01f;
				}
			}
			//�p�x�����̏ꍇ
			if (degree < 0)
			{
				//�p�x��-180�����������ꍇ(0�x�̎��ɂ͉������Ȃ�����)
				if (degree <= -180)
				{
					m_Rotation.y -= DirectX::XM_PI * 0.01f;
				}
				else if (degree > -180)
				{
					m_Rotation.y += DirectX::XM_PI * 0.01f;
				}
			}
		}
		//�v���C���[�Ƃ̊p�x�̍���1�x�����̏ꍇ
		else
			SetRotation(playerrot);  //�v���C���[�̌��Ɉړ�
	}
	
	//�J�����̒Ǐ]OFF�̏ꍇ
	if (playerobj->m_Camlock == false)
	{
		if (Input::GetKeyPress(VK_RIGHT)||Input::GetPadstick_Right_X()>0)
		{
			m_Rotation.y -= DirectX::XM_PI * 0.01f;
		}

		if (Input::GetKeyPress(VK_LEFT)||Input::GetPadstick_Right_X()<0)
		{
			m_Rotation.y += DirectX::XM_PI * 0.01f;
		}

	}

	//�Ǐ]�I�t�̎��Ƀ|�W�V�������Œ肷��
	m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_TargetLength;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_TargetLength;
}

void Camera::Draw()
{
	// �r���[�ϊ����쐬
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up);										

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����s��̐���
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // ����p

	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);	// �A�X�y�N�g��	
	float nearPlane = 1.0f;       // �j�A�N���b�v
	float farPlane = 1000.0f;      // �t�@�[�N���b�v

	//�v���W�F�N�V�����s��̐���
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}