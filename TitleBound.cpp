//#include "TitleBound.h"
//#include"shader.h"
//
//using namespace DirectX::SimpleMath;
//
//void TitleBound::Init()
//{
//	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
//
//	//モデルとテクスチャの読み込み
//	m_Model = AddComponent<Model_Load>();
//	m_Model->LoadStaticModel("asset/model/Open.fbx");
//	m_Model->LoadTex("asset/model/WAGASA_RED.png");
//
//	
//	m_Ver = -0.01f;
//	SetScale(Vector3(0.5f, 0.5f, 0.5f));
//	SetPosition(Vector3(-2.0f, 5.0f, 5.0f));
//}
//
//void TitleBound::Update()
//{
//	m_Ver -= 0.01;
//
//	if (m_Position.y <= 1.0f)
//	{
//		
//		m_Ver = 0.5f-0.01f*count;
//		count++;
//	}
//
//
//	m_Position.y += m_Ver;
//
//
//
//}
