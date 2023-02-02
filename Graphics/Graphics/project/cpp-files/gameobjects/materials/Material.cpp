#include "../../../header-files/gameobjects/materials/Material.h"
#include "../../../header-files/gameobjects/materials/Texture.h"
#include "../../../header-files/gameobjects/materials/Cubemap.h"

int CMaterial::Init(ID3D11Device* _p_dxdevice, ID3D11DeviceContext* _p_dxcontext)
{
	p_dxdevice = _p_dxdevice;
	p_dxcontext = _p_dxcontext;

	CreateVertexShader();
	CreatePixelShader();

	return 0;
}

int CMaterial::Init(ID3D11Device* _p_dxdevice, ID3D11DeviceContext* _p_dxcontext, LPCWSTR _fileName, bool _isCubemap)
{
	p_dxdevice = _p_dxdevice;
	p_dxcontext = _p_dxcontext;

	if (!_isCubemap)
	{
		p_texture = new CTexture(_fileName);
		p_texture->Init(p_dxdevice, p_dxcontext);
	}
	else
	{
		p_cubemap = new CCubemap(_fileName);
		p_cubemap->Init(p_dxdevice, p_dxcontext);
	}

	CreateVertexShader();
	CreatePixelShader();

	return 0;
}

void CMaterial::Render()
{
	// Vertex Shader
	p_dxcontext->VSSetShader(p_vertexShader, nullptr, 0);
	// Pixel Shader
	p_dxcontext->PSSetShader(p_pixelShader, nullptr, 0);

	p_dxcontext->IASetInputLayout(p_inputLayout);

	if (p_texture != nullptr)
		p_texture->Update();

	if (p_cubemap != nullptr)
		p_cubemap->Update();
}
bool CMaterial::DeInit()
{
	return false;
}

int CMaterial::CreateVertexShader()
{
	ID3DBlob* shaderBlob;

	HRESULT hr = D3DReadFileToBlob(vertexShaderName, &shaderBlob);
	FAILHR(-50);

	hr = p_dxdevice->CreateVertexShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), nullptr, &p_vertexShader);
	FAILHR(-51);

	CreateInputLayout(shaderBlob);

	return 0;
}

int CMaterial::CreateInputLayout(ID3DBlob* _p_vertexBlob)
{
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{
			"POSITION",						// Semantic - Identifikation im Shader
			0,								// Semantic index, falls es mehr als eins von diesem Typen vorhanden ist
			DXGI_FORMAT_R32G32B32_FLOAT,	// Float3
			0,								// Falls mehr als ein VertexShader vorhanden ist
			offsetof(SVertexPosColor, Position),
			D3D11_INPUT_PER_VERTEX_DATA,	// Werte einzeln f�r jeden Vertex nacheinander �bergeben
			0
		},
		{
			"NORMAL",						// Semantic - Identifikation im Shader
			0,								// Semantic index, falls es mehr als eins von diesem Typen vorhanden ist
			DXGI_FORMAT_R32G32B32_FLOAT,	// Float3
			0,								// Falls mehr als ein VertexShader vorhanden ist
			offsetof(SVertexPosColor, Normal),
			D3D11_INPUT_PER_VERTEX_DATA,	// Werte einzeln f�r jeden Vertex nacheinander �bergeben
			0
		},
		{
			"COLOR",						// Semantic - Identifikation im Shader
			0,								// Semantic index, falls es mehr als eins von diesem Typen vorhanden ist
			DXGI_FORMAT_R32G32B32A32_FLOAT,	// Float4
			0,								// Falls mehr als ein VertexShader vorhanden ist
			offsetof(SVertexPosColor, Color),
			D3D11_INPUT_PER_VERTEX_DATA,	// Werte einzeln f�r jeden Vertex nacheinander �bergeben
			0
		},
		{
			"TEXCOORD",						// Semantic - Identifikation im Shader
			0,								// Semantic index, falls es mehr als eins von diesem Typen vorhanden ist
			DXGI_FORMAT_R32G32_FLOAT,		// Float2
			0,								// Falls mehr als ein VertexShader vorhanden ist
			offsetof(SVertexPosColor, UV),
			D3D11_INPUT_PER_VERTEX_DATA,	// Werte einzeln f�r jeden Vertex nacheinander �bergeben
			0
		}
	};

	HRESULT hr = p_dxdevice->CreateInputLayout(vertexLayoutDesc,
		_countof(vertexLayoutDesc),
		_p_vertexBlob->GetBufferPointer(),
		_p_vertexBlob->GetBufferSize(),
		&p_inputLayout);
	FAILHR(-52);

	return 0;
}

int CMaterial::CreatePixelShader()
{
	ID3DBlob* shaderBlob;

	HRESULT hr = D3DReadFileToBlob(pixelShaderName, &shaderBlob);
	FAILHR(-53);

	hr = p_dxdevice->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&p_pixelShader);

	FAILHR(-54);

	return 0;
}