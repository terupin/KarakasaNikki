#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;

void PlayerAttack::Enter()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();


	//�`�悵�����A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;

	if (PlayerObj->m_Move == Trigger::ToRun)
		FromRun = true;

	PlayerObj->attack = true;
	PlayerObj->m_Move = Trigger::ToAttack;
}

void PlayerAttack::Update()
{
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();
	m_UmbrellaObj = scene->GetGameObject<Umbrella>();

	if (FromRun)
	{
		Vector3 m_Pos = PlayerObj->GetPosition();	//�v���C���[�̃|�W�V�������擾
		m_Pos += PlayerObj->ZAxis * m_Inertia;
		PlayerObj->SetPosition(m_Pos);	//�|�W�V�������i�[����
	}

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "�P��X��]" << m_UmbrellaObj->GetRotation().x << std::endl;
	std::cout << "�P��Y��]" << m_UmbrellaObj->GetRotation().y << std::endl;
	std::cout << "�P��Z��]" << m_UmbrellaObj->GetRotation().z << std::endl;
#endif//defined(DEBUG) || defined(_DEBUG)


	if (PlayerObj->m_ToFrame > Frame)
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToIdle);
	}
}

void PlayerAttack::Exit()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//To���̃A�j���[�V�����ɃZ�b�g
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(0);
	PlayerObj->attack = false;

	FromRun = false;
}
