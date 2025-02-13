#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <utility>
#include "GLFWImage.h"

class Filter {
public:
    static void InvertColors(unsigned char* data, int width, int height, int channels);
    static void Grayscale(unsigned char* data, int width, int height, int channels);
    static int Brightness(unsigned char* data, int width, int height, int channels, int brightness, int prevNum);
    static void Contrast(unsigned char* data, int width, int height, int channels, float contrast);
    static void AdjustWhites(unsigned char* data, int width, int height, int channels, int adjustment);
    static void AdjustBlacks(unsigned char* data, int width, int height, int channels, int adjustment);
    static void NBrightness(unsigned char* srcData, unsigned char* destData, int width, int height, int channels, int brightness);
    static void AdjustBrightness(unsigned char* pixels, int width, int height, int channels, unsigned char* pixels1, int brightness);
    //static void Filter::AdjustBrightness(GLFWImage& image, int brightness);

};
