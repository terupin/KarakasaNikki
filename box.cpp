#include "main.h"
#include "manager.h"
#include "renderer.h"
#include"Model_Load.h"
#include "modelRenderer.h"
#include "box.h"
#include "shader.h"
#include"shadow.h"

void Box::Init()
{
	Scene* scene = Manager::GetScene();
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

	//モデルとテクスチャの読み込み
	m_Model = AddComponent<Model_Load>();
	m_Model->LoadStaticModel("asset/model/box.obj");
	m_Model->LoadTex("asset/texture/paper.jpg");

	SetPosition({ -2.0f,0.0f,-2.0f });
	SetAABB(m_Box, GetPosition(), GetScale(), m_Model->m_Vertices);
	scene->List_Static.emplace_back(&m_Box);

}
