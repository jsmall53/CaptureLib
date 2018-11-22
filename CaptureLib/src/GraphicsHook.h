#pragma once
#include <CaptureInfo.h>
#include <obs/ipc-util/pipe.h>

class GraphicsHook {
public:
	GraphicsHook();
	GraphicsHook(CaptureInfo *info);
	GraphicsHook(const char* processToHook);
	~GraphicsHook();
	
	
	//Public Methods
	void Initialize();
	void CleanUp();

	//Hooking
	bool TryHook(); //TODO: rename
	void InitHook(); //TODO: rename
	bool Inject();
	bool StartPipeServer();
	

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
	bool CreateKeepliveMutex();

	void StopPipeServer();

	//
	//TODO: Don't think this class should own this object, will probably be owned by a parent controller/manager object
	CaptureInfo* _captureProcess;
	ipc_pipe_server_t _pipeServer;
	
	HANDLE _injectorProcess;
	std::string _hookDllPath;
	std::string _injectPath; //path to obs helper exe
	bool _isHooked;
};