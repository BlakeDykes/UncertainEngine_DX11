#pragma once

#include "ManagerBase.h"
#include "Vec4.h"

class MaterialObject;


class MaterialManager : public ManagerBase
{
	static const int INITIAL_RESERVED = 10;
	static const int DELTA_GROW = 10;

public:
	static MaterialObject* CreateMatObj(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular);
	static MaterialObject* CreateMatObj(const Vec4& flatColor);

protected:
	MaterialManager();
	MaterialManager(const MaterialManager&) = delete;
	MaterialManager& operator =(const MaterialManager&) = delete;

	static MaterialManager& Instance();

	// Inherited via ManagerBase
	NodeBase* CreateNode() override;
};