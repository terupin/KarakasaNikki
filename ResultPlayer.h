#pragma once
#include "player.h"
class ResultPlayer :	public Player
{
private:
	class Model_Load* m_Model;
	class AnimationModel* m_UDModel;

	int	m_ToFrame;  //�t���[����  
	const char* To_Anim = "goal";  //����To�̃A�j���[�V����

public:
	void Init() override;
	void Update() override;
	void Draw() override;
};

