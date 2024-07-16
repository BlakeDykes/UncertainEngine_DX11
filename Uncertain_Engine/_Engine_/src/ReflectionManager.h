#pragma once

#include "ManagerBase.h"
#include "ReflectionPlane.h"
#include "_Time.h"

class GameObject;
class GraphicsObject;
class ShaderNode;
class MaterialObject;
class TextureNode;

class ReflectionManager : public ManagerBase
{
	static const int DELTA_GROW = 2;
	static const int INITIAL_RESERVE = 2;

public:
	static void Create(ID3D11Device*& device, ID3D11RasterizerState* rs_default);
	static void Shutdown();
	static void DestroyAll();

	static void Update(_Time frameTime);

	static ReflectionPlane* CreateReflectionPlane(
		ShaderNode* pShader, MaterialObject* material, TextureNode* text,
		float len, float urep, float vrep,
		const Vec3& scale, const Quat& rot, const Vec3& trans
	);

	static void RenderReflections(ID3D11DeviceContext* context);

	static void SetDefaulttoWireframe(ID3D11RasterizerState* rs_default);
	static void SetDefaulttoSolidFill(ID3D11RasterizerState* rs_default);

private:
	ReflectionManager();
	ReflectionManager(const ReflectionManager&) = delete;
	ReflectionManager& operator=(const ReflectionManager&) = delete;

	void Initialize(ID3D11Device*& device, ID3D11RasterizerState* rs_default);
	void MarkStencils(ID3D11DeviceContext* context);
	void RenderReflectedObjects(ID3D11DeviceContext* context) const;
	void RenderMirrors(ID3D11DeviceContext* context);
	void RenderDeferred(ID3D11DeviceContext* context);

	void ShutdownDirect3D();

	ID3D11Device* pDevice;

	ID3D11DepthStencilState* poDSS_MarkMirror;
	ID3D11DepthStencilState* poDSS_DrawReflection;

	D3D11_RASTERIZER_DESC* poRS_D_Mirror;
	ID3D11RasterizerState* pRS_Default;
	ID3D11RasterizerState* poRS_Mirror;

	ID3D11BlendState* poBS_NoWrite;
	ID3D11BlendState* poBS_Transparent;

	static ReflectionManager& Instance();

	// Inherited via ManagerBase
	NodeBase* CreateNode() override;
};