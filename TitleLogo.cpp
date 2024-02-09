#include "TitleLogo.h"
#include"shader.h"
#include"sprite.h"

using namespace DirectX::SimpleMath;

void TitleLogo::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(30.0f, 440.0f, 438, 162, "asset\\texture\\Title.png");


}

void TitleLogo::Update()
{




}
