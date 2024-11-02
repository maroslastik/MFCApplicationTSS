#include "pch.h"
#include "CppUnitTest.h"
#include "../MFCApplicationTSS/HistogramCalculator.h"
#include "../MFCApplicationTSS/HistogramCalculator.cpp"
#include <vector>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
        TEST_METHOD(TestCalculateHistogramFromPixels_01)
        {
            const int width = 1;
            const int height = 1;
            const int stride = width * 4;

            // one blue pixel
            uint8_t pixels[] = {
                255, 0, 0, 0
            };

            std::array<uint32_t, 256> expectedRed;
            std::array<uint32_t, 256> expectedGreen;
            std::array<uint32_t, 256> expectedBlue;

            expectedRed.fill(0);
            expectedGreen.fill(0);
            expectedBlue.fill(0);

            expectedRed[0] = 1;
            expectedGreen[0] = 1;
            expectedBlue[255] = 1;

            std::array<uint32_t, 256> histogramRed;
            std::array<uint32_t, 256> histogramGreen;
            std::array<uint32_t, 256> histogramBlue;

            histogramRed.fill(0);
            histogramGreen.fill(0);
            histogramBlue.fill(0);

            CalculateHistogramFromPixels(pixels, width, height, stride, histogramRed, histogramGreen, histogramBlue);

            Assert::IsTrue(expectedRed == histogramRed);
            Assert::IsTrue(expectedGreen == histogramGreen);
            Assert::IsTrue(expectedBlue == histogramBlue);
        }

        TEST_METHOD(TestCalculateHistogramFromPixels_02)
        {
            const int width = 2;
            const int height = 2;
            const int stride = width * 4;

            uint8_t pixels[] = {
                0, 0, 255, 0,     // Red pixel
                0, 255, 0, 0,     // Green pixel
                255, 0, 0, 0,     // Blue pixel
                255, 255, 255, 0  // White pixel (all channels at 255)
            };

            std::array<uint32_t, 256> expectedRed;
            std::array<uint32_t, 256> expectedGreen;
            std::array<uint32_t, 256> expectedBlue;

            expectedRed.fill(0);
            expectedGreen.fill(0);
            expectedBlue.fill(0);

            expectedRed[0] = 2;
            expectedRed[255] = 2;
            expectedGreen[0] = 2;
            expectedGreen[255] = 2;
            expectedBlue[0] = 2;
            expectedBlue[255] = 2;

            std::array<uint32_t, 256> histogramRed;
            std::array<uint32_t, 256> histogramGreen;
            std::array<uint32_t, 256> histogramBlue;

            histogramRed.fill(0);
            histogramGreen.fill(0);
            histogramBlue.fill(0);

            CalculateHistogramFromPixels(pixels, width, height, stride, histogramRed, histogramGreen, histogramBlue);

            Assert::IsTrue(expectedRed == histogramRed);
            Assert::IsTrue(expectedGreen == histogramGreen);
            Assert::IsTrue(expectedBlue == histogramBlue);
        }

        TEST_METHOD(TestCalculateHistogramFromPixels_03)
        {
            const int width = 2;
            const int height = 2;
            const int stride = width * 4;

            uint8_t pixels[] = {
                128, 128, 128, 0,
                128, 128, 128, 0,
                128, 128, 128, 0,
                128, 128, 128, 0
            };

            std::array<uint32_t, 256> expectedRed;
            std::array<uint32_t, 256> expectedGreen;
            std::array<uint32_t, 256> expectedBlue;

            expectedRed.fill(0);
            expectedGreen.fill(0);
            expectedBlue.fill(0);

            expectedRed[128] = 4;
            expectedGreen[128] = 4;
            expectedBlue[128] = 4;

            std::array<uint32_t, 256> histogramRed;
            std::array<uint32_t, 256> histogramGreen;
            std::array<uint32_t, 256> histogramBlue;

            histogramRed.fill(0);
            histogramGreen.fill(0);
            histogramBlue.fill(0);

            CalculateHistogramFromPixels(pixels, width, height, stride, histogramRed, histogramGreen, histogramBlue);

            Assert::IsTrue(expectedRed == histogramRed);
            Assert::IsTrue(expectedGreen == histogramGreen);
            Assert::IsTrue(expectedBlue == histogramBlue);
        }
	};
}
