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
	void InitHook(); //TODO: rename
	bool Inject();
	

	//Public Inlines
	std::string HookedProcess() const
	{
		return _captureProcess->Target();
	}

	bool IsHooked() const {
		return _isHooked;
	}

private:

	//
	static std::wstring CreateInjectCmdArgs(wchar_t* injectW, wchar_t* hookDllW, CaptureInfo* info);
	bool CreateKeepAliveMutex();

	//
	//TODO: Don't think this class should own this object, will probably be owned by a parent controller/manager object
	CaptureInfo* _captureProcess;

	std::string _hookDllPath;
	std::string _injectPath; //path to obs helper exe

	bool _isHooked;
};