#include "SpectralRequest.h"

SpectralRequest::SpectralRequest(RequestedChannel reqChannel)
	: ReqChannel(reqChannel)
{
}

void SpectralRequest::Execute(const DataBundle& data)
{
}

SR_FFT::SR_FFT(RequestedChannel reqChannel)
	: SpectralRequest(reqChannel)
{
}

void SR_FFT::Execute(const DataBundle& data)
{
	HandleRequest(data.FFT.GetBufferPosition(ReqChannel));
}

SR_Mag::SR_Mag(RequestedChannel reqChannel)
	: SpectralRequest(reqChannel)
{
}

void SR_Mag::Execute(const DataBundle& data)
{
	HandleRequest(data.Mag.GetBufferPosition(ReqChannel));
}

SR_Spectrum::SR_Spectrum(RequestedChannel reqChannel)
	: SpectralRequest(reqChannel)
{
}

void SR_Spectrum::Execute(const DataBundle& data)
{
	HandleRequest(data.Spectrum.GetBufferPosition(ReqChannel));
}
