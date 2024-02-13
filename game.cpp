#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "camera.h"
#include "field.h"
#include "player.h"
#include "box.h"
#include "cylinder.h"

#include "game.h"
#include "input.h"
#include "audio.h"

#include "manager.h"
#include "transition.h"

#include "result.h"

#include "goal.h"

#include "utility.h"

#include "bullet.h"

#include "battery.h"

#include "sky.h"

#include "score.h"

#include "explosion.h"

#include"umbrella.h"

#include"TitleBench.h"
#include"TitleTorii.h"

using namespace DirectX::SimpleMath;

// �Q�[����������
void Game::Init()
{

	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<Umbrella>(1);
	AddGameObject<Player>(1);

	AddGameObject<Enemy>(1);
	TitleBench* test = AddGameObject<TitleBench>(1);
	test->SetRotation({ 0.0f, 0.0f, 0.0f });

	Box* boxtest = AddGameObject<Box>(1);
	boxtest->SetPosition({ -2.0f,0.0f,-2.0f });

	for (int i = 0; i < 3; i++)
	{
		Book* bookobj = AddGameObject<Book>(1);
		bookobj->SetPosition(Vector3(0.0f, 1.0f, 1.0f+i));
	}

	// �`�F�b�N����
	{
		Goal* goal = AddGameObject<Goal>(1);
		goal->SetPosition(Vector3(11.0f, 1.0f, 20.0f));
		goal->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	}

	// ��ʑJ�ڃI�u�W�F�N�g��o�^
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();
}

// �Q�[���I������
void Game::Uninit()
{
	//List_Book.clear();

}

// �Q�[���X�V����
void Game::Update()
{
	// �t�F�[�h�C�����I�����Ă��邩�H	
//	if (m_Transition->GetState() == Transition::State::Stop) {
//		if (Input::GetKeyTrigger(VK_RETURN))
//		{
//			m_Transition->FadeOut();
//		}
//	}

	// �S�[�����Ă��Ȃ��̂ł����
	if (!m_Goal)
	{
		Goal* goal = GetGameObject<Goal>();

		// �S�[�������ۂɃS�[���I�u�W�F�N�g�͍폜�����
		if (goal == nullptr)
		{
			m_Goal = true;

			// �Q�b��ɃX���b�h�𐶐����ăt�F�[�h�A�E�g�J�n
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
	}

	// �t�F�[�h�A�E�g���I�����Ă��邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
}
