#include "ColorAdjustmentUI.h"
#include <algorithm>

ColorAdjustmentUI::ColorAdjustmentUI()
    : current_mode(AdjustmentMode::Color), selected_color_index(0), color_names{ "Red", "Orange", "Yellow", "Green", "Aqua", "Blue", "Purple", "Magenta" }      
{

}

void ColorAdjustmentUI::RenderColorCircles()
{
    float slider_width = ImGui::CalcItemWidth();
    float circle_radius = 21.5f;
    int num_circles = 8;

    for (int i = 0; i < num_circles; ++i)
    {
        if (i > 0) ImGui::SameLine();
        ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Default to red, should be set based on color_names[i]

        // Set the actual color based on color_names[i]
        if (strcmp(color_names[i], "Red") == 0)
            color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        else if (strcmp(color_names[i], "Orange") == 0)
            color = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        else if (strcmp(color_names[i], "Yellow") == 0)
            color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        else if (strcmp(color_names[i], "Green") == 0)
            color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        else if (strcmp(color_names[i], "Aqua") == 0)
            color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
        else if (strcmp(color_names[i], "Blue") == 0)
            color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
        else if (strcmp(color_names[i], "Purple") == 0)
            color = ImVec4(0.5f, 0.0f, 0.5f, 1.0f);
        else if (strcmp(color_names[i], "Magenta") == 0)
            color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);

        ImGui::PushID(i);
        if (ImGui::InvisibleButton("##circle", ImVec2(circle_radius * 2, circle_radius * 2)))
        {
            selected_color_index = i;
        }
        ImVec2 p = ImGui::GetItemRectMin();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        if (selected_color_index == i)
        {
            draw_list->AddCircleFilled(ImVec2(p.x + circle_radius, p.y + circle_radius), circle_radius, ImGui::ColorConvertFloat4ToU32(color));
        }
        else
        {
            draw_list->AddCircle(ImVec2(p.x + circle_radius, p.y + circle_radius), circle_radius, ImGui::ColorConvertFloat4ToU32(color), 12, 2.0f);
        }
        ImGui::PopID();
    }
    ImGui::NewLine();
}

void ColorAdjustmentUI::Render(float* hueShift, float* satShift, float* lumShift, float* hueLeft, float* hueRight)
{
    ImGui::Begin("Adjustments");

    if (ImGui::Button("Color")) current_mode = AdjustmentMode::Color;
    ImGui::SameLine();
    if (ImGui::Button("Hue")) current_mode = AdjustmentMode::Hue;
    ImGui::SameLine();
    if (ImGui::Button("Saturation")) current_mode = AdjustmentMode::Saturation;
    ImGui::SameLine();
    if (ImGui::Button("Luminance")) current_mode = AdjustmentMode::Luminance;

    ImGui::Separator();

    switch (current_mode)
    {
    case AdjustmentMode::Color:
        ImGui::NewLine();
        ImGui::NewLine();
        RenderColorCircles();
        ImGui::Separator();
        ImGui::PushItemWidth(400.0f);

        RenderHueSlider("Hue", hueShift[selected_color_index], hueLeft[selected_color_index] * 360.0f, hueRight[selected_color_index] * 360.0f);
        RenderSatSlider("Saturation", satShift[selected_color_index], hueLeft[selected_color_index] * 360.0f + 30.0f);
        RenderLumSlider("Luminance", lumShift[selected_color_index], hueLeft[selected_color_index] * 360.0f + 30.0f);
        break;

    case AdjustmentMode::Hue:
        ImGui::Text("Adjust Hue:");
        ImGui::PushItemWidth(400.0f);
        for (int i = 0; i < 8; i++)
        {
            RenderHueSlider(color_names[i], hueShift[i], hueLeft[i] * 360.0f, hueRight[i] * 360.0f);
        }
        break;

    case AdjustmentMode::Saturation:
        ImGui::Text("Adjust Saturation:");
        ImGui::PushItemWidth(400.0f);
        for (int i = 0; i < 8; i++)
        {
            RenderSatSlider(color_names[i], satShift[i], hueLeft[i] * 360.0f + 30.0f);
        }
        break;

    case AdjustmentMode::Luminance:
        ImGui::Text("Adjust Luminance:");
        ImGui::PushItemWidth(400.0f);
        for (int i = 0; i < 8; i++)
        {
            RenderLumSlider(color_names[i], lumShift[i], hueLeft[i] * 360.0f + 30.0f);
        }
        break;
    }
    ImGui::NewLine();
    ImGui::End();
}

