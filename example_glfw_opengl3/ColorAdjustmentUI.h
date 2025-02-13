#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include <string>
#include <vector>
#include "Application.h"

class ColorAdjustmentUI
{
public:
    enum class AdjustmentMode
    {
        Color,
        Hue,
        Saturation,
        Luminance
    };

    ColorAdjustmentUI();

    void Render(float*, float*, float*, float*, float*);
    void GetShifts(float* hue_shifts, float* sat_shifts, float* lum_shifts);
    void RenderHueSlider(const char* label, float& hue, float hueLeft, float hueRight);
    void RenderSatSlider(const char* label, float& sat, float hue);
    void RenderLumSlider(const char* label, float& lum, float hue);
    void ColorAdjustmentUI::RenderColorCircles();
    ImVec4 ColorAdjustmentUI::HSLToRGB(float h, float s, float l);


private:
    AdjustmentMode current_mode;
    int selected_color_index;
    const char* color_names[8];
    
};
