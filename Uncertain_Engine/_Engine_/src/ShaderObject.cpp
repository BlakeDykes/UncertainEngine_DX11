#include "ShaderObject.h"
#include <windows.h>
#include <sstream>
#include <assert.h>
#include <d3d11.h>
#include "d3dUtil.h"
#include "D3DCompiler.h"

ShaderObject::ShaderObject()
	: poVertexShader(nullptr), poPixelShader(nullptr), poVertexLayout(nullptr)
{
}

HRESULT ShaderObject::protCompile(ID3D11Device* device, ID3D11DeviceContext* context, WCHAR* fileName, D3D11_INPUT_ELEMENT_DESC layout[], UINT layoutElements)
{
	HRESULT hr = S_OK;

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(fileName, "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			"Problem compiling the VERTEX shader", "Error", MB_OK);
		assert(!FAILED(hr));
	}

	// Create the vertex shader
	hr = device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &poVertexShader);
	assert(!FAILED(hr));

	// Create the input layout
	hr = device->CreateInputLayout(layout, layoutElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &poVertexLayout);
	assert(!FAILED(hr));
	ReleaseAndDeleteCOMobject(pVSBlob);

	// Set the input layout
	context->IASetInputLayout(poVertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(fileName, "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			"Problem compiling the PIXEL shader", "Error", MB_OK);
		assert(!FAILED(hr));
	}

	// Create the pixel shader
	hr = device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &poPixelShader);
	ReleaseAndDeleteCOMobject(pPSBlob);
	assert(!FAILED(hr));

	return hr;
}

ShaderObject::~ShaderObject()
{
	ReleaseAndDeleteCOMobject(poVertexLayout);
	ReleaseAndDeleteCOMobject(poPixelShader);
	ReleaseAndDeleteCOMobject(poVertexShader);
}

void ShaderObject::SetActive()
{
	// Set the shader as active for context
	pContext->VSSetShader(poVertexShader, nullptr, 0);
	pContext->PSSetShader(poPixelShader, nullptr, 0);
}


// Shader loading utility. Will be moved elsewhere later...
// Needs to be moved. Requires
HRESULT ShaderObject::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}