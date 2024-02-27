#include "main.h"
#include "manager.h"

#include "input.h"

#include "title.h"

#include "game.h"

#include "shader.h"
#include "sprite.h"

#include "transition.h"

#include "result.h"

#include"field.h"
#include"ResultSky.h"
#include"ResultCamera.h"
#include"ResultPlayer.h"
#include"ResultLogo.h"

void Result::Init()
{
	AddGameObject<ResultLogo>(3);


	AddGameObject<ResultCamera>(0);
	AddGameObject<ResultSky>(1);
	AddGameObject<Field>(1);
	AddGameObject<ResultPlayer>(1);

	m_Transition = AddGameObject<Transition>(3);					// 3�̓��C���ԍ�
	m_Transition->FadeIn();
}


void Result::Update()
{
	//  ��ʑJ�ځi�t�F�[�h�C���j���I�����Ă���Ȃ�
	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetKeyTrigger(VK_RETURN) || Input::GetPadButtonTrigger(XINPUT_GAMEPAD_A))
		{
			m_Transition->FadeOut();
		}
	}

	// ��ʑJ��(�t�F�[�h�A�E�g)���I�����Ă邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Title>();
	}
}
