#include "pch.h"
#include "CppUnitTest.h"
#include "../MFCApplicationTSS/HistogramCalculator.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
        TEST_METHOD(TestCalculateHistogramFromPixels)
        {
            uint8_t pixels[] = {
                255, 0, 0,
                0, 255, 0,
                0, 0, 255
            };

            std::array<uint32_t, 256> histogramRed;
            std::array<uint32_t, 256> histogramGreen;
            std::array<uint32_t, 256> histogramBlue;

            CalculateHistogramFromPixels(pixels, 3, 1, 3, histogramRed, histogramGreen, histogramBlue);

            Assert::AreEqual(1U, histogramRed[255]);   // 1 red pixel
            Assert::AreEqual(1U, histogramGreen[255]); // 1 green pixel
            Assert::AreEqual(1U, histogramBlue[255]);  // 1 blue pixel
        }
	};
}
