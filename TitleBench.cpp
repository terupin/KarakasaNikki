#include "TitleBench.h"
#include "shader.h"
#include"Model_Load.h"
#include"scene.h"
#include "manager.h"

using namespace DirectX::SimpleMath;

void TitleBench::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

	//モデルとテクスチャの読み込み
	m_Model = AddComponent<Model_Load>();
	m_Model->LoadTex("asset/model/Bench_2.jpg");
	m_Model->LoadStaticModel("asset/model/bench.obj");
	

	SetPosition(Vector3(1.0f, 0.27f, 3.0f));
	SetRotation(Vector3(0.0f, -1.0f, 0.0f));
	SetScale(Vector3(0.01f, 0.01f, 0.01f));

}
