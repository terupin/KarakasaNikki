#include <SimpleMath.h>
#include"main.h"
#include"renderer.h"
#include "Model_Load.h"
#include "utftosjisconv.h"
#include"utility.h"

using namespace DirectX::SimpleMath;

//�A�j���[�V���������郂�f���̓ǂݍ���
void Model_Load::LoadModel(const char* FileName)
{
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];

	//�ό`�㒸�_�z�񐶐�
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	//�ċA�I�Ƀ{�[������
	CreateBone(m_AiScene->mRootNode);

	//�{�[���̔z��ʒu���i�[����
	unsigned int num = 0;
	for (auto& data : m_Bone)
	{
		data.second.idx = num;
		num++;
	}

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = Vector3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = Vector3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = Vector2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

				//���_�f�[�^��ۑ��i�����蔻��̂��߁j
				m_Vertices.emplace_back(vertex[v]);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd,
				&m_VertexBuffer[m]);

			delete[] vertex;
		}


		// �C���f�b�N�X�o�b�t�@����
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}

		//�ό`�㒸�_�f�[�^������
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX deformVertex;
			deformVertex.Position = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			m_DeformVertex[m].push_back(deformVertex);
		}

		//�{�[���f�[�^������
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

				m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
				m_DeformVertex[m][weight.mVertexId].BoneIdx[num] = m_Bone[bone->mName.C_Str()].idx;
				m_DeformVertex[m][weight.mVertexId].BoneNum++;

				assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
			}
		}
	}

	//���_�ϊ��iCPU�X�L�j���O�j
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@���b�N
		D3D11_MAPPED_SUBRESOURCE ms;
		HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		if (SUCCEEDED(hr)) {
			VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

				vertex->Position = Vector3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex->Normal = Vector3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex->TexCoord = Vector2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

				vertex->BoneIndex[0] = deformVertex->BoneIdx[0];
				vertex->BoneIndex[1] = deformVertex->BoneIdx[1];
				vertex->BoneIndex[2] = deformVertex->BoneIdx[2];
				vertex->BoneIndex[3] = deformVertex->BoneIdx[3];

				vertex->BoneWeight[0] = deformVertex->BoneWeight[0];
				vertex->BoneWeight[1] = deformVertex->BoneWeight[1];
				vertex->BoneWeight[2] = deformVertex->BoneWeight[2];
				vertex->BoneWeight[3] = deformVertex->BoneWeight[3];

				vertex++;
			}

			// ���_�o�b�t�@���X�V
			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0); 
		}
	}

	

	//�e�N�X�`���ǂݍ���
	if (TextureName == nullptr)
	{
		for (int i = 0; i < m_AiScene->mNumTextures; i++)
		{
			ID3D11ShaderResourceView* texture;

			aiTexture* aitexture = m_AiScene->mTextures[i];

			const size_t size = aitexture->mWidth;

			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				Renderer::GetDevice(),
				Renderer::GetDeviceContext(),
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&texture);

			assert(texture);

			m_INTexture[aitexture->mFilename.data] = texture;
		}
	}
	else
	{
		for (int i = 0; i < m_AiScene->mNumTextures; i++)
		{
			aiTexture* aitexture = m_AiScene->mTextures[i];

			const size_t size = aitexture->mWidth;

			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				Renderer::GetDevice(),
				Renderer::GetDeviceContext(),
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&m_OutTexture);

			assert(m_OutTexture);

			m_INTexture[aitexture->mFilename.data] = m_OutTexture;
		}
	}

	//�萔�o�b�t�@����
	bool sts = CreateConstantBufferWrite
	(
		Renderer::GetDevice(),  //�f�o�C�X�I�u�W�F�N�g
		sizeof(CBBoneCombMatrx),  //�R���X�^���g�o�b�t�@�T�C�Y
		&m_BoneCombMtxCBuffer);

	assert(m_BoneCombMtxCBuffer);

}

