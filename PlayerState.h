#pragma once
#include"player.h"
#include"scene.h"
#include"manager.h"
#include"umbrella.h"
#include"StateMachine.h"

#include<iostream>

//Idle��Ԃ̎�
class PlayerIdle :public State<Player>
{
private:
	Scene* scene;  //���݂̃V�[��
	Player* PlayerObj;  //�v���C���[
	const char* AnimName = "Idle";  //���O

public:
	PlayerIdle(Player* context) :State(context) {}  //�X�e�[�g�̓o�^
	void Enter() override;
	void Update() override;
	void Exit() override;

};

//�����Ă�����
class PlayerWalk :public State<Player>
{
private:
	Scene* scene;  //���݂̃V�[��
	Player* PlayerObj;  //�v���C���[
	const char* AnimName = "Walk";  	//�A�j���[�V�����̖��O

	float WarkSpeed = 0.05f;  //��������

public:
	PlayerWalk(Player* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

//�����Ă�����
class PlayerRun :public State<Player>
{
private:
	Scene* scene;  //���݂̃V�[��
	Player* PlayerObj;  //�v���C���[
	const char* AnimName = "Run";  	//�A�j���[�V�����̖��O

	float RunSpeed = 0.2f;

public:
	PlayerRun(Player* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

//�W�����v���
class PlayerJump :public State<Player>
{
private:
	Scene* scene;  //���݂̃V�[��
	Player* PlayerObj;  //�v���C���[
	const char* AnimName = "Walk";  	//�A�j���[�V�����̖��O
	int Frame = 9;

	float WarkSpeed = 0.1f;  //��������
	float JumpMettr = 0.2f;

public:
	PlayerJump(Player* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;

};

//�����Ă�����
class PlayerFall :public State<Player>
{
private:
	Scene* scene;  //���݂̃V�[��
	Player* PlayerObj;  //�v���C���[
	const char* AnimName = "Fall";  	//�A�j���[�V�����̖��O
	int Frame = 9;

public:
	PlayerFall(Player* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

//�U�����
class PlayerAttack :public State<Player>
{
private:
	Scene* scene;  //���݂̃V�[��
	Player* PlayerObj;  //�v���C���[
	Umbrella* m_UmbrellaObj;  //�P
	const char* AnimName = "Attack";  	//�A�j���[�V�����̖��O
	int Frame =45;  //�A�j���[�V�����̃t���[����

	bool FromRun = false;
	float m_Inertia = 0.01f;  //�����̗�

public:
	PlayerAttack(Player* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

//�v���C���[�̊��X�e�[�g
class PlayerState :public State<Player>
{
private:

public:
	PlayerState(Player* context) :State(context)
	{
		//�v���C���[��State�ꗗ
		AddChildState<PlayerIdle>();
		AddChildState<PlayerWalk>();
		AddChildState<PlayerRun>();
		AddChildState<PlayerJump>();
		AddChildState<PlayerFall>();
		AddChildState<PlayerAttack>();

		//Idle��Ԃ���̈ڍs
		AddChildTransition<PlayerIdle, PlayerWalk>(Trigger::ToWalk);
		AddChildTransition<PlayerIdle, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerIdle, PlayerAttack>(Trigger::ToAttack);

		//Walk��Ԃ���̈ڍs
		AddChildTransition<PlayerWalk, PlayerIdle>(Trigger::ToIdle);
		AddChildTransition<PlayerWalk, PlayerRun>(Trigger::ToRun);
		AddChildTransition<PlayerWalk, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerWalk, PlayerAttack>(Trigger::ToAttack);

		//Run��Ԃ���̈ڍs
		AddChildTransition<PlayerRun, PlayerIdle>(Trigger::ToIdle);
		AddChildTransition<PlayerRun, PlayerWalk>(Trigger::ToWalk);
		AddChildTransition<PlayerRun, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerRun, PlayerAttack>(Trigger::ToAttack);

		//Jump��Ԃ���̈ڍs
		AddChildTransition<PlayerJump, PlayerFall>(Trigger::ToFall);

		//Fall��Ԃ���̈ڍs
		AddChildTransition<PlayerFall, PlayerIdle>(Trigger::ToIdle);
		
		//Attack��Ԃ���̈ڍs
		AddChildTransition<PlayerAttack, PlayerIdle>(Trigger::ToIdle);

		SetCurrentState<PlayerIdle>();  //�ŏ��̃X�e�[�g�ݒ�
	}
	void Enter()override {};
	void Update() override {};
	void Exit()override {};
};







