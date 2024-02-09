#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include"input.h"

using namespace DirectX::SimpleMath;

void Camera::Init()
{
	//カメラと注視点の距離を求める

	m_Position = Vector3(0.0f, 5.0f, -6.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);

	m_TargetLength = (m_Position - m_Target).Length();

}

void Camera::Uninit()
{
}

void Camera::Update()
{
	//シーンとプレイヤーの情報を取得
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
	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up);										// 左手系にした　20230511 by suzuki.tomoki

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki

	Renderer::SetProjectionMatrix(&projectionMatrix);
}