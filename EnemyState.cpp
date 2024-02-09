#include "EnemyState.h"
#include"umbrella.h"

using namespace DirectX::SimpleMath;


void EnemyIdle::Enter()
{
	//�V�[���ƓG�̏����擾����
	scene = Manager::GetScene();
	EnemyObj = scene->GetGameObject<Enemy>();

	//From���̃A�j���[�V�����ƃt���[���̃Z�b�g
	EnemyObj->Set_ToAnim(AnimName);
	EnemyObj->Set_ToFrame(0);
	EnemyObj->m_BlendRate = 0.0f;
}

void EnemyIdle::Update()
{
	//�V�[���ƓG�̏����擾����
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
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	EnemyObj = scene->GetGameObject<Enemy>();

	//To���̃A�j���[�V�����ɃZ�b�g
	EnemyObj->Set_FromAnim(AnimName);
	EnemyObj->Set_FromFrame(0);
}

void EnemyDeath::Enter()
{	//�V�[���ƓG�̏����擾����
	scene = Manager::GetScene();
	EnemyObj = scene->GetGameObject<Enemy>();

	//From���̃A�j���[�V�����ƃt���[���̃Z�b�g
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
