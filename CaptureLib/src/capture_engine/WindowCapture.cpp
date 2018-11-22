#include "WindowCapture.h"
#include <iostream>

// TODO: Clean this up and add error handling
// TODO: Add in _captureRate code and figure out a better way to output _mat so the consumer doesn't have to care about setting and keeping in sync with the _captureRate

WindowCapture::WindowCapture()
{
}

WindowCapture::~WindowCapture()
{
	Release();
	if (_bits != nullptr)
		delete _bits;
}

bool WindowCapture::InitializeOld(HWND hWnd, double captureRate)
{
	if (hWnd == nullptr)
	{
		return false;
	}

	_hWnd = hWnd;
	_captureRate = captureRate;

	_hDCSrc = GetDC(hWnd);
	_hDC = CreateCompatibleDC(_hDCSrc);
	GetClientRect(hWnd, &_srcWindowSize);

	_srcHeight = _srcWindowSize.bottom;
	_srcWidth = _srcWindowSize.right;
	_outputHeight = _srcWindowSize.bottom / 1;
	_outputWidth = _srcWindowSize.right / 1;

	_mat.create(_outputHeight, _outputWidth, CV_8UC4);

	_bitmapInfo = { 0 };
	BITMAPINFOHEADER* bitmapInfoHeader = &_bitmapInfo.bmiHeader;
	bitmapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader->biWidth = _outputWidth;
	bitmapInfoHeader->biHeight = -_outputHeight;
	bitmapInfoHeader->biPlanes = 1;
	bitmapInfoHeader->biBitCount = 32;
	
	// _mat.data is already allocated here, will that be an issue?
	hBitmap = CreateDIBSection(_hDC, &_bitmapInfo, DIB_RGB_COLORS, (void**)&_mat.data, NULL, 0);

	SelectObject(_hDC, hBitmap);

	return true;
}

bool WindowCapture::Initialize(HWND hWnd, double captureRate /*= 60.0f*/)
{
	if (hWnd == nullptr)
	{
		return false;
	}

	_hWnd = hWnd;
	_hDCSrc = GetDC(hWnd);
	_hDC = CreateCompatibleDC(_hDCSrc);
	SetStretchBltMode(_hDC, COLORONCOLOR);
	GetClientRect(hWnd, &_srcWindowSize);

	_srcHeight = _srcWindowSize.bottom;
	_srcWidth = _srcWindowSize.right;
	_outputHeight = _srcWindowSize.bottom / 1;
	_outputWidth = _srcWindowSize.right / 1;

	_mat.create(_outputHeight, _outputWidth, CV_8UC4);

	hBitmap = CreateCompatibleBitmap(_hDCSrc, _outputWidth, _outputHeight);
	_bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bitmapInfoHeader.biWidth = _outputWidth;
	_bitmapInfoHeader.biHeight = -_outputHeight;
	_bitmapInfoHeader.biPlanes = 1;
	_bitmapInfoHeader.biBitCount = 32;
	_bitmapInfoHeader.biCompression = BI_RGB;
	_bitmapInfoHeader.biSizeImage = 0;
	_bitmapInfoHeader.biXPelsPerMeter = 0;
	_bitmapInfoHeader.biYPelsPerMeter = 0;
	_bitmapInfoHeader.biClrUsed = 0;
	_bitmapInfoHeader.biClrImportant = 0;

	SelectObject(_hDC, hBitmap);

	return true;
}

void WindowCapture::Release(bool force /*= false*/)
{
	DeleteObject(hBitmap); 
	DeleteDC(_hDC); 
	ReleaseDC(_hWnd, _hDCSrc);
}

cv::Mat WindowCapture::Capture()
{
	StretchBlt(_hDC, 0, 0, _outputWidth, _outputHeight, _hDCSrc, 0, 0, _srcWidth, _srcHeight, SRCCOPY);
	GetDIBits(_hDC, hBitmap, 0, _outputHeight, _mat.data, (BITMAPINFO *)&_bitmapInfoHeader, DIB_RGB_COLORS);
	return _mat;
}

cv::Mat WindowCapture::CaptureBitBlt()
{
	BitBlt(_hDC, 0, 0, _outputWidth, _outputHeight, _hDCSrc, 0, 0, SRCCOPY);
	return _mat;
}
