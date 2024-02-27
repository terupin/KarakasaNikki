#include "ResultLogo.h"
#include"shader.h"
#include"sprite.h"

using namespace DirectX::SimpleMath;

void ResultLogo::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(30.0f, 140.0f, 768, 166, "asset\\texture\\Clear.png");
	AddComponent<Sprite>()->Init(60.0f, 560.0f, 340, 59, "asset\\texture\\Press.png");


}

void ResultLogo::Update()
{
}
