#pragma once

#include "component.h"
#include"renderer.h"

class Shader : public Component
{

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11Device* m_Device = Renderer::GetDevice();
	ID3D11DeviceContext* m_DeviceContext = Renderer::GetDeviceContext();

	long int fsize;
	unsigned char* buffer;

public:

	using Component::Component;

	void Load(const char* VertexShader, const char* PixelShader);
	void Uninit() override;
	void Draw() override;

	void FileInput(const char* FileName);  //ファイルの読み込み専用関数
	void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


};

