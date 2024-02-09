#include    "utility.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

// �w�莞�Ԍ�ɃX���b�h���N��
void Invoke(std::function<void()> Function, int Time)
{
    std::thread([=]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(Time));
            //			Sleep(Time);
            Function();
        }).detach();
}


// �V�F�[�_�[�t�@�C����ǂݍ���

bool readShader(const char* csoName, std::vector<unsigned char>& byteArray)
{
    FILE* fp;
    int ret = fopen_s(&fp, csoName, "rb");
    if (ret != 0) {
        return false;
    }

    fseek(fp, 0, SEEK_END);

    int size = ftell(fp);

    byteArray.resize(size);

    fseek(fp, 0, SEEK_SET);
    fread(byteArray.data(), byteArray.size(), 1, fp);

    fclose(fp);

    return true;
}


// �t�@�C���̊g���q���擾����

std::string GetFileExt(const char* filename) {
    std::string extname;

    std::string fullpathstr(filename);
    size_t ext_i = fullpathstr.find_last_of(".");
    extname = fullpathstr.substr(ext_i + 1, fullpathstr.size() - ext_i);
    return extname;
}


// �V�F�[�_�[���R���p�C��

HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    ID3DBlob* p1 = nullptr;

    HRESULT hr = S_OK;

    WCHAR	filename[512];
    size_t 	wLen = 0;
    int err = 0;

    // char -> wchar�ɕϊ�
    setlocale(LC_ALL, "japanese");
    err = mbstowcs_s(&wLen, filename, 512, szFileName, _TRUNCATE);

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile(
        filename,							// filename LPCWST pFileName
        nullptr,							// D3D_SHADER_MACRO *pDefines
        D3D_COMPILE_STANDARD_FILE_INCLUDE,	// ID3DInclude *pInclude
        szEntryPoint,						// LPCSTR pEntrypoint
        szShaderModel,						// LPCSTR pTarget
        dwShaderFlags,						// UINT Flags1
        0,									// UINT Flags2
        ppBlobOut,							// ID3DBlob** ppCode
        &pErrorBlob);						// ID3DBlob** ppErrorMsg 
    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr) {
            MessageBoxW(NULL,
                L"", L"Error", MB_OK);
        }
        if (pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}


// Structured�o�b�t�@���쐬

bool CreateStructuredBuffer(
    ID3D11Device* device,
    unsigned int stride,				// �X�g���C�h�o�C�g��
    unsigned int num,					// ��
    void* data,							// �f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pStructuredBuffer	// RWStructuredBuffer
) {

    HRESULT hr;

    // Structured�o�b�t�@����
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;		// UA�o�b�t�@
    bd.ByteWidth = stride * num;													// �o�b�t�@�̑傫��
    bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;							// RWStructuredBuffer
    bd.StructureByteStride = stride;												// �\�����o�b�t�@�T�C�Y
    bd.CPUAccessFlags = 0;															// CPU�A�N�Z�X�s�v

    if (data != nullptr) {
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));

        InitData.pSysMem = data;							// �o�b�t�@�̏����l

        hr = device->CreateBuffer(&bd, &InitData, pStructuredBuffer);		// �o�b�t�@����
    }
    else {
        hr = device->CreateBuffer(&bd, nullptr, pStructuredBuffer);		    // �o�b�t�@����
    }

    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"CreateBuffer(StructuredBuffer) error", L"Error", MB_OK);
        return false;
    }

    return true;
}


// Structured�o�b�t�@���쐬
bool CreateStructuredBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// �X�g���C�h�o�C�g��
    unsigned int num,					// ��
    void* data,							// �f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pStructuredBuffer	// RWStructuredBuffer
) {

    HRESULT hr;

    // Structured�o�b�t�@����
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;					// UA�o�b�t�@
    bd.Usage = D3D11_USAGE_DYNAMIC;								// �o�b�t�@�g�p���@
    bd.ByteWidth = stride * num;								// �o�b�t�@�̑傫��
    bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;		// RWStructuredBuffer
    bd.StructureByteStride = stride;							// �\�����o�b�t�@�T�C�Y
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					// CPU�A�N�Z�X�s�v

    if (data != nullptr) {
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));

        InitData.pSysMem = data;							// �o�b�t�@�̏����l

        hr = device->CreateBuffer(&bd, &InitData, pStructuredBuffer);		// �o�b�t�@����
    }
    else {
        hr = device->CreateBuffer(&bd, nullptr, pStructuredBuffer);		// �o�b�t�@����
    }
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"CreateBuffer(StructuredBuffer) error", L"Error", MB_OK);
        return false;
    }

    return true;
}

