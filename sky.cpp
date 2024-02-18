#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "modelRenderer.h"
#include "sky.h"
#include "camera.h"
#include "shader.h"
#include"Model_Load.h"

using namespace DirectX::SimpleMath;

void Sky::Init()
{
	m_Scale = Vector3(100.0f, 100.0f, 100.0f);
	m_Model = AddComponent<Model_Load>();

	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	
	m_Model->LoadTex("asset\\model\\sky.jpg");
	m_Model->LoadStaticModel("asset\\model\\sky.fbx");
	

}


void Sky::Update()
{

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	Vector3 cameraPosition = camera->GetPosition();
	m_Position = cameraPosition;

}


