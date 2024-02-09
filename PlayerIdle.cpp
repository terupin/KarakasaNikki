#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;

void PlayerIdle::Enter()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//描画したいアニメーションとフレームのセット
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;
}

void PlayerIdle::Update()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//プレイヤーが歩く場合
	if (Input::GetKeyPress('W') ||
		Input::GetKeyPress('A') ||
		Input::GetKeyPress('S') ||
		Input::GetKeyPress('D') ||
		Input::GetPadstick_Left_X() != 0 ||  //スティックが動いていない
		Input::GetPadstick_Left_Y() != 0)
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::Towalk);
		return;
	}

	//ジャンプアクション
	if (Input::GetKeyTrigger('J'))
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToJump);
		return;
	}

	//攻撃アクション
	if (Input::GetKeyTrigger('K')||Input::GetPadButtonTrigger(XINPUT_GAMEPAD_X))
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToAttack);
		return;
	}
}

void PlayerIdle::Exit()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//To側のアニメーションにセット
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(0);
}
