#pragma once
#include"player.h"
#include"scene.h"
#include"manager.h"
#include"StateMachine.h"

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

class PlayerWalk :public State<Player>
{
private:
	Scene* scene;  //���݂̃V�[��
	Player* PlayerObj;  //�v���C���[
	const char* AnimName = "Walk";  	//�A�j���[�V�����̖��O

	float WarkSpeed = 0.1f;  //��������

public:
	PlayerWalk(Player* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

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

class PlayerAttack :public State<Player>
{
private:
	Scene* scene;  //���݂̃V�[��
	Player* PlayerObj;  //�v���C���[
	const char* AnimName = "Attack";  	//�A�j���[�V�����̖��O
	int Frame = 45;

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
		AddChildTransition<PlayerIdle, PlayerWalk>(Trigger::Towalk);
		AddChildTransition<PlayerIdle, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerIdle, PlayerAttack>(Trigger::ToAttack);

		//Walk��Ԃ���̈ڍs
		AddChildTransition<PlayerWalk, PlayerIdle>(Trigger::ToIdle);
		AddChildTransition<PlayerWalk, PlayerRun>(Trigger::ToRun);
		AddChildTransition<PlayerWalk, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerIdle, PlayerAttack>(Trigger::ToAttack);

		//Run��Ԃ���̈ڍs
		AddChildTransition<PlayerRun, PlayerIdle>(Trigger::ToIdle);
		AddChildTransition<PlayerRun, PlayerWalk>(Trigger::Towalk);
		AddChildTransition<PlayerRun, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerIdle, PlayerAttack>(Trigger::ToAttack);

		//Jump��Ԃ���̈ڍs
		AddChildTransition<PlayerJump, PlayerFall>(Trigger::ToFall);

		//Fall��Ԃ���̈ڍs
		AddChildTransition<PlayerFall, PlayerIdle>(Trigger::ToIdle);
		
		//Attack��Ԃ���̈ڍs
		AddChildTransition<PlayerAttack, PlayerIdle>(Trigger::ToIdle);

		SetCurrentState<PlayerIdle>();
	}

	void Enter()override {};
	void Update() override {};
	void Exit()override {};
};







