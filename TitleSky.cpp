#include "TitleSky.h"
#include"manager.h"
#include "shader.h"
#include"modelRenderer.h"
#include"scene.h"
#include"TitleCamera.h"
#include"Model_Load.h"


using namespace DirectX::SimpleMath;

void TitleSky::Init()
{
	m_Scale = Vector3(100.0f, 100.0f, 100.0f);


	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\sky.obj");

}

void TitleSky::Update()
{

	Scene* scene = Manager::GetScene();
	TitleCamera* camera = scene->GetGameObject<TitleCamera>();

	Vector3 cameraPosition = camera->GetPosition();
	m_Position = cameraPosition;
}
