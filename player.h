#pragma once
#include "gameObject.h"
#include"Model_Load.h"
#include"AnimationModel.h"
#include"StateMachine.h"
#include"collision.h"

class Player : public GameObject
{
private:
	class Audio* m_SE{};
	class Model_Load* m_Model;
	class AnimationModel* m_UDModel;

	DirectX::SimpleMath::Vector3		m_Velocity;  //�����Ă����Ƃ��̉����x
	float groundHeight = 0.0f;	//�n�ʂւ̐ڒn
protected:
	//�X�e�[�g�Ǘ��N���X
	class StateMachine<Player>* m_StateMachine;

public:

	Capsule PlayerCol;  //�����蔻��
	bool attack = false;  //�U�������Ă��邩�H?
	DirectX::SimpleMath::Vector3 m_OldPosition; //�ʒu�t���[���O�ɂ����ꏊ
	int m_BookNumber = 0; //���Ȃ��Ƃ����Ȃ��{�̐�

	//�A�j���[�V����
	float	m_BlendRate;  //�u�����h�́�
	int	m_ToFrame;  //�t���[����  
	int m_FromFrame;
	const char* To_Anim = "Idle";  //����To�̃A�j���[�V����
	const char* From_Anim = "Idle";

	//�A�j���[�V�����̊i�[
	void Set_ToAnim(const char* Name) { To_Anim = Name; }
	void Set_FromAnim(const char* Name) { From_Anim = Name; }
	void Set_ToFrame(int Frame) { m_ToFrame = Frame;}
	void Set_FromFrame(int Frame) { m_FromFrame = Frame; }

	//��������
	DirectX::SimpleMath::Vector3 XAxis;
	DirectX::SimpleMath::Vector3 ZAxis;

	DirectX::SimpleMath::Vector3 GetOldPosition() { return m_OldPosition; }
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }  //�����x�̎擾
	Model_Load& GetModel() { return *m_Model; }
	void SetVelocity(float SetVel) { m_Velocity.y = SetVel; }

	int m_Move = Trigger::ToIdle;  //���݂̓���

	void Init() override;
	void SetState();  //State��o�^����
	void Update() override;
	void Draw() override;
};
