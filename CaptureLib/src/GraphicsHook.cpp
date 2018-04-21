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

GraphicsHook::GraphicsHook(const char * processToHook)
	:_isHooked(false)
{
	_captureProcess = new CaptureInfo(processToHook);
}

GraphicsHook::~GraphicsHook()
{
}

void GraphicsHook::Initialize()
{
	//TODO: dont use hardcoded paths
	_hookDllPath = "D:\\git\\Capture\\CaptureLib\\bin\\Debug_x64\\graphics-hook64.dll";
	_injectPath = "D:\\git\\Capture\\CaptureLib\\bin\\Debug_x64\\inject-helper64.exe";

	_captureProcess->Initialize();
}

bool GraphicsHook::TryHook()
{
	try {
		InitHook();
	} catch (const std::exception &ex) {
		//TODO: log the exception
		UNREFERENCED_PARAMETER(ex);
		return false;
	}

	return true;
}

void GraphicsHook::InitHook()
{
	if (!_captureProcess->OpenTarget()) {
		//TODO: Log error: could not open the target process
		throw std::exception("could not open the target process");
	}

	if (!CreateKeepAliveMutex()) {
		//TODO: Log error: could not create the keep alive
		throw std::exception("could not create the keep alive");
	}

	if (!Inject()) {
		//TODO: Log error: could not inject the hook into target
		throw std::exception("could not inject the hook into target");
	}
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
	return std::wstring(args);
}

bool GraphicsHook::CreateKeepAliveMutex()
{
	wchar_t mutex_name[64] = { 0 };
	swprintf_s(mutex_name, 64, L"%ws%lu", L"GraphicsHook_", _captureProcess->GetWindowInfo().process_id);

	HANDLE mutex = CreateMutex(NULL, false, mutex_name);
	if (mutex == NULL) {
		//TODO: LOG FAILURE
		return false;
	}

	_captureProcess->SetKeepaliveMutex(mutex);
	return true;
}
