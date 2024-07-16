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

#include "SpectralAnalysisFXParams.h"

#include <AK/Tools/Common/AkBankReadHelpers.h>

SpectralAnalysisFXParams::SpectralAnalysisFXParams()
    : RTPC(), NonRTPC()
{
}

SpectralAnalysisFXParams::SpectralAnalysisFXParams(const SpectralAnalysisFXParams& in_rParams)
{
    RTPC = in_rParams.RTPC;
    NonRTPC = in_rParams.NonRTPC;
    m_paramChangeHandler.SetAllParamChanges();
}

AK::IAkPluginParam* SpectralAnalysisFXParams::Clone(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, SpectralAnalysisFXParams(*this));
}

AKRESULT SpectralAnalysisFXParams::Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    return SetParamsBlock(in_pParamsBlock, in_ulBlockSize);
}

AKRESULT SpectralAnalysisFXParams::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT SpectralAnalysisFXParams::SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    AKRESULT eResult = AK_Success;

    if (in_pParamsBlock)
    {
        AkUInt8* pParamsBlock = (AkUInt8*)in_pParamsBlock;

        // Read bank data here
        NonRTPC.ObjectID = static_cast<AkUInt32>(READBANKDATA(AkUInt32, pParamsBlock, in_ulBlockSize));
        CHECKBANKDATASIZE(in_ulBlockSize, eResult);
        m_paramChangeHandler.SetAllParamChanges();

    }
    
    return eResult;
}

AKRESULT SpectralAnalysisFXParams::SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_ulParamSize)
{
    AKRESULT eResult = AK_Success;

    // Handle parameter change here
    switch (in_paramID)
    {
    case PARAM_OBJECT_ID:
        NonRTPC.ObjectID = *((AkUInt32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OBJECT_ID);
        break;
    default:
        eResult = AK_InvalidParameter;
        break;
    }

    return eResult;
}
