#pragma once
#include"enemy.h"
#include"player.h"
#include"scene.h"
#include"manager.h"
#include"StateMachine.h"

//Idle��Ԃ̎�
class EnemyIdle :public State<Enemy>
{
private:
	Scene* scene;  //���݂̃V�[��
	Enemy* EnemyObj;  //�v���C���[�I�u�W�F
	const char* AnimName = "Idle";//���O
public:
	EnemyIdle(Enemy* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

class EnemyWalk :public State<Enemy>
{
private:
	Scene* scene;  //���݂̃V�[��
	Enemy* EnemyObj;  //�v���C���[�I�u�W�F
	const char* AnimName = "Idle";//���O
public:
	EnemyWalk(Enemy* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

class EnemyAttack :public State<Enemy>
{
private:
	Scene* scene;  //���݂̃V�[��
	Enemy* EnemyObj;  //�v���C���[�I�u�W�F
	const char* AnimName = "Idle";//���O
public:
	EnemyAttack(Enemy* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

class EnemyDeath :public State<Enemy>
{
private:
	Scene* scene;  //���݂̃V�[��
	Enemy* EnemyObj;  //�v���C���[�I�u�W�F
	const char* AnimName = "Death";//���O
public:
	EnemyDeath(Enemy* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;

};






//�G�̊�{�X�e�[�g
class EnemyState :public State<Enemy>
{
private:

public:
	EnemyState(Enemy* context) :State(context)
	{
		AddChildState<EnemyIdle>();
		AddChildState<EnemyDeath>();

		//Idle��Ԃ���̈ڍs
		AddChildTransition<EnemyIdle, EnemyDeath>(Trigger::ToDeath);


		SetCurrentState<EnemyIdle>();
	}


	void Enter()override {};
	void Update() override {};
	void Exit()override {};

};