#pragma once

#include <Windows.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <obs/obfuscate.h>

struct WindowInfo {
	HWND window;
	std::string title;
	std::string class_name;
	std::string executable;
	DWORD process_id;
	DWORD thread_id;
	RECT wndRect;
};

class WindowsUtils {
public:
	static std::string GetWindowExecutable(HWND window);
	static DWORD GetWindowProcessId(HWND window);
	static DWORD GetWindowTheadId(HWND window);
	static std::string GetWindowClass(HWND window);
	static std::string GetWindowTitle(HWND window);
	static RECT GetWindowPosition(HWND window);
	static bool TryGetWindowInfo(struct WindowInfo *info, std::string window);
	static HANDLE Open_Process(DWORD processID, DWORD access);

private:
	static BOOL CALLBACK EnumWindowsProc(__in HWND hWnd, __in LPARAM lParam);
	static struct WindowInfo GetWindowInfo(std::string windowName);
	static void FindAllWindows();
	static HWND FindWindowByName(std::string window);

	static inline HMODULE Kernel32() {
		static HMODULE kernel32_handle = NULL;
		if (!kernel32_handle)
			kernel32_handle = GetModuleHandleA("kernel32");
		return kernel32_handle;
	}

	static inline const char* GetExeNameFromFullPath(char* fullPath) {
		char *slash = strrchr(fullPath, '\\');
		return slash + 1;
	}

	static std::vector<HWND> WindowList;
};
