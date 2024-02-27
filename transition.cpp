#include <SimpleMath.h>
#include "main.h"
#include "renderer.h"
#include "transition.h"
#include "shader.h"
#include "sprite.h"

using namespace DirectX::SimpleMath;

void Transition::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\sky.jpg");

	// �}�e���A���̐ݒ�
	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, 1.0f);
	material.TextureEnable = false;
	m_Sprite->SetMaterial(material);
}

void Transition::Update()
{
	switch (m_State)
	{

		case State::Stop:							// �t�F�[�h�C���I����\��
			break;

		case State::In:								// �t�F�[�h�C�����i�����x�������Ă����j
		{
			m_Alpha -= 2.0f / 60.0f;					// �P�b�ԂɂQ������

			if (m_Alpha < 0.0f)							// ���S�ɓ����ɂȂ�����STOP��Ԃ�
				m_State = State::Stop;

			break;
		}

		case State::Out:							// �t�F�[�h�A�E�g��
		{
			m_Alpha += 2.0f / 60.0f;					// �P�b�ԂɂQ���s����

			if (m_Alpha > 1.0f)							// ���S�ɕs�����ɂȂ�����FINISH��Ԃ�
				m_State = State::Finish;

			break;
		}

		case State::Finish:							// �t�F�[�h�A�E�g�I����\��
			break;

		default:
			break;
	}


	// �}�e���A���̐ݒ�
	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, m_Alpha);
	material.TextureEnable = false;
	m_Sprite->SetMaterial(material);

}

