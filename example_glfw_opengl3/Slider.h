#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include <string>


class Slider {
public:
    Slider(const char* label, float min, float max);

    bool Render(float* value, int id);

private:
    const char* label;
    float min;
    float max;
};
