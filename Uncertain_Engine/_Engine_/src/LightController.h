#pragma once

#include "SceneLightObject.h"
#include "Utils.h"
#include "Command.h"
#include "Mat4.h"
#include <map>
#include "Vec4.h"

class LightCommand;

class LightController
{
public:

	struct ReflectionController
	{
		LightIterator* poIt;
		unsigned char* poReflected;
		const Mat4* pLastReflection;
		LightController& Controller;

		ReflectionController() = delete;
		ReflectionController(const ReflectionController&) = delete;
		ReflectionController& operator=(const ReflectionController&) = delete;
		~ReflectionController();

		void RefreshData(const unsigned char& data, size_t dataSize);

		ReflectionController(LightController& controller, size_t dataSize, LightIterator* it);

		const unsigned char& Update(const Mat4& pReflection);
		void Update(PhongADS_Directional* light);
		void Update(PhongADS_Point* light);
		void Update(PhongADS_Spot* light);

	#ifdef LIGHT_DEBUG	
		void DEBUG_Print(PhongADS_Directional* pLight, const char* const id);
		void DEBUG_Print(PhongADS_Point* pLight, const char* const id);
		void DEBUG_Print(PhongADS_Spot* pLight, const char* const id);
	#endif
	};

	LightController(SceneLightObject& lightObj, LightIterator*& it);
	LightController() = delete;
	LightController(const LightController&) = delete;
	LightController& operator=(const LightController&) = delete;
	~LightController();

	void ReceiveLightCommand(LightCommand* lc);

	void UpdateWorld(PhongADS_Directional* pLight, const Vec4& rot, const Vec4& pos);
	void UpdateWorld(PhongADS_Point* pLight, const Vec4& rot, const Vec4& pos);
	void UpdateWorld(PhongADS_Spot* pLight, const Vec4& rot, const Vec4& pos);

	void TurnOff(PhongADS_Directional* pLight, float& specCacheOut);
	void TurnOff(PhongADS_Point* pLight, float& specCacheOut);
	void TurnOff(PhongADS_Spot* pLight, float& specCacheOut);

	void TurnOn(PhongADS_Directional*, float specVal);
	void TurnOn(PhongADS_Point* pLight, float specVal);
	void TurnOn(PhongADS_Spot* pLight, float specVal);

	void Update();

	const unsigned char& GetReflected(const Mat4& pReflection) { return poReflection->Update(pReflection); }

	size_t GetIndex(PhongADS_Directional* light) { return pIt->GetIndex(light); }
	size_t GetIndex(PhongADS_Point* light) { return pIt->GetIndex(light); }
	size_t GetIndex(PhongADS_Spot* light) { return pIt->GetIndex(light); }

private:
	struct ReflectionController;
	struct DeltaPos;

	void RefreshData(unsigned char*& dest) const;

	SceneLightObject& LightObject;
	LightIterator*& pIt;
	ReflectionController* poReflection;
	std::vector<LightCommand*> oLightCommands;
};

class LightCommand : public Command
{
public:
	LightCommand() = delete;
	LightCommand(CommandName name, ComQueueName execQueue) : Command(name, execQueue) {}
	virtual ~LightCommand() = default;
	virtual void Update(LightController::ReflectionController* pReflection) = 0;

	virtual void Execute() override {}
};
