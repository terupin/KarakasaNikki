#include "TitleSky.h"
#include"manager.h"
#include "shader.h"
#include"scene.h"
#include"TitleCamera.h"
#include"Model_Load.h"


using namespace DirectX::SimpleMath;

void TitleSky::Init()
{
	m_Scale = Vector3(100.0f, 100.0f, 100.0f);
	m_Model = AddComponent<Model_Load>();

	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");

	m_Model->LoadTex("asset\\texture\\sky.jpg");
	m_Model->LoadStaticModel("asset\\model\\sky.fbx");

}

void TitleSky::Update()
{

	Scene* scene = Manager::GetScene();
	TitleCamera* camera = scene->GetGameObject<TitleCamera>();

	Vector3 cameraPosition = camera->GetPosition();
	m_Position = cameraPosition;
}
