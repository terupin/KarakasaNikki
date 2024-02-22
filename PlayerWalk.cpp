#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;


void PlayerWalk::Enter()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();
	Camera* cameraobj = scene->GetGameObject<Camera>();//�J�����̏����擾

	//From���̃A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;

	//�J�����̒Ǐ]��OFF�̏ꍇ
	if (!cameraobj->GetCamlock()&&PlayerObj->m_Move!=Trigger::ToRun)
	{
		//�v���C���[���ŏ��̈ړ�����Ƃ��ɂ����\�ʂ�����
		PlayerObj->SetRotation(cameraobj->GetRotation());

		//���̂��Ƃ͈ړ��������Œ肷��
		Matrix viewmtx = cameraobj->GetViewMatrix();
		PlayerObj->ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
		PlayerObj->XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);
	}

	PlayerObj->m_Move = Trigger::ToWalk;
}

void PlayerWalk::Update()
{
	scene = Manager::GetScene();   //���݂̃V�[���擾
	PlayerObj = scene->GetGameObject<Player>();	//�v���C���[�̏����擾
	Camera* cameraobj = scene->GetGameObject<Camera>();//�J�����̏����擾
	Vector3 m_Pos = PlayerObj->GetPosition();	//�v���C���[�̃|�W�V�������擾

	//�J�����̒Ǐ]��ON�̏ꍇ
	//�ړ�����ON�ɂ���\�������邽��
	if (cameraobj->GetCamlock())
	{
		//�ړ��������J�������猩�����ɂ���
		Matrix viewmtx = cameraobj->GetViewMatrix();
		PlayerObj->ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
		PlayerObj->XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);
	}

	//�X�e�B�b�N�̓|���̒l���擾
	short int Stick_X = Input::GetPadstick_Left_X();
	short int Stick_Y = Input::GetPadstick_Left_Y();

	//�ړ������Ă���
	{
		if (Input::GetKeyPress('W') || Stick_Y > 0)
		{
			m_Pos += PlayerObj->ZAxis * WarkSpeed;
		}
		else if (Input::GetKeyPress('S') || Stick_Y < 0)
		{
			m_Pos -= PlayerObj->ZAxis * WarkSpeed;
			PlayerObj->m_FromFrame -= 2.0f;  //�������ɕ�������
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
	PlayerObj->SetPosition(m_Pos);//�|�W�V�������i�[����

	//�U���A�N�V����
	if (Input::GetKeyTrigger('K') || Input::GetPadButtonTrigger(XINPUT_GAMEPAD_X))
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToAttack);
		return;
	}

	//�����Ă���ꍇ
	if (Input::GetKeyPress('P') || Input::Input::GetPadTrigger_Left())
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToRun);

	//�����Ă��Ȃ��ꍇ
	if (m_Pos == PlayerObj->m_OldPosition)
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToIdle);
}

void PlayerWalk::Exit()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//From���̃A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(0);
	PlayerObj->m_BlendRate = 0.0f;
}