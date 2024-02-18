#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;

void PlayerAttack::Enter()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();


	//描画したいアニメーションとフレームのセット
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;

	PlayerObj->attack = true;
}

void PlayerAttack::Update()
{
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();
	m_UmbrellaObj = scene->GetGameObject<Umbrella>();


	std::cout << "傘のX回転" << m_UmbrellaObj->GetRotation().x << std::endl;
	std::cout << "傘のY回転" << m_UmbrellaObj->GetRotation().y << std::endl;
	std::cout << "傘のZ回転" << m_UmbrellaObj->GetRotation().z << std::endl;
	
	if (PlayerObj->m_ToFrame > Frame)
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToIdle);
	}

}

void PlayerAttack::Exit()
{
	//シーンとプレイヤーの情報を取得する
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//To側のアニメーションにセット
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(0);
	PlayerObj->attack = false;
}
