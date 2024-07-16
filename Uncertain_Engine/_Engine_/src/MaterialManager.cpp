#include "MaterialManager.h"
#include "MaterialObject.h"
#include "DLinkManager.h"

MaterialObject* MaterialManager::CreateMatObj(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular)
{
	MaterialObject* pMat = (MaterialObject*)Instance().BaseAdd();
	pMat->Set(ambient, diffuse, specular);
	return pMat;
}

MaterialObject* MaterialManager::CreateMatObj(const Vec4& flatColor)
{
	MaterialObject* pMat = (MaterialObject*)Instance().BaseAdd();
	pMat->Set(flatColor);
	return pMat;
}

MaterialManager::MaterialManager()
	: ManagerBase(*new DLinkManager(), *new DLinkManager())
{
	this->SetReserve(INITIAL_RESERVED, DELTA_GROW);
}

MaterialManager& MaterialManager::Instance()
{
	static MaterialManager instance;
	return instance;
}

NodeBase* MaterialManager::CreateNode()
{
	return (NodeBase*) new MaterialObject();
}
