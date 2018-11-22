#pragma once
#include <Windows.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "ICaptureEngine.h"

using namespace std;
using namespace cv;

class WindowCapture
{
	public:
		WindowCapture();
		~WindowCapture();

		bool Initialize(HWND hWnd, double captureRate = 60.0f);
		bool InitializeOld(HWND hWnd, double captureRate = 60.0f);
		void Release(bool force = false);
		cv::Mat Capture();
		cv::Mat CaptureBitBlt();

	private:

		HWND _hWnd;
		HDC _hDC;
		HDC _hDCSrc;
		BITMAPINFO _bitmapInfo;
		BITMAPINFOHEADER _bitmapInfoHeader;
		
		HBITMAP hBitmap;
		HBITMAP _hOldBitmap; // Why do they keep track?
		BYTE* _bits;
		cv::Mat _mat;
		
		RECT _srcWindowSize;
		int _height, _width;
		int _outputHeight, _outputWidth;
		int _srcHeight, _srcWidth;

		double _captureRate;
};