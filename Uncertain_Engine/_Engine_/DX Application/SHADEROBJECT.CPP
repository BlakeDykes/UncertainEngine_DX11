#include "ShaderObject.h"
#include <windows.h>
#include <sstream>
#include <assert.h>
#include <d3d11.h>
#include "d3dUtil.h"
#include "D3DCompiler.h"
#include "SceneLightObject.h"
#include "_InputLayouts.h"

ShaderObject::ShaderObject()
	: poVertexShader(nullptr), poPixelShader(nullptr), poVertexLayout(nullptr), pDevice(nullptr), pContext(nullptr)
{
}

HRESULT ShaderObject::protCompile(ID3D11Device* device, ID3D11DeviceContext* context, ShaderName name)
{
	this->pDevice = device;
	this->pContext = context;

	ShaderCompileHelper cHelper = GetInputLayout(name);

	HRESULT hr = S_OK;

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(cHelper.fn, "VS", "vs_5_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			"Problem compiling the VERTEX shader", "Error", MB_OK);
		assert(!FAILED(hr));
	}

	// Create the vertex shader
	hr = pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &poVertexShader);
	assert(!FAILED(hr));

	// Create the input layout
	hr = pDevice->CreateInputLayout(cHelper.ia, cHelper.count, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &poVertexLayout);
	assert(!FAILED(hr));
	ReleaseAndDeleteCOMobject(pVSBlob);

	// Set the input layout
	context->IASetInputLayout(poVertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(cHelper.fn, "PS", "ps_5_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			"Problem compiling the PIXEL shader", "Error", MB_OK);
		assert(!FAILED(hr));
	}

	// Create the pixel shader
	hr = pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &poPixelShader);
	ReleaseAndDeleteCOMobject(pPSBlob);
	assert(!FAILED(hr));

	return hr;
}

void ShaderObject::protSetToContext_VS_PS_IL()
{
	// Set the shader and constant buffers as active for context
	pContext->VSSetShader(poVertexShader, nullptr, 0);
	pContext->PSSetShader(poPixelShader, nullptr, 0);
	pContext->IASetInputLayout(poVertexLayout);
}

ShaderObject::~ShaderObject()
{
	ReleaseAndDeleteCOMobject(poVertexLayout);
	ReleaseAndDeleteCOMobject(poPixelShader);
	ReleaseAndDeleteCOMobject(poVertexShader);
}

HRESULT ShaderObject::CompileShader(ID3D11Device* device, ID3D11DeviceContext* context, ShaderName name)
{
	HRESULT hr = protCompile( device, context, name );

	if (SUCCEEDED(hr))
	{
		ShaderObject::protSetToContext_VS_PS_IL();
		GenerateCBuffers(device, context);
	}

	return hr;
}

void ShaderObject::ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos)
{
	ALIGN_UNUSED_VAR(view);
	ALIGN_UNUSED_VAR(proj);
	ALIGN_UNUSED_VAR(pos);
	assert(false);
}

void ShaderObject::ReceiveLightData(const SceneLightObject& light)
{
	ALIGN_UNUSED_VAR(light);
}

void ShaderObject::ReceiveLightData(const unsigned char& data)
{
	ALIGN_UNUSED_VAR(data);
}

void ShaderObject::ReceiveFogData(const FogParams& fog)
{
	ALIGN_UNUSED_VAR(fog);
}

void ShaderObject::ReceiveReflectionMatrix(const Mat4& m)
{
	ALIGN_UNUSED_VAR(m);
}

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
	hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
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
