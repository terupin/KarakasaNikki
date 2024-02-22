#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;

void PlayerRun::Enter()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//From���̃A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;

	PlayerObj->m_Move = Trigger::ToRun;
}

void PlayerRun::Update()
{
	scene = Manager::GetScene();   //���݂̃V�[���擾
	PlayerObj = scene->GetGameObject<Player>();	//�v���C���[�̏����擾
	Camera* cameraobj = scene->GetGameObject<Camera>();//�J�����̏����擾

	//�v���C���[�̍s����擾
	if (cameraobj->GetCamlock())
	{
		Matrix viewmtx = cameraobj->GetViewMatrix();
		PlayerObj->ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
		PlayerObj->XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);
	}

	//�v���C���[�̃|�W�V�������擾
	Vector3 m_Pos = PlayerObj->GetPosition();

	//�X�e�B�b�N�̓|���̒l���擾
	short int Stick_X = Input::GetPadstick_Left_X();
	short int Stick_Y = Input::GetPadstick_Left_Y();


	if (Input::GetKeyPress('W') || Stick_Y > 0) { m_Pos += PlayerObj->ZAxis * RunSpeed; }
	else if (Input::GetKeyPress('S') || Stick_Y < 0)
	{
		m_Pos -= PlayerObj->ZAxis * RunSpeed;
		PlayerObj->m_FromFrame -= 2.0f;  //�������ɕ�������
	}

	if (Input::GetKeyPress('A') || Stick_X < 0) { m_Pos -= PlayerObj->XAxis * RunSpeed; }
	else if (Input::GetKeyPress('D') || Stick_X > 0) { m_Pos += PlayerObj->XAxis * RunSpeed; }


	//�|�W�V�������i�[����
	PlayerObj->SetPosition(m_Pos);

	//�U���A�N�V����
	if (Input::GetKeyTrigger('K') || Input::GetPadButtonTrigger(XINPUT_GAMEPAD_X))
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToAttack);
		return;
	}

	//�����ɖ߂�
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
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//From���̃A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(0);
	PlayerObj->m_BlendRate = 0.0f;
}