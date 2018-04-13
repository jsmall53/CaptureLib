#include "CaptureInfo.h"

CaptureInfo::CaptureInfo()
{
	_wndInfo = { 0 };
	_targetName = std::string("");
}

CaptureInfo::CaptureInfo(std::string name)
{
	_wndInfo = { 0 };
	_targetName = name;
}

CaptureInfo::CaptureInfo(const char * name)
{
	_wndInfo = { 0 };
	_targetName = std::string(name);
}

CaptureInfo::~CaptureInfo()
{
	_wndInfo = { 0 };
}

int CaptureInfo::Initialize()
{
	if (_targetName.empty())
		return -1;

	if (!WindowsUtils::TryGetWindowInfo(&_wndInfo, _targetName)) {
		return -1;
	}

	return 0;
}

void CaptureInfo::Clear()
{
	_targetName.clear();
	_wndInfo = { 0 };
}