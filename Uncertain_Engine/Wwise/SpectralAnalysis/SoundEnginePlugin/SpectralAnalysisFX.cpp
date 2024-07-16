/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2024 Audiokinetic Inc.
*******************************************************************************/

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "SpectralAnalysisFX.h"
#include "../SpectralAnalysisConfig.h"

#include <AK/AkWwiseSDKVersion.h>
#include <AK/SoundEngine/Common/IAkPluginMemAlloc.h>

AK::IAkPlugin* CreateSpectralAnalysisFX(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, SpectralAnalysisFX());
}

AK::IAkPluginParam* CreateSpectralAnalysisFXParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, SpectralAnalysisFXParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(SpectralAnalysisFX, AkPluginTypeEffect, SpectralAnalysisConfig::CompanyID, SpectralAnalysisConfig::PluginID)

SpectralAnalysisFX::SpectralAnalysisFX()
    : m_pParams(nullptr)
    , m_pAllocator(nullptr)
    , m_pContext(nullptr)
    , poFFT(nullptr)
    , poDAOManager(nullptr)
{
}

AKRESULT SpectralAnalysisFX::Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkEffectPluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat)
{
    m_pParams = (SpectralAnalysisFXParams*)in_pParams;
    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;

    poFFT = AK_PLUGIN_NEW(m_pAllocator, FFT);
    poFFT->Setup(in_pAllocator, in_rFormat); 

    poDAOManager = AK_PLUGIN_NEW(m_pAllocator, DAOManager);
    poDAOManager->Setup(m_pAllocator, in_rFormat);
    
    return AK_Success;
}

AKRESULT SpectralAnalysisFX::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    if (poDAOManager)
    {
        poDAOManager->Destroy(in_pAllocator);
        AK_PLUGIN_DELETE(in_pAllocator, poDAOManager);
    }
    
    if (poFFT)
    {
        poFFT->Destroy(in_pAllocator);
        AK_PLUGIN_DELETE(in_pAllocator, poFFT);
    }

    AK_PLUGIN_DELETE(in_pAllocator, this);

    return AK_Success;
}

AKRESULT SpectralAnalysisFX::Reset()
{
    return AK_Success;
}

AKRESULT SpectralAnalysisFX::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeEffect;
    out_rPluginInfo.bIsInPlace = true;
	out_rPluginInfo.bCanProcessObjects = false;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;
}

void SpectralAnalysisFX::Execute(AkAudioBuffer* io_pBuffer)
{
    poDAOManager->ReceiveGameData(m_pContext);

    if (poDAOManager->HasSubscribers())
    {
        if (poFFT->Execute(io_pBuffer) == FFT::FrameStatus::Complete)
        {
            poDAOManager->WriteToGame(poFFT->GetWriteBuffer());

            poFFT->ClearBuffers();
        }
    }
}

AKRESULT SpectralAnalysisFX::TimeSkip(AkUInt32 in_uFrames)
{
    return AK_DataReady;
}
