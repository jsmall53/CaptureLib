//

#include <utils/Utils.h>
#include <list>

using namespace std;

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



const int MAX_DIGITS = 10;
//4321 = 1->2->3->4
void IntToList(int num) {
	if (num == 0) {
		return;
	}
	string value = to_string(num);
	list<int> list;

	string::iterator end;
	for (end = value.end() - 1; end > value.begin(); end--) {
		list.push_back(*end);
 	}
}

bool SumEquals(int vals[], int target)
{
	int length = sizeof(vals) / sizeof(int);
	for (int i = 0; i < length; i++) {
		int num = target - vals[i];
		//if num is in vals, return true;
	}
	return false;
}
