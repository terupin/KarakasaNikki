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

	DirectX::SimpleMath::Vector3		m_Velocity;  //落ちていくときの加速度
	float groundHeight = 0.0f;	//地面への接地
protected:
	//ステート管理クラス
	class StateMachine<Player>* m_StateMachine;

public:

	Capsule PlayerCol;  //当たり判定
	bool attack = false;  //攻撃をしているか？?
	DirectX::SimpleMath::Vector3 m_OldPosition; //位置フレーム前にいた場所
	int m_BookNumber = 0; //取らないといけない本の数

	//アニメーション
	float	m_BlendRate;  //ブレンドの％
	int	m_ToFrame;  //フレーム数  
	int m_FromFrame;
	const char* To_Anim = "Idle";  //現在Toのアニメーション
	const char* From_Anim = "Idle";

	//アニメーションの格納
	void Set_ToAnim(const char* Name) { To_Anim = Name; }
	void Set_FromAnim(const char* Name) { From_Anim = Name; }
	void Set_ToFrame(int Frame) { m_ToFrame = Frame;}
	void Set_FromFrame(int Frame) { m_FromFrame = Frame; }

	//動く方向
	DirectX::SimpleMath::Vector3 XAxis;
	DirectX::SimpleMath::Vector3 ZAxis;

	DirectX::SimpleMath::Vector3 GetOldPosition() { return m_OldPosition; }
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }  //加速度の取得
	Model_Load& GetModel() { return *m_Model; }
	void SetVelocity(float SetVel) { m_Velocity.y = SetVel; }

	int m_Move = Trigger::ToIdle;  //現在の動き

	void Init() override;
	void SetState();  //Stateを登録する
	void Update() override;
	void Draw() override;
};
