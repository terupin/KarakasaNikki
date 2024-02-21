#pragma once
#include "gameObject.h"
#include"Model_Load.h"
#include"AnimationModel.h"
#include"StateMachine.h"
#include"collision.h"

#include"enemy.h"
#include"Book.h"

class Player : public GameObject
{
private:
	DirectX::SimpleMath::Vector3		m_Velocity;  //�����Ă����Ƃ��̉����x
	class Audio*	m_SE{};
	float groundHeight = 0.0f;	//�n�ʂւ̐ڒn

protected:
	//�X�e�[�g�Ǘ��N���X
	class StateMachine<Player>* m_StateMachine;

public:
	class Model_Load* m_Model;
	class AnimationModel* m_UDModel;

	bool attack = false;  //�U�������Ă��邩�H?
	
	//�ʒu�t���[���O�ɂ����ꏊ
	DirectX::SimpleMath::Vector3 m_OldPosition;

	//�A�j���[�V����
	int	m_ToFrame;  //�t���[����  
	int m_FromFrame;
	float	m_BlendRate;  //�u�����h�́�
	const char* To_Anim="Idle";  //����To�̃A�j���[�V����
	const char* From_Anim="Idle";

	//�A�j���[�V�����̊i�[
	void Set_ToAnim(const char* Name) { To_Anim = Name; }
	void Set_FromAnim(const char* Name) { From_Anim = Name; }
	void Set_ToFrame(int Frame) { m_ToFrame = Frame;}
	void Set_FromFrame(int Frame) { m_FromFrame = Frame; }

	DirectX::SimpleMath::Vector3 GetOldPosition() { return m_OldPosition; }

	//�����x
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }
	void SetVelocity(float SetVel) { m_Velocity.y = SetVel; }

	//�J�v�Z��
	Capsule PlayerCol;

	bool m_Camlock = false;  //�J�����̒Ǐ]
	int m_BookNumber = 0;  //���Ȃ��Ƃ����Ȃ��{�̐�


	void SetState();  //State��o�^����

	void Init() override;
	void Update() override;
	void Draw() override;

};
