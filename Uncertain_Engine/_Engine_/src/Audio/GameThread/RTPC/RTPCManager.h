#pragma once

#include <map>
#include "AK/SoundEngine/Common/AkTypes.h"
#include "_Time.h"
#include "Vec3.h"

struct PhongADS_Directional;
struct PhongADS_Point;
struct PhongADS_Spot;
enum class LightParameters;

namespace Uncertain
{
	class RTPCObject;
	class RTPCC_LightParam;

	class RTPCManager
	{
	public:
		virtual ~RTPCManager();

		static void Create();
		static void Destroy();

		static RTPCC_LightParam* Subscribe(AkRtpcID rtpcID, PhongADS_Directional* pLight, LightParameters lParam, 
			const Vec3& min = Vec3(0.0f, 0.0f, 0.0f), const Vec3& max = Vec3(0.0f, 0.0f, 0.0f));
		static RTPCC_LightParam* Subscribe(AkRtpcID rtpcID, PhongADS_Point* pLight, LightParameters lParam, 
			const Vec3& min = Vec3(0.0f, 0.0f, 0.0f), const Vec3& max = Vec3(0.0f, 0.0f, 0.0f));
		static RTPCC_LightParam* Subscribe(AkRtpcID rtpcID, PhongADS_Spot* pLight, LightParameters lParam, 
			const Vec3& min = Vec3(0.0f, 0.0f, 0.0f), const Vec3& max = Vec3(0.0f, 0.0f, 0.0f));

		static void Set(AkRtpcID rtpcID, AkRtpcValue value);
		static void Add(AkRtpcID rtpcID, float value = 1.0f);
		static void Reduce(AkRtpcID rtpcID, float value = 1.0f);

		static void Update();

		static void LerpTo(AkRtpcID rtpcID, AkRtpcValue value, _Time totalLerpTime);

	private:
		std::map<AkRtpcID, RTPCObject*> RTPCs;

		RTPCManager();
		RTPCManager(const RTPCManager&) = delete;
		RTPCManager& operator =(const RTPCManager&) = delete;

		void LoadAll();

		static RTPCManager* instance;
		static RTPCManager* Instance();
	};
}