#pragma once
#include "gameObject.h"
#include"collision.h"
#include"StateMachine.h"


class Enemy : public GameObject
{

private:
	float Eye_Radian = 0.0f;
	bool watch = false;


protected:
	//ステート管理クラス
	class StateMachine<Enemy>* m_StateMachine;

public:
	class Model_Load* m_Model;
	class AnimationModel* m_UDModel;

	//アニメーション
	int	m_ToFrame;
	int m_FromFrame;
	float	m_BlendRate;
	const char* To_Anim = "Idle";
	const char* From_Anim = "Idle";

	void Set_ToAnim(const char* Name) { To_Anim = Name; }
	void Set_FromAnim(const char* Name) { From_Anim = Name; }
	void Set_ToFrame(int Frame) { m_ToFrame = Frame; }
	void Set_FromFrame(int Frame) { m_FromFrame = Frame; }

	Capsule EnemyCol;

	void Init() override;
	void SetState();//Stateを登録する
	void Update() override;
	void Draw() override;

};