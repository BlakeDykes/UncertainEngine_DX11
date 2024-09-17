#pragma once

size_t TexelIndex(size_t pixelWidth, size_t width, size_t x, size_t y) { return pixelWidth * (y * width + x); }