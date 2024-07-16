#pragma once

#include "NodeBase.h"
#include <d3d11.h>
#include "d3dUtil.h"
#include "Model.h"

class GameObject;
class MaterialObject;
class SceneLightObject;

struct FogParams
{
	float Range;
	float Start;
	Vec4 FogColor;

	FogParams(float start, float range, const Vec4& fogColor)
		: Range(range), Start(start), FogColor(fogColor)
	{ }
	FogParams() = default;
	FogParams(const FogParams&) = default;
	FogParams& operator=(const FogParams&) = default;
	~FogParams() = default;
};

class ShaderObject
{
public:
	ShaderObject();
	ShaderObject(const ShaderObject&) = default;
	ShaderObject& operator=(const ShaderObject&) = default;
	ShaderObject(ShaderObject&&) = default;
	ShaderObject& operator=(ShaderObject&&) = default;
	virtual ~ShaderObject();

	HRESULT CompileShader(ID3D11Device* device, ID3D11DeviceContext* context, ShaderName name);
	virtual void GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context) = 0;

	virtual void SetToCurrent() = 0;
	virtual void UpdateData(const GameObject* pGO, const MaterialObject* pMaterial) = 0;
	virtual void ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos);
	virtual void ReceiveLightData(const SceneLightObject& light);
	virtual void ReceiveLightData(const unsigned char& data);
	virtual void ReceiveFogData(const FogParams& fog);
	virtual void ReceiveReflectionMatrix(const Mat4& m);
	ID3D11DeviceContext*& GetContext() { return pContext; }


protected:
	ID3D11VertexShader* poVertexShader;
	ID3D11PixelShader* poPixelShader;
	ID3D11InputLayout* poVertexLayout;

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;

	virtual HRESULT protCompile(ID3D11Device* device, ID3D11DeviceContext* context, ShaderName name);

	void protSetToContext_VS_PS_IL();

private:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

};