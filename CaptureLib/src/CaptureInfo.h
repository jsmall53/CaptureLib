#pragma once
#include <windows.h>
#include "utils\WindowsUtils.h"

class CaptureInfo {
public:
	CaptureInfo();
	CaptureInfo(std::string name);
	CaptureInfo(const char* name);
	~CaptureInfo();

	int Initialize();
	void Clear();
	bool OpenTarget();

	bool SetTarget(std::string target) {
		if (_targetName.empty()) {
			_targetName = target;
			return true;
		}
		return false;
	}

	bool SetTarget(const char* target) {
		return SetTarget(std::string(target));
	}

	std::string Target() const {
		return _targetName;
	}

	WindowInfo GetWindowInfo() {
		return _wndInfo;
	}

	void SetInjectorProcess(HANDLE hProcess) {
		_injectorProcess = hProcess;
	}

	void SetTargetProcess(HANDLE hProcess) {
		_targetProcess = hProcess;
	}

	void SetKeepaliveMutex(HANDLE mutex) {
		_keepaliveMutex = mutex;
	}

private:
	std::string _targetName;
	WindowInfo _wndInfo;

	HANDLE _injectorProcess;
	HANDLE _targetProcess;
	HANDLE _keepaliveMutex;

	bool _isCapturing;
};
