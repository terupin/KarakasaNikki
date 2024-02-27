#pragma once
#include "player.h"
class ResultPlayer :	public Player
{
private:
	class Model_Load* m_Model;
	class AnimationModel* m_UDModel;

	int	m_ToFrame;  //フレーム数  
	const char* To_Anim = "goal";  //現在Toのアニメーション

public:
	void Init() override;
	void Update() override;
	void Draw() override;
};

