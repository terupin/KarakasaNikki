#include	"main.h"
#include	"renderer.h"
#include	"manager.h"
#include	"audio.h"
#include	"input.h"
#include	"scene.h"
#include	"game.h"
#include	"title.h"

// �V�[���}�l�[�W���������o�ϐ��̎���
Scene* Manager::m_Scene{};					// ���݃V�[��

void Manager::Init(Application* ap)
{
	// �^�C�}�𑜓x���P�~���b�ɂ���
	timeBeginPeriod(1);

	// �����_���[��������
	Renderer::Init(ap);

	// ��������
	Audio::InitMaster();

	// ���͂�������
	Input::Init();

	// Title�������V�[���ɓo�^
	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->UninitBase();
	delete m_Scene;

	// �R�c���f����S�ĉ��
	ModelRenderer::UnloadAll();

	// �C���v�b�g�I��
	Input::Uninit();

	// �I�[�f�B�I�I��
	Audio::UninitMaster();

	// �^�C�}�𑜓x�����ɖ߂�
	timeEndPeriod(1);
}

void Manager::Update(uint64_t d)
{
	Input::Update();

	m_Scene->UpdateBase();
}

void Manager::Draw(uint64_t d)
{
	Renderer::Begin();

	m_Scene->DrawBase();

	Renderer::End();
}