//ShaderResourceView���쐬
bool CreateShaderResourceView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	// Buffer
    ID3D11ShaderResourceView** ppSRV) {

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    pBuffer->GetDesc(&bd);

    D3D11_SHADER_RESOURCE_VIEW_DESC  srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    srvDesc.BufferEx.FirstElement = 0;

    if (bd.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) {
        srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
        srvDesc.BufferEx.NumElements = bd.ByteWidth / 4;
    }
    else if (bd.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED) {
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.BufferEx.NumElements = bd.ByteWidth / bd.StructureByteStride;
    }
    else {
        return false;
    }

    HRESULT hr;
    hr = device->CreateShaderResourceView(pBuffer, &srvDesc, ppSRV);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"CreateShaderResourceView error", L"Error", MB_OK);
        return false;
    }

    return true;
}


//UnOrderedAccessView���쐬
bool CreateUnOrderAccessView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	                    // Buffer
    ID3D11UnorderedAccessView** ppUAV) {

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    pBuffer->GetDesc(&bd);

    D3D11_UNORDERED_ACCESS_VIEW_DESC  uavDesc;
    ZeroMemory(&uavDesc, sizeof(uavDesc));
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.FirstElement = 0;

    if (bd.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) {
        uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
        uavDesc.Buffer.NumElements = bd.ByteWidth / 4;
    }
    else if (bd.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED) {
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.Buffer.NumElements = bd.ByteWidth / bd.StructureByteStride;
    }
    else {
        return false;
    }

    HRESULT hr;
    hr = device->CreateUnorderedAccessView(pBuffer, &uavDesc, ppUAV);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"CreateUnorderedAccessView error", L"Error", MB_OK);
        return false;
    }

    return true;
}

//STAGING�o�b�t�@���쐬���R�s�[����
ID3D11Buffer* CreateAndCopyToBuffer(
    ID3D11Device* device,
    ID3D11DeviceContext* devicecontext,
    ID3D11Buffer* pBuffer	// RWStructuredBuffer
) {

    HRESULT hr;
    ID3D11Buffer* CloneBuffer = nullptr;

    // Structured�o�b�t�@����
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    pBuffer->GetDesc(&bd);

    bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    bd.Usage = D3D11_USAGE_STAGING;
    bd.BindFlags = 0;
    bd.MiscFlags = 0;

    hr = device->CreateBuffer(&bd, nullptr, &CloneBuffer);
    if (SUCCEEDED(hr)) {
        devicecontext->CopyResource(CloneBuffer, pBuffer);
    }

    return CloneBuffer;
}

//���_�o�b�t�@���쐬
bool CreateVertexBuffer(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
) {
    HRESULT hr;

    // ���_�o�b�t�@����
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;							// �o�b�t�@�g�p���@
    bd.ByteWidth = stride * vertexnum;						// �o�b�t�@�̑傫��
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// ���_�o�b�t�@

    bd.CPUAccessFlags = 0;									// CPU�A�N�Z�X�s�v

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertexdata;							// �o�b�t�@�̏����l

    hr = device->CreateBuffer(&bd, &InitData, pVertexBuffer);		// �o�b�t�@����
    if (FAILED(hr)) {
        MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
        return false;
    }

    return true;
}

//���_�o�b�t�@���쐬(�b�o�t�������݉\)
bool CreateVertexBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
) {

    HRESULT hr;

    // ���_�o�b�t�@����
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;							// �o�b�t�@�g�p���@
    bd.ByteWidth = stride * vertexnum;						// �o�b�t�@�̑傫��
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// ���_�o�b�t�@
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				// CPU�A�N�Z�X�\

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertexdata;							// �o�b�t�@�̏����l

    hr = device->CreateBuffer(&bd, &InitData, pVertexBuffer);		// �o�b�t�@����
    if (FAILED(hr)) {
        MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
        return false;
    }

    return true;
}

//���_�o�b�t�@(UAV)���쐬
bool CreateVertexBufferUAV(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
) {

    HRESULT hr;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.ByteWidth = stride * vertexnum;						// �o�b�t�@�̑傫��
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags =
        D3D11_BIND_VERTEX_BUFFER |
        D3D11_BIND_SHADER_RESOURCE |
        D3D11_BIND_UNORDERED_ACCESS;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertexdata;							// �o�b�t�@�̏����l

    hr = device->CreateBuffer(&bd, &InitData, pVertexBuffer);		// �o�b�t�@����
    if (FAILED(hr)) {
        MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
        return false;
    }

    return true;
}


//�R���X�^���g�o�b�t�@���쐬(MAP�ŏ��������\)
bool CreateConstantBufferWrite(
    ID3D11Device* device,					// �f�o�C�X�I�u�W�F�N�g
    unsigned int bytesize,					// �R���X�^���g�o�b�t�@�T�C�Y
    ID3D11Buffer** pConstantBuffer			// �R���X�^���g�o�b�t�@
) {

    // �R���X�^���g�o�b�t�@����
    D3D11_BUFFER_DESC bd;

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;							// �o�b�t�@�g�p���@
    bd.ByteWidth = bytesize;									// �o�b�t�@�̑傫
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					// �R���X�^���g�o�b�t�@
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					// CPU�A�N�Z�X�\

    HRESULT hr = device->CreateBuffer(&bd, nullptr, pConstantBuffer);
    if (FAILED(hr)) {
        MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
        return false;
    }

    return true;
}
