#include "PlayerState.h"
#include"input.h"
#include"camera.h"
#include "shadow.h"

using namespace DirectX::SimpleMath;

void PlayerIdle::Enter()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//�`�悵�����A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;

	PlayerObj->m_Move = Trigger::ToIdle;
}

void PlayerIdle::Update()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();
	Camera* cameraobj = scene->GetGameObject<Camera>();//�J�����̏����擾

	PlayerObj->GetComponent<Shadow>();

	//�v���C���[�������ꍇ
	if (Input::GetKeyPress('W') ||
		Input::GetKeyPress('A') ||
		Input::GetKeyPress('S') ||
		Input::GetKeyPress('D') ||
		Input::GetPadstick_Left_X() != 0 ||  //�X�e�B�b�N�������Ă��Ȃ�
		Input::GetPadstick_Left_Y() != 0)
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToWalk);
		return;
	}

	//�W�����v�A�N�V����
	if (Input::GetKeyTrigger('J'))
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToJump);
		return;
	}

	//�U���A�N�V����
	if (Input::GetKeyTrigger('K')||Input::GetPadButtonTrigger(XINPUT_GAMEPAD_X))
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToAttack);
		return;
	}
}

void PlayerIdle::Exit()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//To���̃A�j���[�V�����ɃZ�b�g
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(0);
}
