#include "GraphicsHook.h"

GraphicsHook::GraphicsHook()
	:_isHooked(false)
{
	_pipeServer = { 0 };
}

GraphicsHook::GraphicsHook(CaptureInfo* info)
	:_isHooked(false),
	_injectorProcess(NULL)
{
	_captureProcess = info;
	_pipeServer = { 0 };
}

GraphicsHook::GraphicsHook(const char * processToHook)
	:_isHooked(false)
{
	_captureProcess = new CaptureInfo(processToHook);
	_pipeServer = { 0 };
}

GraphicsHook::~GraphicsHook()
{
	//
}

void GraphicsHook::Initialize()
{
	//TODO: dont use hardcoded paths
	_hookDllPath = "D:\\git\\Capture\\CaptureLib\\bin\\Debug_x64\\graphics-hook64.dll";
	_injectPath = "D:\\git\\Capture\\CaptureLib\\bin\\Debug_x64\\inject-helper64.exe";

	_captureProcess->Initialize();
}

void GraphicsHook::CleanUp()
{
	StopPipeServer();
	_captureProcess->CleanUp();
	CloseHandle(_injectorProcess);
}

bool GraphicsHook::TryHook()
{
	try {
		InitHook();
	} catch (const std::exception &ex) {
		//TODO: log the exception (ex.what();)
		UNREFERENCED_PARAMETER(ex);
		_isHooked = false;
		return false;
	}
	_isHooked = true;
	return true;
}

void GraphicsHook::InitHook()
{
	if (!_captureProcess->OpenTarget()) {
		throw std::exception("could not open the target process");
	}

	if (!CreateKeepliveMutex()) {
		throw std::exception("could not create the keep alive");
	}

	if (!Inject()) {
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
		_injectorProcess = pi.hProcess;
	}
	else {
		//TODO: Log error: could not open injector process
	}
	//
	return success;
}

bool GraphicsHook::StartPipeServer()
{
	//not sure what this method is for

	char buf[64];
	sprintf(buf, "%s_%lu", "GraphicsHookPipe", _captureProcess->GetWindowInfo().process_id);

	//not sure what to do with the last two parameters
	if (!ipc_pipe_server_start(&_pipeServer, buf, NULL, NULL)) {
		//TODO: Log it: pipe server failed to start
		return false;
	}

	return true;
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

bool GraphicsHook::CreateKeepliveMutex()
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

void GraphicsHook::StopPipeServer()
{
	ipc_pipe_server_free(&_pipeServer);
}
