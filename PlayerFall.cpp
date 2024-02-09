#include "PlayerState.h"
#include"input.h"
#include"camera.h"

using namespace DirectX::SimpleMath;

void PlayerFall::Enter()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//�`�悵�����A�j���[�V�����ƃt���[���̃Z�b�g
	PlayerObj->Set_ToAnim(AnimName);
	PlayerObj->Set_ToFrame(0);
	PlayerObj->m_BlendRate = 1.0f;
}

void PlayerFall::Update()
{
	scene = Manager::GetScene();   //���݂̃V�[���擾
	PlayerObj = scene->GetGameObject<Player>();	//�v���C���[�̏����擾
	Camera* cameraobj = scene->GetGameObject<Camera>();//�J�����̏����擾

	Vector3 Pos = PlayerObj->GetPosition();
	Vector3 OldPos = PlayerObj->GetOldPosition();

	Pos += PlayerObj->GetVelocity();

	PlayerObj->SetPosition(Pos);

	if (Pos.y >= OldPos.y)
		PlayerObj->GetComponent<StateMachine<Player>>()->SendTrigger(Trigger::ToIdle);

}

void PlayerFall::Exit()
{
	//�V�[���ƃv���C���[�̏����擾����
	scene = Manager::GetScene();
	PlayerObj = scene->GetGameObject<Player>();

	//To���̃A�j���[�V�����ɃZ�b�g
	PlayerObj->Set_FromAnim(AnimName);
	PlayerObj->Set_FromFrame(23);
}
