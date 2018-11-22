#pragma once
#include <Windows.h>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "CaptureCommon.h"

using namespace boost::interprocess;

struct GraphicsOffsets
{
	//dxgi
	//TODO: figure out what these do
	uint32_t present;
	uint32_t resize;
	uint32_t present1;
};

struct Capture_Data_struct
{
	CaptureMode mode;
	uint64_t frameInterval;
	uint32_t window;
	uint32_t width;
	uint32_t height;
	GraphicsOffsets gfxOffsets;
};

class WriteableData
{
	public:
		STDMETHOD(GetSize(size_t* size) = 0);
		STDMETHOD(GetData(void** data) = 0);
};

class TestData : public WriteableData
{
	public:
		STDMETHODIMP GetSize(size_t * size) override
		{
			*size = sizeof(this->data);
			return S_OK;
		}

		STDMETHODIMP GetData(void** data) override 
		{
			*data = &this->data;
			return S_OK;
		};

		std::string data = "TEST DATA";
};

class Capture_Data : public WriteableData
{
	// Inherited via WriteableData
	public:
		STDMETHOD(GetSize(size_t * size) override);
		STDMETHOD(GetData(void** data) override);
		Capture_Data_struct data;
};

class CaptureData
{
	public:
		CaptureData();
		CaptureData(CaptureMode mode, HWND window);
		~CaptureData();

		HRESULT WriteToSharedMemory(void* data);

		// INLINES
		mapping_handle_t GetShmemHandle() const
		{
			return shmemObj->get_mapping_handle();
		}

		CaptureMode GetMode() const
		{
			return mode;
		}
		
	private:
		void InitSharedMemory(DWORD id);
		void CleanSharedMemory();
		
		const char* _name;

		CaptureMode mode;
		UINT64 frameInterval;
		void* graphicsOffsets; // TODO: struct? class? dictionary? how do I want to store these guys

		shared_memory_object* shmemObj;
};
