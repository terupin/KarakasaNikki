#pragma once
#include "gameObject.h"
#include"sprite.h"

struct Logo
{
	float posx;
	float posy;
	float width;
	float height;
};

class TitleLogo : public GameObject
{
private:
	VERTEX_3D TitleVertex[4];
	Sprite* sprite;
public:

	void Init();
	void Update();

};

