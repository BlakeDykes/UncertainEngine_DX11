#pragma once

typedef uint32_t AkUniqueID;			 		///< Unique 32-bit ID

namespace EngineNames
{
	enum class CommandName
	{
		// Anim
		Light_Debug,
		Light_SetDeltaPos,
		Light_SetColor,

		// Audio
		Audio_Register,
		Audio_RegisterNotification,
		Audio_Unregister,
		Audio_PostEvent,
		Audio_SetState,
		Audio_SetDefaultListener,
		Audio_SetListener,
		Audio_SendPluginGameData,

		Audio_CB_RegisterSpectralDAO,
		Audio_CB_UnregisterSpectralDAO,

		// Wav
		Wav_Fill,

		// File
		File_Load,
		
		// Text
		SetMessage,

		// Utility
		ComPackage,

		UNINITIALIZED
	};

	enum class ComQueueName
	{
		GAME,
		ANIM,
		AUDIO,
		AUDIO_COORDINATOR,
		AUDIO_GAME,
		FILE,
		AUX,
		RTPC,
		UCallback,

		UNINITIALIZED
	};

	enum class ShaderName
	{
		ColorLight = 1,
		ColorLight_1_3_1,
		SimpleColor3D,
		ColorSelected3D,
		SimpleTexture,
		TextureLight,
		TextureLight_1_3_1,
		BlendedTile,
		BlendedTile_1_3_1,
		
		// Audio
		SpectrumLight,
		MagnitudeLight,

		UNINITIALIZED
	};


	namespace UTWWISE
	{
		enum class RTPCComName
		{
			LightParam,
		};

		enum class CallbackCookies : uint8_t
		{
			None			= 0x0,
			RegisterDAO		= 0x1,
			UnregisterDAO	= 0x2
		};

		namespace FILEPATH
		{
			static const std::wstring GENERATED_SOUND_BANKS	= L"../../../Wwise/ResearchProject/GeneratedSoundBanks/Windows/";
			static const std::wstring ORIGINAL_DATA			= L"../../../Original_Data/_Audio/";

			namespace ORIGINALS
			{
				static const std::wstring PLUGINS = ORIGINAL_DATA + L"Plugins/";
				static const std::wstring SFX = ORIGINAL_DATA + L"SFX/";
				static const std::wstring VOICES = ORIGINAL_DATA + L"Voices/";
			}

			namespace INFOFILE
			{
				static const std::wstring SOUND_BANK_INFO = GENERATED_SOUND_BANKS + L"SoundbanksInfo.xml";
			}
		}
	}

	const char* Stringify(ComQueueName name);
	const char* Stringify(ShaderName name);
	const char* Stringify(CommandName name);

	WCHAR* GetFileName(ShaderName name);
	
	void GetRTPCRange(AkUniqueID rtpcID, float& minOut, float& maxOut);
	const char* GetSourceWav(AkUniqueID eventID);

	
}

using namespace EngineNames;

