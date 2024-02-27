#pragma once
#include "camera.h"
class ResultCamera :    public Camera
{
private:
	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw() override;
};

