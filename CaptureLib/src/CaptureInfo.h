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

	std::string GetName() const {
		return _targetName;
	}

	WindowInfo GetWindowInfo() const {
		return _wndInfo;
	}

private:
	std::string _targetName;
	WindowInfo _wndInfo;

	bool _hooked;
	bool _isCapturing;
};