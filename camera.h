#pragma once

#include	<SimpleMath.h>
#include	"gameObject.h"

class Camera : public GameObject{
private:

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};

	float m_TargetLength;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	DirectX::SimpleMath::Matrix GetViewMatrix() 
	{ 
		return m_ViewMatrix;
	}
};