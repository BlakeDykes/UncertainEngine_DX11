#pragma once

#include "ManagerBase.h"
#include "PCSTree.h"
#include "MathEngine.h"
#include "Camera.h"
#include "_Time.h"
#include "CommandQueue.h"
#include "GameObject.h"
#include <map>

struct ID3D11DeviceContext;
class CommandQueue;
class GraphicsObject;
class ReflectionPlane;
class GameObj_Root;

class GameObjectManager
{
public:
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator =(const GameObjectManager&) = delete;

	static void Create();
	static void Destroy();

	static void Update();
	static void Draw(ID3D11DeviceContext* context);
	static void Draw_Reflections(ID3D11DeviceContext* context);
	static void Draw_Deferred(ID3D11DeviceContext* context);

	static GameObject* Add(GraphicsObject* graphicsObj, const char* const name = "GameObject", GameObject* pParent = nullptr, GameObject::Type objectType = GameObject::Type::Default);
	static GameObject* Add_Reflected(ReflectionPlane* pReflection, GraphicsObject* graphicsObj, const char* const name = "GameObject", GameObject* pParent = nullptr, GameObject::Type objectType = GameObject::Type::Default);
	static GameObject* Add_Deferred(ReflectionPlane* pReflection, GraphicsObject* graphicsObj, const char* const name = "GameObject", GameObject* pParent = nullptr, GameObject::Type objectType = GameObject::Type::Default);
	
	static GameObject* CreateReflectionObject(GraphicsObject* graphicsObj,
		const Vec3& scale, const Quat& rot, const Vec3& trans,
		const char* const name = "Reflection Obj");

	static void Remove(GameObject& pNode);

	//static void SetMessage(GameObjectFont* pObj, const char* pMessage);

	//static void PrintAll();

	//// UI manager functions
	//class UI
	//{
	//private:
	//	friend UIManager;

	//	static GameObjectUIRoot* AddRoot(UINode& uiNode, const char* const name);
	//	static GameObjectUIElement* Add(Image& imageName, ShaderName shaderName, GameObjectUI* pParent, bool generateBoundingGraphics = GENERATE_BOUNDING_GRAPHICS);
	//	static GameObjectFont* Add(Font& font, const char* const pMessage, ShaderName shaderName, GameObjectUI* pParent, bool generateBoundingGraphics = GENERATE_BOUNDING_GRAPHICS);
	//	static void Remove(GameObjectUIElement& pNode);
	//	static void InsertUIObject(GameObjectUI* pUINode, GameObject* pParent);
	//};

private:
	static GameObjectManager* instance;
	GameObject* CreateGameObject(GraphicsObject* graphicsObj, const char* const name, GameObject::Type objectType, GameObject::RenderOrder order);
	
	void RemoveReflectionObject(GameObject& node);

	GameObj_Root* InitTree(std::map<ReflectionPlane*, PCSTree*>& map, ReflectionPlane* pPlane, GameObject::RenderOrder rOrder);

	PCSTree* FindTree(GameObject* pNode);

	GameObjectManager();
	static GameObjectManager* GetInstance();
	~GameObjectManager();

	std::map<ReflectionPlane*, PCSTree*> poReflected;
	PCSTree* poRootTree;
	std::map<ReflectionPlane*, PCSTree*> poDeferred;
	Uncertain::CommandQueue* pCommandQueue;
};