#include "Book.h"
#include"shader.h"
#include"Model_Load.h"
#include"manager.h"
#include"player.h"

using namespace DirectX::SimpleMath; 

void Book::Init()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

	m_Model = AddComponent<Model_Load>();
	m_Model->LoadTex("asset/model/Object/Book_Color.png");
	m_Model->LoadStaticModel("asset/model/Object/Book.fbx");

	SetRotation(Vector3(1.5f, 0.0f, 0.0f));
	SetScale(Vector3(0.01f, 0.01f, 0.01f));

	player->m_BookNumber++;  //取らないといけない本の数を足す

}

void Book::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	Vector3 Rot = GetRotation();

	Rot.y+=0.01f;

	SetRotation(Rot);

	hit_player = HitCapsuleAABB_xz(player->PlayerCol, GetAABB(), player->GetPosition());
	
	if(hit_player)
	{
		player->m_BookNumber--;
		this->SetDestroy();	
	}
}


