#include "CaptureData.h"

static const char* SHMEM_NAME = "TEST_SHMEM123";

CaptureData::CaptureData()
{
}

CaptureData::CaptureData(CaptureMode mode, HWND window)
	:mode(mode)
{
	_name = SHMEM_NAME;
	InitSharedMemory(1);
}

CaptureData::~CaptureData()
{
	if (shmemObj != nullptr)
		delete shmemObj;
}

void CaptureData::InitSharedMemory(DWORD id)
{
	shmemObj = new shared_memory_object(
		open_or_create,
		_name,
		read_write
	);

	// TODO: Make sure this gets created with the correct size. Currently just 8 bytes (minimum i think).
	shmemObj->truncate(sizeof(this));
}

void CaptureData::CleanSharedMemory()
{
	shared_memory_object::remove(SHMEM_NAME);
}

HRESULT CaptureData::WriteToSharedMemory(void* data)
{
	if (data == NULL)
		return E_POINTER;

	mapped_region region(*shmemObj, read_write);
	std::memcpy(region.get_address(), data, region.get_size()); // TODO: figure out the size of data that I am writing?
	S_OK;
}

STDMETHODIMP Capture_Data::GetSize(size_t * size)
{
	*size = sizeof(data);
	return S_OK;
}

STDMETHODIMP Capture_Data::GetData(void** data)
{
	*data = &this->data;
	return S_OK;
}

