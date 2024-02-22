#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "enemy.h"
#include "shader.h"
#include"player.h"
#include"umbrella.h"
#include"EnemyState.h"
#include"AnimationModel.h"
#include"shadow.h"

#include"collision.h"

#include<iostream>

using namespace DirectX::SimpleMath;

void Enemy::Init()
{
	//シェーダーの読み込み
	AddComponent<Shader>()->Load("shader\\LightingOneskinVS.cso", "shader\\vertexLightingPS.cso");

	//モデルとテクスチャの読み込み
	m_Model = AddComponent<Model_Load>();
	m_UDModel = AddComponent<AnimationModel>();

	m_Model->LoadModel("asset/model/Ninja Idle.fbx");

	//アニメーションの読み込み
	m_Model->LoadAnimation("asset/model/Ninja Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset/model/Sneak Walk.fbx", "Walk");
	m_Model->LoadAnimation("asset/model/Run.fbx", "Run");
	m_Model->LoadAnimation("asset/model/Enemy/Death.fbx", "Death");

	SetPosition(Vector3(3.0f, 0.0f, 7.0f));
	SetRotation(Vector3(0.0f, 180.0f, 0.0f));
	SetScale(Vector3(0.01f, 0.01f, 0.01f));  //プレイヤーのサイズ設定

	AddComponent<Shadow>()->SetSize(1.5f);

	//読み込んだアニメーションを取得する
	m_UDModel->GetAnim(m_Model);

	SetState();
}

void Enemy::SetState()
{
	//ステートマシンの取得
	m_StateMachine = AddComponent<StateMachine<Enemy>>();

	//使用する状態の追加
	m_StateMachine->AddState<EnemyState>();
	m_StateMachine->SetStartState<EnemyState>();
}

void Enemy::Update()
{
	// 現在シーンを取得
	Scene* scene = Manager::GetScene();
	Player* PlayerPos = scene->GetGameObject<Player>();
	Umbrella* umbrella = scene->GetGameObject<Umbrella>();

	//当たり判定の設定
	SetCapsule(EnemyCol, GetPosition(), GetScale(), m_Model->m_Vertices);
	
	//ステートで実行をしている
	m_StateMachine->StateUpdate();

	//アニメーションフレーム
	m_ToFrame++;
	m_FromFrame++;
	m_BlendRate += 0.1f;

	//アニメーションブレンド
	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

}

void Enemy::Draw()
{
	m_UDModel->Update(From_Anim, m_FromFrame, To_Anim, m_ToFrame, m_BlendRate);
}


