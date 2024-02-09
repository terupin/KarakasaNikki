#pragma once
#include"player.h"
#include"scene.h"
#include"manager.h"
#include"StateMachine.h"

//Idle状態の時
class PlayerIdle :public State<Player>
{
private:
	Scene* scene;  //現在のシーン
	Player* PlayerObj;  //プレイヤー
	const char* AnimName = "Idle";  //名前

public:
	PlayerIdle(Player* context) :State(context) {}  //ステートの登録
	void Enter() override;
	void Update() override;
	void Exit() override;

};

class PlayerWalk :public State<Player>
{
private:
	Scene* scene;  //現在のシーン
	Player* PlayerObj;  //プレイヤー
	const char* AnimName = "Walk";  	//アニメーションの名前

	float WarkSpeed = 0.1f;  //歩く速さ

public:
	PlayerWalk(Player* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

class PlayerRun :public State<Player>
{
private:
	Scene* scene;  //現在のシーン
	Player* PlayerObj;  //プレイヤー
	const char* AnimName = "Run";  	//アニメーションの名前

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
	Scene* scene;  //現在のシーン
	Player* PlayerObj;  //プレイヤー
	const char* AnimName = "Walk";  	//アニメーションの名前
	int Frame = 9;

	float WarkSpeed = 0.1f;  //歩く速さ
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
	Scene* scene;  //現在のシーン
	Player* PlayerObj;  //プレイヤー
	const char* AnimName = "Fall";  	//アニメーションの名前
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
	Scene* scene;  //現在のシーン
	Player* PlayerObj;  //プレイヤー
	const char* AnimName = "Attack";  	//アニメーションの名前
	int Frame = 45;

public:
	PlayerAttack(Player* context) :State(context) {};
	void Enter() override;
	void Update() override;
	void Exit() override;
};

//プレイヤーの基底ステート
class PlayerState :public State<Player>
{
private:

public:
	PlayerState(Player* context) :State(context)
	{
		//プレイヤーのState一覧
		AddChildState<PlayerIdle>();
		AddChildState<PlayerWalk>();
		AddChildState<PlayerRun>();
		AddChildState<PlayerJump>();
		AddChildState<PlayerFall>();
		AddChildState<PlayerAttack>();

		//Idle状態からの移行
		AddChildTransition<PlayerIdle, PlayerWalk>(Trigger::Towalk);
		AddChildTransition<PlayerIdle, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerIdle, PlayerAttack>(Trigger::ToAttack);

		//Walk状態からの移行
		AddChildTransition<PlayerWalk, PlayerIdle>(Trigger::ToIdle);
		AddChildTransition<PlayerWalk, PlayerRun>(Trigger::ToRun);
		AddChildTransition<PlayerWalk, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerIdle, PlayerAttack>(Trigger::ToAttack);

		//Run状態からの移行
		AddChildTransition<PlayerRun, PlayerIdle>(Trigger::ToIdle);
		AddChildTransition<PlayerRun, PlayerWalk>(Trigger::Towalk);
		AddChildTransition<PlayerRun, PlayerJump>(Trigger::ToJump);
		AddChildTransition<PlayerIdle, PlayerAttack>(Trigger::ToAttack);

		//Jump状態からの移行
		AddChildTransition<PlayerJump, PlayerFall>(Trigger::ToFall);

		//Fall状態からの移行
		AddChildTransition<PlayerFall, PlayerIdle>(Trigger::ToIdle);
		
		//Attack状態からの移行
		AddChildTransition<PlayerAttack, PlayerIdle>(Trigger::ToIdle);

		SetCurrentState<PlayerIdle>();
	}

	void Enter()override {};
	void Update() override {};
	void Exit()override {};
};







