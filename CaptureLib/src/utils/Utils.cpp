//
#include <windows.h>
#include <CaptureInfo.h>

static bool CheckFileIntegrity(CaptureInfo *info, 
	std::string filePath,
	std::string key) {

	if (filePath.empty()) {
		//LOG
		return false;
	}
	
	wchar_t bufW[MAX_PATH] = { 0 };
	mbstowcs(bufW, filePath.c_str(), MAX_PATH);
	std::wstring filePathW(bufW);

	HANDLE handle = CreateFile(filePathW.c_str(), GENERIC_READ | GENERIC_EXECUTE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (handle != INVALID_HANDLE_VALUE) {
		CloseHandle(handle);
		return true;
	}
	else {
		DWORD error = GetLastError();
		//LOG
	}
	//TODO: add in something to check hash against "registered" values
}