// Function to render the HSL slider with a specified hue range
//float RenderHueSlider(const char* label, float& hue, float hueLeft, float hueRight)
void ColorAdjustmentUI::RenderHueSlider(const char* label, float& hue, float hueLeft, float hueRight)

{
    ImGui::Text(label);

    // Generate the gradient colors for the specified hue range
    static const int gradient_steps = 100;
    std::vector<ImVec4> colors;
    for (int i = 0; i <= gradient_steps; ++i)
    {
        float t = static_cast<float>(i) / gradient_steps;
        float h;
        if (hueLeft > hueRight) {
            // Handle hue wrapping around 360 to 0
            h = hueLeft + t * (360.0f - hueLeft + hueRight);
            if (h >= 360.0f) h -= 360.0f;
        }
        else {
            h = hueLeft + t * (hueRight - hueLeft);
        }
        colors.push_back(HSLToRGB(h, 1.0f, 0.5f));
    }

    // Draw the gradient as the slider background
    ImVec2 p = ImGui::GetCursorScreenPos();
    float width = ImGui::CalcItemWidth();
    float height = 20.0f;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for (int i = 0; i < gradient_steps; ++i)
    {
        float t = static_cast<float>(i) / gradient_steps;
        ImVec2 a = ImVec2(p.x + t * width, p.y);
        ImVec2 b = ImVec2(p.x + (t + 1.0f / gradient_steps) * width, p.y + height);
        draw_list->AddRectFilled(a, b, ImGui::ColorConvertFloat4ToU32(colors[i]));
    }

    // Draw the slider overlay
    ImGui::SetCursorScreenPos(p);
    ImGui::InvisibleButton(label, ImVec2(width, height));
    ImGui::SetItemAllowOverlap();
    float slider_pos = (hue + 100.0f) / 200.0f; // Normalize hue to 0.0f - 1.0f
    ImVec2 slider_pos_pixel = ImVec2(p.x + slider_pos * width, p.y + height / 2);
    draw_list->AddCircleFilled(slider_pos_pixel, 5.0f, IM_COL32(255, 255, 255, 255));

    if (ImGui::IsItemActive())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        float mouse_pos = (ImGui::GetIO().MousePos.x - p.x) / width;
        hue = mouse_pos * 200.0f - 100.0f;
        if (hue < -100.0f) hue = -100.0f;
        if (hue > 100.0f) hue = 100.0f;

    }
    ImGui::SameLine();
    if (hue >= 1)
    {
        ImGui::Text("+%.0f", hue);
    }
    else if (hue <= -1)
    {
        ImGui::Text("%.0f", hue);
    }
    else
    {
        ImGui::Text("0");
    }
}

void ColorAdjustmentUI::RenderSatSlider(const char* label, float& sat, float hue)
{
    ImGui::Text(label);

    // Generate the gradient colors for the specified hue range
    static const int gradient_steps = 100;
    std::vector<ImVec4> colors;

    // Generate the gradient colors for the saturation slider
    colors.clear();
    for (int i = 0; i <= gradient_steps; ++i)
    {
        float t = static_cast<float>(i) / gradient_steps;
        ImVec4 color = HSLToRGB(hue, t, 0.5f); // Fixed luminance at 0.5
        colors.push_back(color);
    }

    // Draw the gradient as the slider background
    ImVec2 p = ImGui::GetCursorScreenPos();
    float width = ImGui::CalcItemWidth();
    float height = 20.0f;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Draw the saturation gradient background
    p = ImGui::GetCursorScreenPos();
    width = ImGui::CalcItemWidth();
    draw_list = ImGui::GetWindowDrawList();

    for (int i = 0; i < gradient_steps; ++i)
    {
        float t = static_cast<float>(i) / gradient_steps;
        ImVec2 a = ImVec2(p.x + t * width, p.y);
        ImVec2 b = ImVec2(p.x + (t + 1.0f / gradient_steps) * width, p.y + height);
        draw_list->AddRectFilled(a, b, ImGui::ColorConvertFloat4ToU32(colors[i]));
    }

    // Draw the saturation slider overlay
    ImGui::SetCursorScreenPos(p);
    ImGui::InvisibleButton((std::string(label) + " Saturation").c_str(), ImVec2(width, height));
    ImGui::SetItemAllowOverlap();
    float slider_pos = (sat + 100.0f) / 200.0f; // Normalize saturation shift to 0.0f - 1.0f
    ImVec2  slider_pos_pixel = ImVec2(p.x + slider_pos * width, p.y + height / 2);
    draw_list->AddCircleFilled(slider_pos_pixel, 5.0f, IM_COL32(255, 255, 255, 255));

    if (ImGui::IsItemActive())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        float mouse_pos = (ImGui::GetIO().MousePos.x - p.x) / width;
        sat = mouse_pos * 200.0f - 100.0f;
        if (sat < -100.0f) sat = -100.0f;
        if (sat > 100.0f) sat = 100.0f;
    }

    // Display the saturation shift value next to the slider
    ImGui::SameLine();
    if (sat > 0)
        ImGui::Text("+%.1f", sat);
    else if (sat < 0)
        ImGui::Text("%.1f", sat);
    else
        ImGui::Text("0");
}

