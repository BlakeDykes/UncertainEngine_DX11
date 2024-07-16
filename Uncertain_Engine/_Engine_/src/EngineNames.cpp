#include "EngineNames.h"
#include "AK/SoundEngine/Common/AkTypes.h"
#include <AK/Wwise_IDs.h>

const char* EngineNames::Stringify(ComQueueName name)
{
	switch (name)
	{
	case EngineNames::ComQueueName::GAME:				return "-- Game --";
	case EngineNames::ComQueueName::ANIM:				return "-- Animation --";
	case EngineNames::ComQueueName::AUDIO:				return "-- Audio -- ";
	case EngineNames::ComQueueName::AUDIO_COORDINATOR:	return "-- Audio File Coordinator -- ";
	case EngineNames::ComQueueName::AUDIO_GAME:			return "-- AudioObjectManager -- ";
	case EngineNames::ComQueueName::FILE:				return "-- File --";
	case EngineNames::ComQueueName::AUX:				return "-- Aux --";
	case EngineNames::ComQueueName::UCallback:			return "-- Audio UCallback --";
	case EngineNames::ComQueueName::UNINITIALIZED:		return "-- Uninitialized --";
	default:
		assert(false);
		return nullptr;
	}
}

const char* EngineNames::Stringify(ShaderName name)
{
	switch (name)
	{
	case EngineNames::ShaderName::ColorLight:				return "Color Light";
	case EngineNames::ShaderName::ColorLight_1_3_1:			return "Color Light 1_3_1";
	case EngineNames::ShaderName::SimpleColor3D:			return "Simple Color 3D";
	case EngineNames::ShaderName::ColorSelected3D:			return "Color Selected 3D";
	case EngineNames::ShaderName::SimpleTexture:			return "Simple Texture";
	case EngineNames::ShaderName::TextureLight:				return "Texture Light";
	case EngineNames::ShaderName::TextureLight_1_3_1:		return "Texture Light 1_3_1";
	case EngineNames::ShaderName::BlendedTile:				return "Blended Tile";
	case EngineNames::ShaderName::BlendedTile_1_3_1:		return "Blended Tile 1_3_1";
	case ShaderName::UNINITIALIZED:							return "Uninitialized";
	default:
		assert(false);
		return nullptr;
	}
}

const char* EngineNames::Stringify(CommandName name)
{
	switch (name)
	{
	// Light
	case EngineNames::CommandName::Light_Debug:						return "Light - Debug";
	case EngineNames::CommandName::Light_SetDeltaPos:				return "Light - Set Delta Pos";

	// Audio
	case EngineNames::CommandName::Audio_Register:					return "Audio - Register";
	case EngineNames::CommandName::Audio_RegisterNotification:		return "Audio - Register";
	case EngineNames::CommandName::Audio_Unregister:				return "Audio - Unregister";
	case EngineNames::CommandName::Audio_PostEvent:					return "Audio - Post event";
	case EngineNames::CommandName::Audio_SetState:					return "Audio - Set state";
	case EngineNames::CommandName::Audio_SetDefaultListener:		return "Audio - Set default listener";
	case EngineNames::CommandName::Audio_SetListener:				return "Audio - Set listener";
	case EngineNames::CommandName::Audio_SendPluginGameData:		return "Audio - Send plugin game data";
	// Audio Callbacks
	case EngineNames::CommandName::Audio_CB_RegisterSpectralDAO:	return "Audio Callback - Register Spectral DAO";

	// File
	case EngineNames::CommandName::File_Load:						return "File - Load";
	
	// Text
	case EngineNames::CommandName::SetMessage:						return "Text - Set Message";

	// Utility
	case EngineNames::CommandName::ComPackage:						return "Command Package";

	// Somethings gone wrong
	case EngineNames::CommandName::UNINITIALIZED:					return "Uninitialized";
	default:
		assert(false);
		return nullptr;
	}
}

WCHAR* EngineNames::GetFileName(ShaderName name)
{
	switch (name)
	{
	case EngineNames::ShaderName::ColorLight:			return (WCHAR*)L"../../Shaders/ColorLight.hlsl";
	case EngineNames::ShaderName::ColorLight_1_3_1:		return (WCHAR*)L"../../Shaders/ColorLight_1_3_1.hlsl";
	case EngineNames::ShaderName::SimpleColor3D:		return (WCHAR*)L"../../Shaders/SimpleColor3D.hlsl";
	case EngineNames::ShaderName::ColorSelected3D:		return (WCHAR*)L"../../Shaders/ColorSelected3D.hlsl";
	case EngineNames::ShaderName::SimpleTexture:		return (WCHAR*)L"../../Shaders/SimpleTexture.hlsl";
	case EngineNames::ShaderName::TextureLight:			return (WCHAR*)L"../../Shaders/TextureLight.hlsl";
	case EngineNames::ShaderName::TextureLight_1_3_1:	return (WCHAR*)L"../../Shaders/TextureLight_1_3_1.hlsl";
	case EngineNames::ShaderName::BlendedTile:			return (WCHAR*)L"../../Shaders/BlendedTile.hlsl";
	case EngineNames::ShaderName::BlendedTile_1_3_1:	return (WCHAR*)L"../../Shaders/BlendedTile.hlsl";
	case EngineNames::ShaderName::SpectrumLight:		return (WCHAR*)L"../../Shaders/SpectrumLight.hlsl";
	case EngineNames::ShaderName::MagnitudeLight:		return (WCHAR*)L"../../Shaders/MagnitudeLight.hlsl";

	case EngineNames::ShaderName::UNINITIALIZED:
	default:
		assert(false);
		return nullptr;
	}
}

void EngineNames::GetRTPCRange(AkUniqueID rtpcID, float& minOut, float& maxOut)
{
	switch (rtpcID)
	{
		case AK::GAME_PARAMETERS::PERCUSSION_LOWPASS:
		default:
		{
			minOut = 0.0f;
			maxOut = 100.0f;
		}
	}
}

const char* EngineNames::GetSourceWav(AkUniqueID eventID)
{
	switch (eventID)
	{
	case AK::STATES::PERCUSSION::STATE::ACTION:			return "../../../Original_Data/_Audio/SFK/SnackPack_1";
	case AK::STATES::PERCUSSION::STATE::CHILL:			return "../../../Original_Data/_Audio/SFK/SnackPack_4";
	case AK::STATES::PERCUSSION::STATE::OMINOUS:		return "../../../Original_Data/_Audio/SFK/SnackPack_5";
	default:
		assert(false);
		return nullptr;
	}
}
