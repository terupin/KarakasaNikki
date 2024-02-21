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
	DirectX::SimpleMath::Vector3		m_Velocity;  //落ちていくときの加速度
	class Audio*	m_SE{};
	float groundHeight = 0.0f;	//地面への接地

protected:
	//ステート管理クラス
	class StateMachine<Player>* m_StateMachine;

public:
	class Model_Load* m_Model;
	class AnimationModel* m_UDModel;

	bool attack = false;  //攻撃をしているか？?
	
	//位置フレーム前にいた場所
	DirectX::SimpleMath::Vector3 m_OldPosition;

	//アニメーション
	int	m_ToFrame;  //フレーム数  
	int m_FromFrame;
	float	m_BlendRate;  //ブレンドの％
	const char* To_Anim="Idle";  //現在Toのアニメーション
	const char* From_Anim="Idle";

	//アニメーションの格納
	void Set_ToAnim(const char* Name) { To_Anim = Name; }
	void Set_FromAnim(const char* Name) { From_Anim = Name; }
	void Set_ToFrame(int Frame) { m_ToFrame = Frame;}
	void Set_FromFrame(int Frame) { m_FromFrame = Frame; }

	DirectX::SimpleMath::Vector3 GetOldPosition() { return m_OldPosition; }

	//加速度
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }
	void SetVelocity(float SetVel) { m_Velocity.y = SetVel; }

	//カプセル
	Capsule PlayerCol;

	bool m_Camlock = false;  //カメラの追従
	int m_BookNumber = 0;  //取らないといけない本の数


	void SetState();  //Stateを登録する

	void Init() override;
	void Update() override;
	void Draw() override;

};
