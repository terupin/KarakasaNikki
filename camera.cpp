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

	m_Position = Vector3(0.0f, 5.0f, -6.0f);
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

	m_Target = playerobj->GetPosition();

	Vector3 forward = playerobj->GetForward();
	Vector3 playerpos = playerobj->GetPosition();

	this->m_Position.y = playerpos.y+2.0;

	if (Input::GetKeyPress(VK_SPACE))
	{
		this->m_Position = playerpos - forward * 7.0f;
		this->m_Position.y += 2.0;
	}

	if (Input::GetKeyPress(VK_RIGHT))
	{
		m_Rotation.y -= DirectX::XM_PI * 0.01f;
		if (m_Rotation.y < -DirectX::XM_PI)
		{
			m_Rotation.y += DirectX::XM_PI * 2.0f;
		}
	}


	if (Input::GetKeyPress(VK_LEFT))
	{
		m_Rotation.y += DirectX::XM_PI * 0.01f;
		if (m_Rotation.y > -DirectX::XM_PI)
		{
			m_Rotation.y -= DirectX::XM_PI * 2.0f;
		}
	}

	m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_TargetLength;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_TargetLength;


}

void Camera::Draw()
{
	// �r���[�ϊ����쐬
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up);										// ����n�ɂ����@20230511 by suzuki.tomoki

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����s��̐���
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // ����p

	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);	// �A�X�y�N�g��	
	float nearPlane = 1.0f;       // �j�A�N���b�v
	float farPlane = 1000.0f;      // �t�@�[�N���b�v

	//�v���W�F�N�V�����s��̐���
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// ����n�ɂ����@20230511 by suzuki.tomoki

	Renderer::SetProjectionMatrix(&projectionMatrix);
}