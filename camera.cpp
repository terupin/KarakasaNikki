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

	m_Position = Vector3(0.0f, 2.0f, -6.0f);
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
	Vector3 playerpos = playerobj->GetPosition();
	Vector3 playerrot = playerobj->GetRotation();
	
	m_Target = playerobj->GetPosition();//ターゲットをプレイヤーにする

	//カメラ追従がONになったら、プレイヤーの後ろに移動
	if (playerobj->m_Camlock == true)
	{
		float  player_degree = (180 / DirectX::XM_PI) * playerrot.y;  //プレイヤーの回転（度に変換）
		float camera_degree = (180 / DirectX::XM_PI) * m_Rotation.y;  //カメラの回転（度に変換）

		float degree = camera_degree - player_degree;  //プレイヤーを0度とした場合のカメラの回転(単位　度)
		degree = fmod(degree, 360);  //360で割った余りを出す(回転回数を0にする)

		//プレイヤーとカメラの角度の差が1度以上の場合
		if (degree > 1 || degree < -1)
		{
			//角度が正の場合
			if (degree > 0)
			{
				//角度が180よりも大きい場合(0度の時には何もしないため)
				if (degree >= 180)
				{
					m_Rotation.y += DirectX::XM_PI * 0.01f;
				}
				else if (degree < 180)
				{
					m_Rotation.y -= DirectX::XM_PI * 0.01f;
				}
			}
			//角度が負の場合
			if (degree < 0)
			{
				//角度が-180よりも小さい場合(0度の時には何もしないため)
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
		//プレイヤーとの角度の差が1度未満の場合
		else
			SetRotation(playerrot);  //プレイヤーの後ろに移動
	}
	
	//カメラの追従OFFの場合
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

	//追従オフの時にポジションを固定する
	m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_TargetLength;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_TargetLength;
}

void Camera::Draw()
{
	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up);										

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}