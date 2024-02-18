#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "player.h"
#include "input.h"
#include "audio.h"
#include "shadow.h"
#include "goal.h"
#include "collision.h"
#include "camera.h"

#include"game.h"
#include"PlayerState.h"

#include<iostream>

using namespace DirectX::SimpleMath;

void Player::Init()
{
	//シェーダーの読み込み
	AddComponent<Shader>()->Load("shader\\LightingOneskinVS.cso", "shader\\vertexLightingPS.cso");

	//モデルとテクスチャの読み込み
	m_Model = AddComponent<Model_Load>();
	m_UDModel = AddComponent<AnimationModel>();

	m_Model->LoadModel("asset/model/Player/T-Pose.fbx");
	m_Model->LoadTex("asset/model/Player/ALLTEX.png");

	//アニメーションの読み込み
	m_Model->LoadAnimation("asset/model/Player/Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset/model/Player/Walk.fbx", "Walk");
	m_Model->LoadAnimation("asset/model/Player/Run.fbx", "Run");
	m_Model->LoadAnimation("asset/model/Player/Idle_Jump57.fbx", "Jump");
	m_Model->LoadAnimation("asset/model/Player/Falling.fbx", "Fall");
	m_Model->LoadAnimation("asset/model/Player/Attack_45.fbx", "Attack");

	AddComponent<Shadow>()->SetSize(1.5f);//影の大きさの設定

	m_SE = AddComponent<Audio>();
	m_SE->Load("asset\\audio\\wan.wav");

	 SetScale(Vector3(1.3f, 1.3f, 1.3f));  //プレイヤーのサイズ設定

	 //読み込んだアニメーションを取得する
	 m_UDModel->GetAnim(m_Model);

	 SetState();

}

void Player::SetState()
{
	//ステートマシンの取得
	m_StateMachine = AddComponent<StateMachine<Player>>();

	//使用する状態の追加
	m_StateMachine->AddState<PlayerState>();
	m_StateMachine->SetStartState<PlayerState>();

}

void Player::Update()
{
	m_OldPosition = m_Position;  //前の位置を取得

	Scene* scene = Manager::GetScene();   //現在のシーン取得
	Camera* cameraobj = scene->GetGameObject<Camera>();//カメラの情報を取得
	bool hit_static = false;
	bool hit_book = false;

	//当たり判定の更新
	SetCapsule(PlayerCol,GetPosition(), GetScale(), m_Model->m_Vertices);

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Camlock = !m_Camlock;
	}

	//カメラ追従
	if (Input::GetKeyPress(VK_LEFT)&&m_Camlock)
	{
		m_Rotation.y += DirectX::XM_PI * 0.01f;

	}
	if (Input::GetKeyPress(VK_RIGHT)&&m_Camlock)
	{
		m_Rotation.y -= DirectX::XM_PI * 0.01f;
	}


	//ステートで実行をしている
	m_StateMachine->StateUpdate();
	
	//静的オブジェクトの当たり判定
	for (auto* s :scene->List_Static)
	{		//当たり判定
		hit_static = HitCapsuleAABB_xz(PlayerCol, *s, m_Position);
			if (hit_static)
			{
				//上に載っていない場合
				m_Position.x = m_OldPosition.x;
				m_Position.z = m_OldPosition.z;
			}
	}

	//アニメーションフレーム
	m_ToFrame++;
	m_FromFrame++;
	m_BlendRate += 0.1f;

	//アニメーションブレンド
	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

	//重力
	m_Velocity.y -= 0.015;

	//移動
	m_Position += m_Velocity;

	// 位置が０以下なら地面位置にセットする
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	if (Input::GetKeyTrigger('R'))
	{
		std::cout << "プレイヤーのX回転" << m_Rotation.x << std::endl;
		std::cout << "プレイヤーのY回転" << m_Rotation.y << std::endl;
		std::cout << "プレイヤーのZ回転" << m_Rotation.z << std::endl;
		std::cout << "カメラのX回転" << cameraobj->GetRotation().x << std::endl;
		std::cout << "カメラのY回転" << cameraobj->GetRotation().y << std::endl;
		std::cout << "カメラのZ回転" << cameraobj->GetRotation().z << std::endl;
	}

}

void Player::Draw()
{
	m_UDModel->Update(From_Anim, m_FromFrame, To_Anim, m_ToFrame, m_BlendRate);
}