void ColorAdjustmentUI::RenderLumSlider(const char* label, float& lum, float hue)
{
    ImGui::Text(label);

    // Generate the gradient colors for the specified hue range
    static const int gradient_steps = 100;
    std::vector<ImVec4> colors;

    // Generate the gradient colors for the saturation slider
    colors.clear();
    for (int i = 0; i <= gradient_steps; ++i)
    {
        float t = static_cast<float>(i) / gradient_steps;
        ImVec4 color = HSLToRGB(hue, 1.0f, t); // Fixed luminance at 0.5
        colors.push_back(color);
    }

    // Draw the gradient as the slider background
    ImVec2 p = ImGui::GetCursorScreenPos();
    float width = ImGui::CalcItemWidth();
    float height = 20.0f;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Draw the saturation gradient background
    p = ImGui::GetCursorScreenPos();
    width = ImGui::CalcItemWidth();
    draw_list = ImGui::GetWindowDrawList();

    for (int i = 0; i < gradient_steps; ++i)
    {
        float t = static_cast<float>(i) / gradient_steps;
        ImVec2 a = ImVec2(p.x + t * width, p.y);
        ImVec2 b = ImVec2(p.x + (t + 1.0f / gradient_steps) * width, p.y + height);
        draw_list->AddRectFilled(a, b, ImGui::ColorConvertFloat4ToU32(colors[i]));
    }

    // Draw the saturation slider overlay
    ImGui::SetCursorScreenPos(p);
    ImGui::InvisibleButton((std::string(label) + " Luminance").c_str(), ImVec2(width, height));
    ImGui::SetItemAllowOverlap();
    float slider_pos = (lum + 100.0f) / 200.0f; // Normalize saturation shift to 0.0f - 1.0f
    ImVec2  slider_pos_pixel = ImVec2(p.x + slider_pos * width, p.y + height / 2);
    draw_list->AddCircleFilled(slider_pos_pixel, 5.0f, IM_COL32(255, 255, 255, 255));

    if (ImGui::IsItemActive())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        float mouse_pos = (ImGui::GetIO().MousePos.x - p.x) / width;
        lum = mouse_pos * 200.0f - 100.0f;
        if (lum < -100.0f) lum = -100.0f;
        if (lum > 100.0f) lum = 100.0f;
    }

    // Display the saturation shift value next to the slider
    ImGui::SameLine();
    if (lum > 0)
        ImGui::Text("+%.1f", lum);
    else if (lum < 0)
        ImGui::Text("%.1f", lum);
    else
        ImGui::Text("0");
}

// Function to convert HSL to RGB
ImVec4 ColorAdjustmentUI::HSLToRGB(float h, float s, float l)
{
    float c = (1.0f - std::fabs(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - std::fabs(fmod(h / 60.0f, 2) - 1.0f));
    float m = l - c / 2.0f;
    float r, g, b;

    if (h >= 0 && h < 60) { r = c; g = x; b = 0; }
    else if (h >= 60 && h < 120) { r = x; g = c; b = 0; }
    else if (h >= 120 && h < 180) { r = 0; g = c; b = x; }
    else if (h >= 180 && h < 240) { r = 0; g = x; b = c; }
    else if (h >= 240 && h < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }

    return ImVec4(r + m, g + m, b + m, 1.0f);
}
