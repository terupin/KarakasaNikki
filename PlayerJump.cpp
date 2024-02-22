#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;

void PlayerJump::Enter()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();
	PlayerObj->SetVelocity(JumpMettr);  //�����x���i�[


	//�`�悵�����A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 0.0f;

	PlayerObj->m_Move = Trigger::ToJump;
}

void PlayerJump::Update()
{

	scene = Manager::GetScene();   //���݂̃V�[���擾
	PlayerObj = scene->GetGameObject<Player>();	//�v���C���[�̏����擾
	Camera* cameraobj = scene->GetGameObject<Camera>();//�J�����̏����擾

	//�v���C���[�̍s����擾
	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	Vector3 OldPos = PlayerObj->GetOldPosition();
	Vector3 m_Pos = PlayerObj->GetPosition();

	if (PlayerObj->m_ToFrame < Frame)
		PlayerObj->m_ToFrame = Frame;

	//�O��̈ړ�
	if (Input::GetKeyPress('W') || Input::GetPadstick_Left_Y() > 0)
	{
		m_Pos += PlayerObj->GetForward() * WarkSpeed;
	}
	else if (Input::GetKeyPress('S') || Input::GetPadstick_Left_Y() < 0)
	{
		m_Pos -= PlayerObj->GetForward() * WarkSpeed;
		PlayerObj->m_FromFrame -= 2.0f;  //�������ɕ�������
	}

	if (Input::GetKeyPress('A') || Input::GetPadstick_Left_X() < 0)
	{
		m_Pos -= XAxis * WarkSpeed;
	}
	else if (Input::GetKeyPress('D') || Input::GetPadstick_Left_X() > 0)
	{
		m_Pos += XAxis * WarkSpeed;
	}

	if (PlayerObj->m_ToFrame >= 23)
	{
		PlayerObj->m_ToFrame = 23;
	}

	//�|�W�V�������i�[����
	PlayerObj->SetPosition(m_Pos);


	if (PlayerObj->GetVelocity().y<0.0f)
	{
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToFall);
	}

}

void PlayerJump::Exit()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//From���̃A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(22);
	PlayerObj->m_BlendRate = 0.0f;
}