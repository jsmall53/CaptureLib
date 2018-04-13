// Capture.cpp : Defines the entry point for the console application.
//
//
/*
Some notes regarding OBS code:
"on_module_load" function which gets called when loading the win-capture plugin uses
init_hooks to load the graphics offsets and setup the other capture sources (window cap, monitor cap).
Then the thread exits.
Once those are loaded...
...
...For "anti-cheat" the inject-helper takes the THREAD_ID instead of the PROCESS_ID
*/


#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <utils/WindowsUtils.h>
#include <CaptureInfo.h>
#include <GraphicsHook.h>

using namespace std;

static WindowInfo info = { 0 };
static const std::string PROCESS_NAME = "Path of Exile";

void InitHook(WindowInfo *info);
void ObsTryHook_and_InitHook();
void ObsInjectHook();

void TestFindWindow() {

	if (WindowsUtils::TryGetWindowInfo(&info, PROCESS_NAME)) {
		std::cout << info.executable;
		std::cout << ":";
		std::cout << info.process_id;
		std::cout << ":";
		std::cout << info.thread_id;
		std::cout << ":";
		std::cout << info.class_name;
		std::cout << ":";
		std::cout << info.title;
	}
	else
	{
		std::cout << "Unable to find window";
	}
}

void TestTryHook() {
	if (WindowsUtils::TryGetWindowInfo(&info, PROCESS_NAME)) {
		InitHook(&info);
	}
	else {
		//could not find the process don't bother trying to hook
		std::cout << "Could not hook process:";
		std::cout << PROCESS_NAME << std::endl;
	}
}

void TestCaptureInfo() {
	CaptureInfo *capInfo = new CaptureInfo(PROCESS_NAME);
	capInfo->Initialize();

	capInfo->Clear();
	capInfo->SetTarget(std::string(PROCESS_NAME));

	delete capInfo;
}

void TestGraphicsHook() {
	CaptureInfo *capInfo = new CaptureInfo(PROCESS_NAME);
	capInfo->Initialize();

	GraphicsHook* hook = new GraphicsHook(capInfo);
	hook->Initialize();
	hook->Inject();
}

void InitHook(WindowInfo *info) {
	//open the process
	HANDLE hProc = WindowsUtils::Open_Process(info->process_id, PROCESS_QUERY_INFORMATION | SYNCHRONIZE);
	//create a keep alive mutex. Why?

	//init a pipe for ????

	//Injects the hook if there isn't one created already. InjectHook();
}

void ObsTryHook_and_InitHook() {
	/**********************
	the try_hook() method in game_capture.c, gets the window process/thread ID information,
	then calls init_hook, which gets more of the window information and then
	1. opens the process
	2. Creates a keepalive Mutex
	3. Initializes a pipe for ????
	4. Checks if theres already a hook, if not
	5. Injects the hook into the process
	**********************/
}

void ObsInjectHook()
{
	/**********************
	The inject_hook() method:
	1. gets the "graphics-hook64.dll"
	2. Checks the integrity of our two helper executables
	3. Creates the inject process by calling CreateProcessW for "inject-helper64.exe"
		Success
	**********************/
}

int main()
{
	TestFindWindow();
	TestTryHook();
	TestCaptureInfo();
	TestGraphicsHook();
	//********
	string end;
	getline(cin, end);
	return 0;
}