#include "EnemyState.h"
#include"umbrella.h"

using namespace DirectX::SimpleMath;


void EnemyIdle::Enter()
{
	//シーンと敵の情報を取得する
	scene = Manager::GetScene();
	EnemyObj = scene->GetGameObject<Enemy>();

	//From側のアニメーションとフレームのセット
	EnemyObj->Set_ToAnim(AnimName);
	EnemyObj->Set_ToFrame(0);
	EnemyObj->m_BlendRate = 0.0f;
}

void EnemyIdle::Update()
{
	//シーンと敵の情報を取得する
	scene = Manager::GetScene();
	Player *PlayerObj = scene->GetGameObject<Player>();
	Umbrella* UmbObj = scene->GetGameObject<Umbrella>();

	EnemyObj = scene->GetGameObject<Enemy>();

	if (PlayerObj->attack)
	{
		if (CapsuleCollision(EnemyObj->EnemyCol, UmbObj->UnbCol))
		{
			EnemyObj->GetComponent<StateMachine<Enemy>>()->SendTrigger(Trigger::ToDeath);
			return;
		}
	}

}

void EnemyIdle::Exit()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	EnemyObj = scene->GetGameObject<Enemy>();

	//To側のアニメーションにセット
	EnemyObj->Set_FromAnim(AnimName);
	EnemyObj->Set_FromFrame(0);
}

void EnemyDeath::Enter()
{	//シーンと敵の情報を取得する
	scene = Manager::GetScene();
	EnemyObj = scene->GetGameObject<Enemy>();

	//From側のアニメーションとフレームのセット
	EnemyObj->Set_ToAnim(AnimName);
	EnemyObj->Set_ToFrame(0);
	EnemyObj->m_BlendRate = 0.0f;
}

void EnemyDeath::Update()
{
	if (EnemyObj->m_FromFrame > 107.9f)
	{
		EnemyObj->m_BlendRate = 100.0f;
		EnemyObj->m_FromFrame = 107.9f;
	}
}

void EnemyDeath::Exit()
{
}
