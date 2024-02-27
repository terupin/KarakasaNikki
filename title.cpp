#include "main.h"
#include "manager.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "transition.h"
#include "field.h"
#include"TitleCamera.h"
#include "TitleSky.h"
#include"TitlePlayer.h"
#include"TitleBench.h"
#include"TitleLogo.h"

using namespace DirectX::SimpleMath;

void Title::Init()
{
	AddGameObject<TitleLogo>(3);			// 3�̓��C���ԍ�

	AddGameObject<TitleCamera>(0);
	AddGameObject<TitleSky>(1);
	AddGameObject<Field>(1);
	AddGameObject<TitlePlayer>(1);
	AddGameObject<TitleBench>(1);


	m_Transition = AddGameObject<Transition>(3);					// 3�̓��C���ԍ�
	m_Transition->FadeIn();
}


void Title::Update()
{
	// 
	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetKeyTrigger(VK_RETURN)||Input::GetPadButtonTrigger(XINPUT_GAMEPAD_A))
		{
			m_Transition->FadeOut();
		}
	}

	// ��ʑJ�ڂ��I�����Ă邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Game>();
	}
}

