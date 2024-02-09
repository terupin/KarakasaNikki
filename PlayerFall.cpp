#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;

void PlayerFall::Enter()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//描画したいアニメーションとフレームのセット
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 1.0f;
}

void PlayerFall::Update()
{
	scene = Manager::GetScene();   //現在のシーン取得
	PlayerObj = scene->GetGameObject<Player>();	//プレイヤーの情報を取得
	Camera* cameraobj = scene->GetGameObject<Camera>();//カメラの情報を取得

	Vector3 Pos = PlayerObj->GetPosition();
	Vector3 OldPos = PlayerObj->GetOldPosition();

	Pos += PlayerObj->GetVelocity();

	PlayerObj->SetPosition(Pos);

	if (Pos.y >= OldPos.y)
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToIdle);

}

void PlayerFall::Exit()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//To側のアニメーションにセット
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(23);
}