//�ό`���Ȃ����f���̓ǂݍ���
void Model_Load::LoadStaticModel(const char* FileName)
{
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_DeformVertex = 0;

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = Vector3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = Vector3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = Vector2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

				//���_�f�[�^��ۑ��i�����蔻��̂��߁j
				m_Vertices.emplace_back(vertex[v]);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd,
				&m_VertexBuffer[m]);

			delete[] vertex;
		}

		// �C���f�b�N�X�o�b�t�@����
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}
	}

	//�e�N�X�`���ǂݍ���
	if (TextureName == nullptr)
	{
		for (int i = 0; i < m_AiScene->mNumTextures; i++)
		{
			ID3D11ShaderResourceView* texture;

			aiTexture* aitexture = m_AiScene->mTextures[i];

			const size_t size = aitexture->mWidth;

			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				Renderer::GetDevice(),
				Renderer::GetDeviceContext(),
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&texture);

			assert(texture);

			m_INTexture[aitexture->mFilename.data] = texture;
		}
	}

	else
	{
		for (int i = 0; i < m_AiScene->mNumTextures; i++)
		{
			aiTexture* aitexture = m_AiScene->mTextures[i];

			const size_t size = aitexture->mWidth;

			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				Renderer::GetDevice(),
				Renderer::GetDeviceContext(),
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&m_OutTexture);

			assert(m_OutTexture);

			m_INTexture[aitexture->mFilename.data] = m_OutTexture;
		}
	}


	//�萔�o�b�t�@����
	bool sts = CreateConstantBufferWrite
	(
		Renderer::GetDevice(),  //�f�o�C�X�I�u�W�F�N�g
		sizeof(CBBoneCombMatrx),  //�R���X�^���g�o�b�t�@�T�C�Y
		&m_BoneCombMtxCBuffer);

	assert(m_BoneCombMtxCBuffer);
}

void Model_Load::LoadTex(const char* FileName)
{
	TextureName = FileName;  //�e�N�X�`���̖��O
	std::wstring ws = sjis_to_wide_winapi(FileName);

	//�e�N�X�`���ǂݍ���
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		ws.c_str(),
		nullptr,
		&m_OutTexture);
	assert(m_OutTexture);

	D3D11_TEXTURE2D_DESC textureDesc = {}; //�e�N�X�`���̐ݒ�

	//�e�N�X�`���f�[�^���g�p����TextureDesc��ݒ�
	Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, nullptr);

}

void Model_Load::LoadAnimation(const char* FileName, const char* AnimName)
{
	m_Animation[AnimName] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[AnimName]);
}

void Model_Load::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	//�X�^�e�B�b�N���f���ɂ͕ό`�㒸�_���Ȃ�����
	if (m_DeformVertex != nullptr)
	{
		delete[] m_DeformVertex;
	}

	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_INTexture)
	{
		pair.second->Release();
	}


	aiReleaseImport(m_AiScene);


	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		aiReleaseImport(pair.second);
	}
}

void Model_Load::Draw()
{
	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//���W�X�^5�ɃZ�b�g
	Renderer::GetDeviceContext()->VSSetConstantBuffers(5, 1, &this->m_BoneCombMtxCBuffer);

	for (unsigned int m = 0; m < this->m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = this->m_AiScene->mMeshes[m];
		aiMaterial* material = this->m_AiScene->mMaterials[mesh->mMaterialIndex];
		aiString path;  //�e�N�X�`���ݒ�

		//�e�N�X�`����ʂŐݒ肵�Ă��Ȃ��ꍇ
		if (this->TextureName == nullptr)
		{
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1,
				&this->m_INTexture[path.data]);
		}
		else 	//�ʂŃe�N�X�`����ݒ肵�Ă���ꍇ(TexLoad�œǂݍ���ł���)
		{
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1,
				&this->m_OutTexture);
		}

		//���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &this->m_VertexBuffer[m], &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�ݒ�
		Renderer::GetDeviceContext()->IASetIndexBuffer(this->m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//�|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void Model_Load::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	//�q�I�u�W�F�N�g�����ׂĉ�
	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}

}

