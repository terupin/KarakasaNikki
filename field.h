#pragma once
#include	<d3d11.h>
#include	<SimpleMath.h>
#include	"gameObject.h"

class Field : public GameObject
{

private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};