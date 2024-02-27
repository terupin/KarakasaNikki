#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "shadow.h"
#include "box.h"
#include "cylinder.h"

using namespace DirectX::SimpleMath;

void Shadow::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3(-1.0f, 0.01f, 1.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3(1.0f, 0.01f, 1.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(-1.0f, 0.01f, -1.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3(1.0f, 0.01f, -1.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"asset/texture/shadow.png",
		nullptr,
		&m_Texture);

	//assert(m_Texture);
}


void Shadow::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}





void Shadow::Update()
{
	//�ڒn
	float groundHeight = 0.0f;

	Scene* scene = Manager::GetScene();

	{
		std::vector<Cylinder*> cylinderList = scene->GetGameObjects<Cylinder>();

		for (Cylinder* cylinder : cylinderList)
		{
			if (cylinder == m_GameObject)
				continue;

			Vector3 position = cylinder->GetPosition();
			Vector3 scale = cylinder->GetScale();

			Vector3 direction = m_Position - position;
			direction.y = 0.0f;
			float length = direction.Length();

			if (length < scale.x)
			{
				groundHeight = position.y + scale.y;

				break;
			}
		}
	}

	{
		std::vector<Box*> boxList = scene->GetGameObjects<Box>();

		for (Box* box : boxList)
		{
			if (box == m_GameObject)
				continue;

			Vector3 position = box->GetPosition();
			Vector3 scale = box->GetScale();

			if (position.x - scale.x - 0.5f < m_Position.x && m_Position.x < position.x + scale.x + 0.5f &&
				position.z - scale.z - 0.5f < m_Position.z && m_Position.z < position.z + scale.z + 0.5f)
			{
				groundHeight = position.y + scale.y * 2.0f;

				break;
			}
		}
	}

	m_Position = m_GameObject->GetPosition();
	m_Position.y = groundHeight;

}




void Shadow::Draw()
{

	// �}�g���N�X�ݒ�
	Matrix world, scale, trans;
	scale = DirectX::SimpleMath::Matrix::CreateScale(m_Size);
	trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * trans;
	Renderer::SetWorldMatrix(&world);


	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::SetDepthEnable(false);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

	Renderer::SetDepthEnable(true);


}