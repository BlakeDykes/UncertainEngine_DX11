#include "ShaderManager.h"
#include "_SOWrapper.h"
#include "SceneLightManager.h"
#include "DLinkManager.h"
#include "DLinkIterator.h"

ShaderManager* ShaderManager::instance = nullptr;

ShaderManager::ShaderManager(ID3D11Device*& device)
	: ManagerBase(*new DLinkManager(), *new DLinkManager()), pDevice(device),
	CurrentSO(0), poComp(new ShaderNode())
{
	this->SetReserve(INITIAL_RESERVED, DELTA_GROW);
}

ShaderManager::~ShaderManager()
{
	delete this->poComp;
	BaseClear();
}

void ShaderManager::Create(ID3D11Device*& device)
{
	assert(instance == nullptr);

	instance = new ShaderManager(device);

	Instance().pDevice = device;
}

void ShaderManager::DestroyAll()
{
	delete instance;
}

ShaderNode* ShaderManager::CreateShader(ID3D11DeviceContext* context, ShaderName name)
{
	ShaderObject* pShader = nullptr;

	switch (name)
	{
	case ShaderName::ColorLight:					pShader = new SO_ColorLight();				 break;
	case ShaderName::ColorLight_1_3_1:				pShader = new SO_ColorLight_1_3_1();		 break;
	case ShaderName::SimpleColor3D:					pShader = new SO_SimpleColor3D();			 break;
	case ShaderName::ColorSelected3D:				pShader = new SO_ColorSelected3D();			 break;
	case ShaderName::SimpleTexture:					pShader = new SO_SimpleTexture();			 break;
	case ShaderName::TextureLight_1_3_1:			pShader = new SO_TextureLight_1_3_1();		 break;
	case ShaderName::TextureLight:					pShader = new SO_TextureLight();			 break;
	case ShaderName::BlendedTile:					pShader = new SO_BlendedTile();				 break;
	case ShaderName::BlendedTile_1_3_1:				pShader = new SO_BlendedTile_1_3_1();		 break;
	case ShaderName::SpectrumLight:					pShader = new SO_SpectrumLight();			 break;
	case ShaderName::MagnitudeLight:				pShader = new SO_MagnitudeLight();			 break;
	default:
		assert(false);
		break;
	}

	HRESULT hr = pShader->CompileShader(Instance().pDevice, context, name);
	//HRESULT hr = pShader->PostCompileInit(Instance().pDevice, context, name);
	if (SUCCEEDED(hr))
	{
		ShaderNode* pNode = (ShaderNode*)Instance().BaseAdd();
		pNode->Set(name, pShader);
		Instance().CurrentSO = pNode->GetHandle();

		return pNode;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

ShaderNode* ShaderManager::FindShader(ShaderName name)
{
	ShaderManager* inst = &Instance();

	inst->poComp->Set(name);

	return (ShaderNode*)inst->BaseFind((NodeBase&)*inst->poComp);
}

ShaderManager::ShaderManager()
	: ManagerBase(*new DLinkManager(), *new DLinkManager()), pDevice(nullptr), CurrentSO(0), poComp(new ShaderNode())
{
}

void ShaderManager::SetCameraAndLighting(Camera& cam)
{
	ShaderManager* inst = &Instance();

	Vec3 pos;
	cam.getPos(pos);
	Mat4& view = cam.getViewMatrix();
	Mat4& proj = cam.getProjMatrix();
	SceneLightObject* pLightObj = SceneLightManager::GetActiveLightObj();

	DLinkIterator* pIt = (DLinkIterator*)inst->BaseGetIterator();
	ShaderObject* pCur = nullptr;

	while (!pIt->IsDone())
	{
		if (((ShaderNode*)pIt->Current())->GetShaderObj(pCur))
		{
			pCur->ReceiveCamData(view, proj, pos);
			pCur->ReceiveLightData(*pLightObj);

		}
		pIt->Next();
	}

}

void ShaderManager::SetLighting()
{
	ShaderManager* inst = &Instance();

	SceneLightObject* pLightObj = SceneLightManager::GetActiveLightObj();

	DLinkIterator* pIt = (DLinkIterator*)inst->BaseGetIterator();
	ShaderObject* pCur = nullptr;

	while (!pIt->IsDone())
	{
		if (((ShaderNode*)pIt->Current())->GetShaderObj(pCur))
		{
			pCur->ReceiveLightData(*pLightObj);

		}
		pIt->Next();
	}
}

void ShaderManager::SetReflectedLighting(const Mat4& reflectionMat)
{
	SceneLightObject* pLightObj = SceneLightManager::GetActiveLightObj();
	const unsigned char& lightData = pLightObj->GetReflected(reflectionMat);

	DLinkIterator* pIt = (DLinkIterator*)Instance().BaseGetIterator();
	ShaderObject* pCur = nullptr;

	while (!pIt->IsDone())
	{
		if (((ShaderNode*)pIt->Current())->GetShaderObj(pCur))
		{
			pCur->ReceiveLightData(lightData);

		}
		pIt->Next();
	}
}

void ShaderManager::SetFog(const FogParams& fog)
{
	DLinkIterator* pIt = (DLinkIterator*)Instance().BaseGetIterator();
	ShaderObject* pCur = nullptr;

	while (!pIt->IsDone())
	{
		if (((ShaderNode*)pIt->Current())->GetShaderObj(pCur))
		{
			pCur->ReceiveFogData(fog);
		}
		pIt->Next();
	}
}

void ShaderManager::SetToCurrent(ShaderNode*& so)
{
	ShaderManager* inst = &Instance();
	UINT soHandle = so->GetHandle();

	if (soHandle != inst->CurrentSO) 
	{
		inst->CurrentSO = soHandle;
		so->SetToCurrent();
	}
}

LightCount ShaderManager::GetMaxLightCount(ShaderName shaderName)
{
	switch (shaderName)
	{
	case ShaderName::BlendedTile:
	case ShaderName::ColorLight:
	case ShaderName::TextureLight:
	case ShaderName::SpectrumLight:
	case ShaderName::MagnitudeLight:
		return LightCount(1, 1, 1);
	case ShaderName::BlendedTile_1_3_1:
	case ShaderName::ColorLight_1_3_1:
	case ShaderName::TextureLight_1_3_1:
		return LightCount(1, 3, 1);

	case ShaderName::SimpleColor3D:
	case ShaderName::ColorSelected3D:
	case ShaderName::SimpleTexture:

	case ShaderName::UNINITIALIZED:
	default:
		return LightCount();
	}
}

ShaderManager& ShaderManager::Instance()
{
	assert(instance != nullptr);
    return *instance;
}

NodeBase* ShaderManager::CreateNode()
{
	return new ShaderNode();
}
