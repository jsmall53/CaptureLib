// Capture.cpp : Defines the entry point for the console application.
//
//
/*
Some notes regarding OBS code:
"on_module_load" function which gets called when loading the win-capture plugin uses
init_hooks to load the graphics offsets and setup the other capture sources (window cap, monitor cap).
Then the thread exits.
Once those are loaded...
*/


#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <utils/WindowsUtils.h>

void TestFindPOEWindow() {
	Utils::WindowInfo info = { 0 };
	std::string name = "Calculator";

	if (Utils::WindowsUtils::TryGetWindowInfo(&info, name)) {
		std::cout << info.executable;
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

int main()
{
	TestFindPOEWindow();
    return 0;
}



  