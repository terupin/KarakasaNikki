#include "TitleLogo.h"
#include"shader.h"
#include"sprite.h"

using namespace DirectX::SimpleMath;

void TitleLogo::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(30.0f, 440.0f, 439, 163, "asset\\texture\\Title.png");
	AddComponent<Sprite>()->Init(60.0f, 560.0f, 340, 59, "asset\\texture\\Press.png");


}

void TitleLogo::Update()
{
}
