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

Some overall TODOS:
-Remove the executable obs dependencies (aka rewrite them to be built into this library)
-possibly remove the need for the hooking dll
-Finish the Graphics Hook class. Still needs a way to "close" the hook, and recover from failures
-Learn more about dll injection and its problems
-Thread this stuff, what im thinking:
	main initialization/management
	one listener thread for data retrieval
	????

-Look at copy_shmem_tex => pretty sure this is whats actually grabbing the data every tick
*/


#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <assert.h>
#include <utils/WindowsUtils.h>
#include <utils/Utils.h>
#include <CaptureInfo.h>
#include <GraphicsHook.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "capture_engine/CaptureData.h"
#include "capture_engine/WindowCapture.h"

using namespace std;
using namespace cv;

static WindowInfo info = { 0 };
static const std::string PROCESS_NAME = "Overwatch";

typedef std::pair<double, int> Mytype;

class MyClass
{
	double dbl;
	int num;
};

void ObsInitHook(WindowInfo *info);
void ObsTryHook_and_InitHook();
void ObsInjectHook();

void TestFindWindow() {
	if (WindowsUtils::TryGetWindowInfo(PROCESS_NAME, &info)) {
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
	auto result = hook->TryHook();
	std::cout << "TryHook result: " << result;
}

void ObsInitHook(WindowInfo *info) {
	//open the process
	//create a keep alive mutex. Why?
	//init a pipe for ????
	//Injects the hook if there isn't one created already. InjectHook();
	//init texture mutexes, these are for rendering to owned texture objects
	//init hook info
	//init events
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

Mat hwnd2mat(HWND hwnd)
{

	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
	width = windowsize.right / 1;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	// avoid memory leak
	DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(hwnd, hwindowDC);

	return src;
}

void TestCapture()
{
	auto key = 0;
	auto desktop = GetDesktopWindow();
	auto capture = WindowCapture();
	capture.InitializeOld(info.window);
	while (key != 27)
	{
		auto frame = capture.CaptureBitBlt();
		//auto frame = hwnd2mat(desktop);
		imshow("output", frame);
		key = cv::waitKey(10);
	}
}

int main()
{
	TestFindWindow();
	TestCaptureInfo();
	TestGraphicsHook();
	TestCapture();
	//********
	//IntToList(4321);
	string end;
	getline(cin, end);

	return 0;
}
