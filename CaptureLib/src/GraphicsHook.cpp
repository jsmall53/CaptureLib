#include "GraphicsHook.h"

GraphicsHook::GraphicsHook()
	:_isHooked(false)
{
}

GraphicsHook::GraphicsHook(CaptureInfo* info)
	:_isHooked(false)
{
	_captureProcess = info;
}

GraphicsHook::~GraphicsHook()
{
}

void GraphicsHook::Initialize()
{
	//TODO: dont use hardcoded paths
	_hookDllPath = "D:/git/Capture/CaptureLib/obs_deps/graphics-hook64.dll";
	_injectPath = "D:/git/Capture/CaptureLib/obs_deps/inject-helper64.exe";
}

bool GraphicsHook::Inject()
{
	//TODO: Wrap this
	wchar_t injectPathW[MAX_PATH] = { 0 };
	wchar_t hookDllPathW[MAX_PATH] = { 0 };;
	std::wstring cmdLineArgsW;
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	bool success = false;

	mbstowcs(injectPathW, _injectPath.c_str(), MAX_PATH);
	mbstowcs(hookDllPathW, _hookDllPath.c_str(), MAX_PATH);

	cmdLineArgsW = CreateInjectCmdArgs(injectPathW, hookDllPathW, _captureProcess);

	success = CreateProcess(injectPathW, (LPWSTR)cmdLineArgsW.c_str(), NULL, NULL, false, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	if (success) {
		CloseHandle(pi.hThread);
		_captureProcess->SetInjectorProcess(pi.hProcess);
	}
	else {
		//LOG IT
	}
	//
	_isHooked = success;
	return success;
}

std::wstring GraphicsHook::CreateInjectCmdArgs(wchar_t* injectW, wchar_t* hookDllW, CaptureInfo* info)
{
	wchar_t args[4096] = { 0 };
	WindowInfo windowInfo = info->GetWindowInfo();

	swprintf_s(args, L"\"%s\" \"%s\" %lu %lu",
		injectW, hookDllW, TRUE, windowInfo.thread_id); //uses thread_id for "safe"

	return std::wstring(args);
}
