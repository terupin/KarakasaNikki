#pragma once

#include	<SimpleMath.h>
#include	"gameObject.h"

class Camera : public GameObject{
private:

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};
	bool m_Camlock = true;  //�J�����̒Ǐ]
	float m_TargetLength;

public:
	bool GetCamlock() { return m_Camlock; }
	void SetCamlock(bool lock) { m_Camlock = lock; }

	void Init();
	void Uninit();
	void Update();
	void Draw();
	DirectX::SimpleMath::Matrix GetViewMatrix() 
	{ 
		return m_ViewMatrix;
	}
};