#pragma once
#include <windows.h>
#include <CaptureInfo.h>

struct node {
	int val;
	node* next;
};

static bool CheckFileIntegrity(CaptureInfo *info,
	std::string filePath,
	std::string key);

void IntToList(int num);

bool SumEquals(int vals[], int target);