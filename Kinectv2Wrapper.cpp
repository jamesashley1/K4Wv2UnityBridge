#include "Kinectv2Wrapper.h"

IKinectSensor* context;
IColorFrameReader* m_pColorFrameReader;
byte *pBuffer = NULL;
UINT nBufferSize = 0;
RGBQUAD* m_pColorRGBX;
static const int cColorWidth = 1920;
static const int cColorHeight = 1080;


byte* Get_Color()
{
	return pBuffer;
}

bool Kinect_Init()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&context);
	if (FAILED(hr))
	{
		return false;
	}

	if (context)
	{
		// Initialize the Kinect and get the color reader
		IColorFrameSource* pColorFrameSource = NULL;
		pBuffer = new byte[cColorWidth * cColorHeight * 4];
		hr = context->Open();

		if (SUCCEEDED(hr))
		{
			hr = context->get_ColorFrameSource(&pColorFrameSource);
		}
		else{
			return false;
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
		}

		if (pColorFrameSource)
		{
			pColorFrameSource->Release();
			pColorFrameSource = NULL;
		}

	}

	if (!context || FAILED(hr))
	{
		return false;
	}

	return true;

}

bool Kinect_Deinit(){

	if (m_pColorFrameReader)
	{
		m_pColorFrameReader->Release();
		m_pColorFrameReader = NULL;
	}

	if (context)
	{
		context->Close();
	}
	if (context)
	{
		context->Release();
		context = NULL;
	}
	return true;
}

int Poll_Color(){
	ColorImageFormat imageFormat = ColorImageFormat_Bgra;

	if (!m_pColorFrameReader)
	{
		return 0;
	}

	IColorFrame* pColorFrame = NULL;
	HRESULT hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);


	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;
		IFrameDescription* pFrameDescription = NULL;
		int nWidth = 0;
		int nHeight = 0;



		if (SUCCEEDED(hr))
		{

			hr = pColorFrame->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr))
		{

			hr = pFrameDescription->get_Width(&nWidth);
		}

		if (SUCCEEDED(hr))
		{

			hr = pFrameDescription->get_Height(&nHeight);
		}

		if (SUCCEEDED(hr))
		{

			hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
		}

		if (SUCCEEDED(hr))
		{
				nBufferSize = cColorWidth * cColorHeight * 4;
				hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, pBuffer, ColorImageFormat_Bgra);
		}

	}

	if (pColorFrame)
	{
		pColorFrame->Release();
		pColorFrame = NULL;
	}
	return (int)nBufferSize;
}

