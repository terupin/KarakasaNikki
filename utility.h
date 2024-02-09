#pragma once
#include	<functional>
#include	<thread>
#include    <d3d11.h>
#include    <string>
#include    <locale.h>
#include    <d3dcompiler.h>


// �w�莞�Ԍ�ɃX���b�h���N��
void Invoke(std::function<void()> Function, int Time);

// �V�F�[�_�[�t�@�C����ǂݍ���
bool readShader(const char* csoName, std::vector<unsigned char>& byteArray);

// �t�@�C���̊g���q���擾����
std::string GetFileExt(const char* filename);

// �V�F�[�_�[���R���p�C��
HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

// Structured�o�b�t�@���쐬
bool CreateStructuredBuffer(
    ID3D11Device* device,
    unsigned int stride,				// �X�g���C�h�o�C�g��
    unsigned int num,					// ��
    void* data,							// �f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pStructuredBuffer	// RWStructuredBuffer
);

// Structured�o�b�t�@����������
bool CreateStructuredBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// �X�g���C�h�o�C�g��
    unsigned int num,					// ��
    void* data,							// �f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pStructuredBuffer	// RWStructuredBuffer
);

  //ShaderResourceView���쐬
bool CreateShaderResourceView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	// Buffer
    ID3D11ShaderResourceView** ppSRV);

//UnOrderedAccessView���쐬
bool CreateUnOrderAccessView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	                    // Buffer
    ID3D11UnorderedAccessView** ppUAV);

//STAGING�o�b�t�@���쐬���R�s�[����
ID3D11Buffer* CreateAndCopyToBuffer(
    ID3D11Device* device,
    ID3D11DeviceContext* devicecontext,
    ID3D11Buffer* pBuffer	// RWStructuredBuffer
);

//���_�o�b�t�@���쐬
bool CreateVertexBuffer(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
);

//���_�o�b�t�@���쐬(�b�o�t�������݉\)
bool CreateVertexBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
);

//���_�o�b�t�@(UAV)���쐬
bool CreateVertexBufferUAV(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
);

//�R���X�^���g�o�b�t�@���쐬(MAP�ŏ��������\)
bool CreateConstantBufferWrite(
    ID3D11Device* device,					// �f�o�C�X�I�u�W�F�N�g
    unsigned int bytesize,					// �R���X�^���g�o�b�t�@�T�C�Y
    ID3D11Buffer** pConstantBuffer			// �R���X�^���g�o�b�t�@
);