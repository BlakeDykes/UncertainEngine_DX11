#include "ReflectionManager.h"
#include "GraphicsObjectManager.h"
#include "ModelManager.h"
#include "TextureNode.h"
#include "ShaderNode.h"
#include "MaterialObject.h"
#include "ShaderManager.h"
#include "GameObjectManager.h"
#include "DLinkManager.h"
#include "DLinkIterator.h"

void ReflectionManager::Create(ID3D11Device*& device, ID3D11RasterizerState* rs_default)
{
	Instance().Initialize(device, rs_default);
}

void ReflectionManager::Shutdown()
{
	ReflectionManager* inst = &Instance();
	inst->DestroyAll();
	inst->ShutdownDirect3D();
}

void ReflectionManager::DestroyAll()
{
	ReflectionManager* inst = &Instance();

	inst->BaseClear();
}

void ReflectionManager::Update(_Time frameTime)
{
	IteratorBase* pIt = Instance().BaseGetIterator();
	ReflectionPlane* pCur = (ReflectionPlane*)pIt->First();

	while (!pIt->IsDone())
	{
		pCur->Update();
		pCur = (ReflectionPlane*)pIt->Next();
	}
}

ReflectionPlane* ReflectionManager::CreateReflectionPlane(
	ShaderNode* pShader, MaterialObject* material, TextureNode* text,
	float len, float urep, float vrep,
	const Vec3& scale, const Quat& rot, const Vec3& trans
)
{
	ModelNode* pModel = ModelManager::CreateFlatPlane(len, urep, vrep);

	GraphicsObject* pGraphObj = GraphicsObjectManager::CreateGraphicsObj(pModel, pShader, material, text);

	GameObject* pMirror = GameObjectManager::CreateReflectionObject(pGraphObj, scale, rot, trans);

	ReflectionPlane* pPlane = (ReflectionPlane*)Instance().BaseAdd();

	pPlane->Set(*pMirror);

	return pPlane;
}

void ReflectionManager::RenderReflections(ID3D11DeviceContext* context)
{
	ReflectionManager* inst = &Instance();

	inst->MarkStencils(context);
	inst->RenderReflectedObjects(context);
	inst->RenderMirrors(context);
	inst->RenderDeferred(context);
}

void ReflectionManager::SetDefaulttoWireframe(ID3D11RasterizerState* rs_default)
{
	ReflectionManager* inst = &Instance();
	inst->pRS_Default = rs_default;

	inst->poRS_D_Mirror->FillMode = D3D11_FILL_WIREFRAME;
	ReleaseAndDeleteCOMobject(inst->poRS_Mirror);

	inst->pDevice->CreateRasterizerState(inst->poRS_D_Mirror, &inst->poRS_Mirror);
}

void ReflectionManager::SetDefaulttoSolidFill(ID3D11RasterizerState* rs_default)
{
	ReflectionManager* inst = &Instance();
	inst->pRS_Default = rs_default;

	inst->poRS_D_Mirror->FillMode = D3D11_FILL_SOLID;
	ReleaseAndDeleteCOMobject(inst->poRS_Mirror);

	inst->pDevice->CreateRasterizerState(inst->poRS_D_Mirror, &inst->poRS_Mirror);
}

void ReflectionManager::MarkStencils(ID3D11DeviceContext* context)
{
	context->OMSetBlendState(poBS_NoWrite, nullptr, 0xffffffff);
	context->OMSetDepthStencilState(poDSS_MarkMirror, 1);

	DLinkIterator* pIt = (DLinkIterator*)BaseGetIterator();
	ReflectionPlane* pCur = (ReflectionPlane*)pIt->Current();

	while (!pIt->IsDone())
	{
		pCur->Render(context);
		pCur = (ReflectionPlane*)pIt->Next();
	}

	context->OMSetDepthStencilState(0, 0);
	context->OMSetBlendState(0, nullptr, 0xffffffff);
}

void ReflectionManager::RenderReflectedObjects(ID3D11DeviceContext* context) const
{
	context->RSSetState(poRS_Mirror);
	context->OMSetDepthStencilState(poDSS_DrawReflection, 1);

	GameObjectManager::Draw_Reflections(context);

	context->OMSetDepthStencilState(0, 0);
	context->RSSetState(pRS_Default);
}

void ReflectionManager::RenderMirrors(ID3D11DeviceContext* context)
{
	context->OMSetBlendState(poBS_Transparent, nullptr, 0xffffffff);

	DLinkIterator* pIt = (DLinkIterator*)BaseGetIterator();
	ReflectionPlane* pCur = (ReflectionPlane*)pIt->Current();

	while (!pIt->IsDone())
	{
		pCur->Render(context);
		pCur = (ReflectionPlane*)pIt->Next();
	}

	context->OMSetBlendState(0, nullptr, 0xffffffff);
}

