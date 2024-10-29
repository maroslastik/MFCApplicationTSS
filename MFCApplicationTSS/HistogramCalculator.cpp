#include "pch.h"
#include "HistogramCalculator.h"

void CalculateHistogramFromPixels(
    const uint8_t* pixels, int width, int height, int stride,
    std::array<uint32_t, 256>& histogramRed,
    std::array<uint32_t, 256>& histogramGreen,
    std::array<uint32_t, 256>& histogramBlue
) {
    histogramRed.fill(0);
    histogramGreen.fill(0);
    histogramBlue.fill(0);

    for (int y = 0; y < height; ++y) {
        const uint8_t* row = pixels + (y * stride);
        for (int x = 0; x < width; ++x) {
            uint8_t blue = row[x * 4 + 0];
            uint8_t green = row[x * 4 + 1];
            uint8_t red = row[x * 4 + 2];

            histogramRed[red]++;
            histogramGreen[green]++;
            histogramBlue[blue]++;
        }
    }
}