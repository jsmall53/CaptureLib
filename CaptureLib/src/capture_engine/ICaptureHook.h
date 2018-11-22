#pragma once
#include <Windows.h>

class ICaptureHook
{
	public:
		STDMETHOD(InitHook(/*TODO: inputs*/) = 0);
		STDMETHOD(ReleaseHook(/*TODO: inputs*/) = 0);
};