void ReflectionManager::RenderDeferred(ID3D11DeviceContext* context)
{
	GameObjectManager::Draw_Deferred(context);
}

void ReflectionManager::ShutdownDirect3D()
{
	ReleaseAndDeleteCOMobject(poBS_Transparent);
	ReleaseAndDeleteCOMobject(poBS_NoWrite);

	ReleaseAndDeleteCOMobject(poRS_Mirror);
	delete poRS_D_Mirror;

	ReleaseAndDeleteCOMobject(poDSS_DrawReflection);
	ReleaseAndDeleteCOMobject(poDSS_MarkMirror);
}

ReflectionManager::ReflectionManager()
	: ManagerBase(*new DLinkManager(), *new DLinkManager()), pDevice(nullptr), poDSS_MarkMirror(nullptr), poDSS_DrawReflection(nullptr),
	poRS_D_Mirror(new D3D11_RASTERIZER_DESC()), pRS_Default(nullptr), poRS_Mirror(nullptr), poBS_NoWrite(nullptr), 
	poBS_Transparent(nullptr)
{
	SetReserve(INITIAL_RESERVE, DELTA_GROW);
}

void ReflectionManager::Initialize(ID3D11Device*& device, ID3D11RasterizerState* rs_default)
{
	assert(this->pDevice == nullptr);
	assert(this->pRS_Default == nullptr);

	this->pDevice = device;
	this->pRS_Default = rs_default;

	HRESULT hr = S_OK;

	// Create Depth Stencil for marking the mirror
	D3D11_DEPTH_STENCIL_DESC mirrorDesc;
	ZeroMemory(&mirrorDesc, sizeof(mirrorDesc));
	mirrorDesc.DepthEnable = true;								// Use the depth test
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;	// but don't write to the depth buffer
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mirrorDesc.StencilEnable = true;							// Use the stencil test
	mirrorDesc.StencilReadMask = 0xff;
	mirrorDesc.StencilWriteMask = 0xff;

	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// When passing the test (Always) replace
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;	// the color value with the reference value (int 1)

	// Back faces are culled, so these don't matter 
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = pDevice->CreateDepthStencilState(&mirrorDesc, &poDSS_MarkMirror);
	assert(SUCCEEDED(hr));

	// Create Depth Stencil for drawing the reflection
	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	ZeroMemory(&drawReflectionDesc, sizeof(drawReflectionDesc));
	drawReflectionDesc.DepthEnable = true;								// Use the depth test
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// normally
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true;							// Use the stencil test
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// When passing the stencil comparison test,
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// do not modify the stencil

	// Back faces are culled, so these don't matter
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	hr = pDevice->CreateDepthStencilState(&drawReflectionDesc, &poDSS_DrawReflection);
	assert(SUCCEEDED(hr));

	// Create Rasterizer state for mirror
	poRS_D_Mirror->FillMode = D3D11_FILL_SOLID;   // D3D11_FILL_WIREFRAME
	poRS_D_Mirror->CullMode = D3D11_CULL_BACK;
	poRS_D_Mirror->FrontCounterClockwise = false;
	poRS_D_Mirror->DepthBias = 0;
	poRS_D_Mirror->SlopeScaledDepthBias = 0.0f;
	poRS_D_Mirror->DepthBiasClamp = 0.0f;
	poRS_D_Mirror->DepthClipEnable = true;
	poRS_D_Mirror->ScissorEnable = false;
	poRS_D_Mirror->MultisampleEnable = true;
	poRS_D_Mirror->AntialiasedLineEnable = false;

	hr = pDevice->CreateRasterizerState(poRS_D_Mirror, &poRS_Mirror);
	assert(SUCCEEDED(hr));

	// Create Blend State for standard alpha blending
	// c_src*(a_src) + c_dst*(1-a_src) 
	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = pDevice->CreateBlendState(&bd, &poBS_Transparent);
	assert(SUCCEEDED(hr));

	// Create Blend State that prevents all rendering
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = false;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = 0;

	hr = pDevice->CreateBlendState(&bd, &poBS_NoWrite);
	assert(SUCCEEDED(hr));


}

ReflectionManager& ReflectionManager::Instance()
{
	static ReflectionManager instance;
	return instance;
}

NodeBase* ReflectionManager::CreateNode()
{
	return (NodeBase*)new ReflectionPlane();
}
