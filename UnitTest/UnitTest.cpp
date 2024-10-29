#include "pch.h"
#include "CppUnitTest.h"
#include "HistogramCalculator.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
        TEST_METHOD(TestCalculateHistogramFromPixels)
        {
            // Príklad pixelov (ARGB)
            uint8_t pixels[] = {
                255, 0, 0, 255, // Red
                0, 255, 0, 255, // Green
                0, 0, 255, 255, // Blue
                0, 0, 0, 255    // Black
            };

            std::array<uint32_t, 256> histogramRed;
            std::array<uint32_t, 256> histogramGreen;
            std::array<uint32_t, 256> histogramBlue;

            // Zavolaj funkciu na výpočet histogramu
            CalculateHistogramFromPixels(pixels, 2, 2, 8, histogramRed, histogramGreen, histogramBlue);

            // Over, že histogramy sú správne
            Assert::AreEqual(1U, histogramRed[255]);   // 1 red pixel
            Assert::AreEqual(1U, histogramGreen[255]); // 1 green pixel
            Assert::AreEqual(1U, histogramBlue[255]);  // 1 blue pixel
            Assert::AreEqual(1U, histogramRed[0]);     // 1 black pixel
        }
	};
}
