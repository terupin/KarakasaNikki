#pragma once
#include"enemy.h"
#include"player.h"
#include"scene.h"
#include"manager.h"
#include"StateMachine.h"

//Idle状態の時
class EnemyIdle :public State<Enemy>
{
private:
	Scene* scene;  //現在のシーン
	Enemy* EnemyObj;  //プレイヤーオブジェ
	const char* AnimName = "Idle";//名前
public:
	EnemyIdle(Enemy* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

class EnemyWalk :public State<Enemy>
{
private:
	Scene* scene;  //現在のシーン
	Enemy* EnemyObj;  //プレイヤーオブジェ
	const char* AnimName = "Idle";//名前
public:
	EnemyWalk(Enemy* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

class EnemyAttack :public State<Enemy>
{
private:
	Scene* scene;  //現在のシーン
	Enemy* EnemyObj;  //プレイヤーオブジェ
	const char* AnimName = "Idle";//名前
public:
	EnemyAttack(Enemy* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

class EnemyDeath :public State<Enemy>
{
private:
	Scene* scene;  //現在のシーン
	Enemy* EnemyObj;  //プレイヤーオブジェ
	const char* AnimName = "Death";//名前
public:
	EnemyDeath(Enemy* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;

};






//敵の基本ステート
class EnemyState :public State<Enemy>
{
private:

public:
	EnemyState(Enemy* context) :State(context)
	{
		AddChildState<EnemyIdle>();
		AddChildState<EnemyDeath>();

		//Idle状態からの移行
		AddChildTransition<EnemyIdle, EnemyDeath>(Trigger::ToDeath);


		SetCurrentState<EnemyIdle>();
	}


	void Enter()override {};
	void Update() override {};
	void Exit()override {};

};