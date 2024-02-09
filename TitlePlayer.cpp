#include "TitlePlayer.h"
#include "shader.h"
#include "renderer.h"

#include"shadow.h"

using namespace DirectX::SimpleMath;

void TitlePlayer::Init()
{
	//シェーダーの読み込み
	AddComponent<Shader>()->Load("shader\\LightingOneskinVS.cso", "shader\\vertexLightingPS.cso");

	//モデルとテクスチャの読み込み
	m_Model = AddComponent<Model_Load>();
	m_UDModel = AddComponent<AnimationModel>();
	
	m_Model->LoadTex("asset/model/Player/ALLTEX.png");
	m_Model->LoadModel("asset/model/Player/T-Pose.fbx");
	
	//椅子に座っているアニメーション
	m_Model->LoadAnimation("asset/model/Player/Sitting.fbx", "Sit");

	AddComponent<Shadow>()->SetSize(1.5f);//影の大きさの設定

	SetPosition(Vector3(1.0f, 0.0f, 3.0f));
	SetScale(Vector3(1.3f, 1.3f, 1.3f));  //プレイヤーのサイズ設定
	SetRotation(Vector3(0.0f, -1.0f, 0.0f));
	//読み込んだアニメーションを取得する
	m_UDModel->GetAnim(m_Model);
}

void TitlePlayer::Update()
{
	//アニメーションフレーム
	m_ToFrame++;
}

void TitlePlayer::Draw()
{
	//座るアニメーションしかないため
	m_UDModel->Update(To_Anim, m_ToFrame, To_Anim, m_ToFrame,0);
}
