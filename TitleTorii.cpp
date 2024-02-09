#include "TitleTorii.h"
#include"shader.h"
#include "manager.h"
#include"Model_Load.h"

using namespace DirectX::SimpleMath;

void TitleTorii::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

	m_Model = AddComponent<Model_Load>();
	m_Model->LoadStaticModel("asset/model/Object/zinja.fbx");

	SetPosition(Vector3(-1.0f, 1.0f, 2.0f));
//	SetScale(Vector3(0.1, 0.1, 0.1));


}
