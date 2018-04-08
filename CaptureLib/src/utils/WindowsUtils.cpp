#include "WindowsUtils.h"
#include <iostream>
#include <cassert>
#include <exception>

//Declarations
std::vector<HWND> WindowsUtils::WindowList;
//

std::string WindowsUtils::GetWindowExecutable(HWND window)
{
	DWORD proc_id, thread_id;
	wchar_t name_w[MAX_PATH];
	char name[MAX_PATH];

	thread_id = GetWindowThreadProcessId(window, &proc_id);
	if (proc_id == GetCurrentProcessId())
		return "";

	HANDLE process = NULL;
	process = Open_Process(proc_id, PROCESS_QUERY_LIMITED_INFORMATION);
	if (!process)
		return "";

	if (!GetProcessImageFileName(process, name_w, MAX_PATH))
		return "";

	wcstombs(name, name_w, MAX_PATH);

	const char* exe = GetExeNameFromFullPath(name);

	return std::string(exe);
}

DWORD WindowsUtils::GetWindowProcessId(HWND window)
{
	DWORD id;
	GetWindowThreadProcessId(window, &id);
	if (id == GetCurrentThreadId()) {
		return 0;
	}

	return id;
}

DWORD WindowsUtils::GetWindowTheadId(HWND window) {
	DWORD id;
	id = GetWindowThreadProcessId(window, NULL);
	if (id == GetCurrentThreadId())
		return 0;

	return id;
}

std::string WindowsUtils::GetWindowClass(HWND window)
{
	wchar_t temp[256];
	char final[256];
	if (GetClassName(window, temp, sizeof(temp) / sizeof(wchar_t))) {
		wcstombs(final, temp, 256);
	}

	return std::string(final);
}

std::string WindowsUtils::GetWindowTitle(HWND window)
{
	int len;
	len = GetWindowTextLength(window);
	if (!len) {
		//TODO: Throw an exception?
		return "";
	}

	wchar_t *name_w = new wchar_t[len + 1];
	char *name = new char[len + 1];
	if (GetWindowText(window, name_w, len + 1)) {
		wcstombs(name, name_w, len + 1);
	}

	return name;
}

RECT WindowsUtils::GetWindowPosition(HWND window)
{
	LPRECT rect = new RECT();
	GetWindowRect(window, rect);

	return (*rect);
}

BOOL WindowsUtils::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	std::string windowName = GetWindowTitle(hWnd);

	if (!windowName.empty()) {
		WindowList.push_back(hWnd);
	}

	return TRUE;
}

void WindowsUtils::FindAllWindows() {
	WindowList.clear();
	EnumWindows(&WindowsUtils::EnumWindowsProc, NULL);
}

HWND WindowsUtils::FindWindowByName(std::string window)
{
	std::string tmpName;

	//update the window list
	FindAllWindows();

	std::vector<HWND>::iterator iter;
	for (iter = WindowList.begin(); iter != WindowList.end(); iter++) {
		tmpName = GetWindowTitle(*iter);
		if (tmpName.compare(window) == 0) {
			return *iter;
		}
	}

	return 0;
}

bool WindowsUtils::TryGetWindowInfo(WindowInfo * info, std::string window)
{
	try {
		*info = GetWindowInfo(window);
	}
	catch (const std::exception &ex) {
		//log the exception, return false
		UNREFERENCED_PARAMETER(ex);
		return false;
	}

	return true;
}

HANDLE WindowsUtils::Open_Process(DWORD processID, DWORD access)
{
	static HANDLE(WINAPI *Open_Process_proc)(DWORD, BOOL, DWORD) = NULL;
	if (!Open_Process_proc) {
		Open_Process_proc = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))get_obfuscated_func(Kernel32(), "B}caZyah`~q", 0x2D5BEBAF6DDULL);
	}

	return Open_Process_proc(access, false, processID);
}

WindowInfo WindowsUtils::GetWindowInfo(std::string windowName)
{
	struct WindowInfo info = { 0 };
	info.window = FindWindowByName(windowName);

	if (!info.window) {
		//could not find the window
		throw std::exception("Window not found for given name");
	}

	assert(info.window != 0);

	info.process_id = GetWindowProcessId(info.window);
	if (info.process_id == 0) {
		throw std::exception("Could not find window process id");
	}

	info.thread_id = GetWindowProcessId(info.window);
	if (info.thread_id == 0) {
		throw std::exception("Could not find window thread id");
	}

	info.title = GetWindowTitle(info.window);
	if (info.title.empty()) {
		//no window title?
		throw std::exception("No window title associated with window handle");
	}

	info.class_name = GetWindowClass(info.window);
	if (info.class_name.empty()) {
		//no window class name, this is impossible if we have a valid window
		throw std::exception("No window class associated with window handle");
	}

	info.executable = GetWindowExecutable(info.window);
	if (info.executable.empty()) {
		//again, this should be impossible if there is a valid window object
		throw std::exception("No executable associated with window handle");
	}

	info.wndRect = GetWindowPosition(info.window);

	return info;
}


