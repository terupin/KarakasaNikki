#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;


void PlayerWalk::Enter()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();
	Camera* cameraobj = scene->GetGameObject<Camera>();//カメラの情報を取得

	//From側のアニメーションとフレームのセット
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;

	//カメラの追従がOFFの場合
	if (!cameraobj->GetCamlock()&&PlayerObj->m_Move!=Trigger::ToRun)
	{
		//プレイヤーを最初の移動するときにだけ表面を向く
		PlayerObj->SetRotation(cameraobj->GetRotation());

		//そのあとは移動方向を固定する
		Matrix viewmtx = cameraobj->GetViewMatrix();
		PlayerObj->ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
		PlayerObj->XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);
	}

	PlayerObj->m_Move = Trigger::ToWalk;
}

void PlayerWalk::Update()
{
	scene = Manager::GetScene();   //現在のシーン取得
	PlayerObj = scene->GetGameObject<Player>();	//プレイヤーの情報を取得
	Camera* cameraobj = scene->GetGameObject<Camera>();//カメラの情報を取得
	Vector3 m_Pos = PlayerObj->GetPosition();	//プレイヤーのポジションを取得

	//カメラの追従がONの場合
	//移動中にONにする可能性もあるため
	if (cameraobj->GetCamlock())
	{
		//移動方向をカメラから見た時にする
		Matrix viewmtx = cameraobj->GetViewMatrix();
		PlayerObj->ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
		PlayerObj->XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);
	}

	//スティックの倒れ具合の値を取得
	short int Stick_X = Input::GetPadstick_Left_X();
	short int Stick_Y = Input::GetPadstick_Left_Y();

	//移動をしている
	{
		if (Input::GetKeyPress('W') || Stick_Y > 0)
		{
			m_Pos += PlayerObj->ZAxis * WarkSpeed;
		}
		else if (Input::GetKeyPress('S') || Stick_Y < 0)
		{
			m_Pos -= PlayerObj->ZAxis * WarkSpeed;
			PlayerObj->m_FromFrame -= 2.0f;  //後ろ向きに歩くため
		}

		if (Input::GetKeyPress('A') || Stick_X < 0)
		{
			m_Pos -= PlayerObj->XAxis * WarkSpeed;
		}
		if (Input::GetKeyPress('D') || Stick_X > 0)
		{
			m_Pos += PlayerObj->XAxis * WarkSpeed;

		}
	}
	PlayerObj->SetPosition(m_Pos);//ポジションを格納する

	//攻撃アクション
	if (Input::GetKeyTrigger('K') || Input::GetPadButtonTrigger(XINPUT_GAMEPAD_X))
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToAttack);
		return;
	}

	//走っている場合
	if (Input::GetKeyPress('P') || Input::Input::GetPadTrigger_Left())
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToRun);

	//歩いていない場合
	if (m_Pos == PlayerObj->m_OldPosition)
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToIdle);
}

void PlayerWalk::Exit()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//From側のアニメーションとフレームのセット
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(0);
	PlayerObj->m_BlendRate = 0.0f;
}