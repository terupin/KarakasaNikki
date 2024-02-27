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
#include "utility.h"
#include "sky.h"
#include "score.h"
#include"umbrella.h"
#include"Book.h"
#include"enemy.h"

using namespace DirectX::SimpleMath;

// �Q�[����������
void Game::Init()
{
	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<Umbrella>(2);
	AddGameObject<Player>(2);
	AddGameObject<Enemy>(2);

	//��
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition({ 50.0f,0.0f,0.0f });
		box->SetScale({ 1.0f,5.0f,70.0f });
		SetAABB(box->GetAABB(), box->GetPosition(),
			box->GetScale(), box->GetModel().m_Vertices);
	}

	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition({ -50.0f,0.0f,0.0f });
		box->SetScale({ 1.0f,5.0f,70.0f });
		SetAABB(box->GetAABB(), box->GetPosition(),
			box->GetScale(), box->GetModel().m_Vertices);
	}

	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition({ 0.0f,0.0f,50.0f });
		box->SetScale({ 70.0f,5.0f,1.0f });
		SetAABB(box->GetAABB(), box->GetPosition(),
			box->GetScale(), box->GetModel().m_Vertices);
	}

	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition({ 0.0f,0.0f,-50.0f });
		box->SetScale({ 70.0f,5.0f,1.0f });
		SetAABB(box->GetAABB(), box->GetPosition(),
			box->GetScale(), box->GetModel().m_Vertices);
	}

	//�{�̓ǂݍ���(�Q�[���Ŏ��Ȃ��Ƃ����Ȃ����)
	{
		Book* bookobj = AddGameObject<Book>(1);
		bookobj->SetPosition(Vector3(0.0f, 1.0f, 5.0f));
		SetAABB(bookobj->GetAABB(), bookobj->GetPosition(),
			bookobj->GetScale(), bookobj->GetModel().m_Vertices);
	}
	{
		Book* bookobj = AddGameObject<Book>(1);
		bookobj->SetPosition(Vector3(4.0f, 1.0f, 3.0f));
		SetAABB(bookobj->GetAABB(), bookobj->GetPosition(),
			bookobj->GetScale(), bookobj->GetModel().m_Vertices);
	}
	{
		Book* bookobj = AddGameObject<Book>(1);
		bookobj->SetPosition(Vector3(9.0f, 1.0f, 5.0f));
		SetAABB(bookobj->GetAABB(), bookobj->GetPosition(),
			bookobj->GetScale(), bookobj->GetModel().m_Vertices);
	}

	// ��ʑJ�ڃI�u�W�F�N�g��o�^
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();
}

// �Q�[���I������
void Game::Uninit()
{

}

// �Q�[���X�V����
void Game::Update()
{
	Player* player = GetGameObject<Player>();

	if (!m_Goal)
	{
		//�S�Ă̖{�����������?
		if (player->m_BookNumber == 0)
		{
			m_Goal = true;

			// 1�b��ɃX���b�h�𐶐����ăt�F�[�h�A�E�g�J�n
			Invoke([=]() { m_Transition->FadeOut(); }, 1000);
		}
	}

	// �t�F�[�h�A�E�g���I�����Ă��邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
}
