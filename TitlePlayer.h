#pragma once
#include "player.h"
#include"Model_Load.h"
#include"AnimationModel.h"

class TitlePlayer : public Player
{
private:
	class Model_Load* m_Model;
	class AnimationModel* m_UDModel;

	int	m_ToFrame;  //�t���[����  
	const char* To_Anim = "Sit";  //����To�̃A�j���[�V����

public:
	void Init() override;
	void Update() override;
	void Draw() override;
};

