// Capture.cpp : Defines the entry point for the console application.
//
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <utils/WindowsUtils.h>

void TestFindPOEWindow() {
	Utils::WindowInfo info = { 0 };
	std::string name = "Path of Exile";

	if (Utils::WindowsUtils::TryGetWindowInfo(&info, name)) {
		std::cout << info.executable;
		std::cout << ":";
		std::cout << info.class_name;
		std::cout << ":";
		std::cout << info.title;
	}
	else {
		std::cout << "Unable to find window";
	}
}

int main()
{
	TestFindPOEWindow();
    return 0;
}



  