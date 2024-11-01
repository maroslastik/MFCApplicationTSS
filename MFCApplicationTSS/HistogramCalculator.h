#pragma once
#ifndef HISTOGRAM_CALCULATOR_H
#define HISTOGRAM_CALCULATOR_H

#include <array>
#include <cstdint>

void CalculateHistogramFromPixels(
    const uint8_t* pixels,
    int width,
    int height,
    int stride,
    std::array<uint32_t, 256>& histogramRed,
    std::array<uint32_t, 256>& histogramGreen,
    std::array<uint32_t, 256>& histogramBlue
);

#endif // HISTOGRAM_CALCULATOR_H