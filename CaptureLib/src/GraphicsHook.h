#pragma once
#include <CaptureInfo.h>

class GraphicsHook {
public:
	GraphicsHook();
	GraphicsHook(CaptureInfo *info);
	GraphicsHook(const char* processToHook);
	~GraphicsHook();
	
	
	//Public Methods
	void Initialize();

	//Hooking
	bool TryHook(); //TODO: rename
	bool InitHook(); //TODO: rename
	bool Inject();
	

	//Public Inlines
	std::string HookedProcess()
	{
		return _captureProcess->Target();
	}

	bool IsHooked() {
		return _isHooked;
	}

private:

	//
	static std::wstring CreateInjectCmdArgs(wchar_t* injectW, wchar_t* hookDllW, CaptureInfo* info);
	bool CreateKeepAliveMutex();

	//
	CaptureInfo* _captureProcess;

	std::string _hookDllPath;
	std::string _injectPath; //path to obs helper exe

	bool _isHooked;
};