#include "renderer.h"
#include "shader.h"
#include <d3dcompiler.h>
#include <io.h>

#pragma comment(lib,"d3dcompiler.lib")

void Shader::Load(const char* VertexShader, const char* PixelShader)
{

	CreateVertexShader(&m_VertexShader, &m_VertexLayout, VertexShader);

	CreatePixelShader(&m_PixelShader, PixelShader);
}

void Shader::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Shader::Draw()
{
	// 入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);


}

void Shader::FileInput(const char* FileName)
{
	FILE* file;

	fopen_s(&file, FileName, "rb");
	assert(file);

	//ファイル読み込み
	fsize = _filelength(_fileno(file));
	buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);
}

void Shader::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{
	FileInput(FileName);  //ファイルを読み込む

	//シェーダー作成
	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_SINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[]buffer;
}

void Shader::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FileInput(FileName);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

}


