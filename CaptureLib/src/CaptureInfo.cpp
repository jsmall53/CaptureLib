#include "CaptureInfo.h"

CaptureInfo::CaptureInfo()
{
	_wndInfo = { 0 };
	_targetName = std::string("");
	Initialize();
}

CaptureInfo::CaptureInfo(std::string name)
{
	_wndInfo = { 0 };
	_targetName = name;
	Initialize();
}

CaptureInfo::CaptureInfo(const char * name)
{
	_wndInfo = { 0 };
	_targetName = std::string(name);
	Initialize();
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

bool CaptureInfo::OpenTarget()
{
	if (_targetName.empty()) {
		//TODO: LOG ERROR: no target set
		return false;
	}

	_targetProcess = WindowsUtils::Open_Process(_wndInfo.process_id, PROCESS_QUERY_INFORMATION | SYNCHRONIZE);
	if (_targetProcess == NULL) {
		return false;
	}

	return true;
}
