#pragma once
#include <Windows.h>
#include "CaptureCommon.h"

class ICaptureEngine
{
	public:
		STDMETHOD(InitCapture(CaptureMode mode) = 0);
		STDMETHOD(StopCapture() = 0);
		// STDMETHOD(CopyTexture(/*TODO: inputs*/) = 0);
		// STDMETHOD(ResetInterval(/*TODO: inputs*/) = 0);
};
