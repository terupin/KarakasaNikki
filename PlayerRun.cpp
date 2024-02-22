#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;

void PlayerRun::Enter()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//From側のアニメーションとフレームのセット
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;

	PlayerObj->m_Move = Trigger::ToRun;
}

void PlayerRun::Update()
{
	scene = Manager::GetScene();   //現在のシーン取得
	PlayerObj = scene->GetGameObject<Player>();	//プレイヤーの情報を取得
	Camera* cameraobj = scene->GetGameObject<Camera>();//カメラの情報を取得

	//プレイヤーの行列を取得
	if (cameraobj->GetCamlock())
	{
		Matrix viewmtx = cameraobj->GetViewMatrix();
		PlayerObj->ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
		PlayerObj->XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);
	}

	//プレイヤーのポジションを取得
	Vector3 m_Pos = PlayerObj->GetPosition();

	//スティックの倒れ具合の値を取得
	short int Stick_X = Input::GetPadstick_Left_X();
	short int Stick_Y = Input::GetPadstick_Left_Y();


	if (Input::GetKeyPress('W') || Stick_Y > 0) { m_Pos += PlayerObj->ZAxis * RunSpeed; }
	else if (Input::GetKeyPress('S') || Stick_Y < 0)
	{
		m_Pos -= PlayerObj->ZAxis * RunSpeed;
		PlayerObj->m_FromFrame -= 2.0f;  //後ろ向きに歩くため
	}

	if (Input::GetKeyPress('A') || Stick_X < 0) { m_Pos -= PlayerObj->XAxis * RunSpeed; }
	else if (Input::GetKeyPress('D') || Stick_X > 0) { m_Pos += PlayerObj->XAxis * RunSpeed; }


	//ポジションを格納する
	PlayerObj->SetPosition(m_Pos);

	//攻撃アクション
	if (Input::GetKeyTrigger('K') || Input::GetPadButtonTrigger(XINPUT_GAMEPAD_X))
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToAttack);
		return;
	}

	//歩きに戻る
	if (Input::GetPadTriggerUp_Left())
	{
		if (!Input::GetKeyPress('P'))
		{
			PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToWalk);
		}
	}
	else if (m_Pos == PlayerObj->m_OldPosition)
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToIdle);
	}


}

void PlayerRun::Exit()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//From側のアニメーションとフレームのセット
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(0);
	PlayerObj->m_BlendRate = 0.0f;
}