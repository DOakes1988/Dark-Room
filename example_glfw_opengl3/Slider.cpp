#include "Slider.h"

Slider::Slider(const char* label, float min, float max)
    : label(label), min(min), max(max) {}

bool Slider::Render(float* value, int id) {
    ImGui::PushID(id); // Push a unique identifier for this slider

    ImGui::BeginGroup(); // Lock horizontal position

    ImGui::TextUnformatted(label);
    ImGui::SameLine();

    // Check if the label is hovered and the value is not zero
    if (ImGui::IsItemHovered() && *value != 0.0f) {
        
        if (ImGui::IsItemClicked()) {
            *value = 0.0f; // Reset the value to zero
        }
        ImGui::TextUnformatted("/ RESET");
    }
   
    // Calculate the width of the slider
    float sliderWidth = ImGui::CalcItemWidth();
    float inputTextWidth = ImGui::CalcTextSize("+0.00").x + ImGui::GetStyle().FramePadding.x * 2;

    // Align the value input field to the right side of the slider and grow to the left
    ImGui::SameLine(sliderWidth - 40.0f);

    // Render the value as an input text to make it clickable
    char valueStr[32];
    if (*value > 0.0f) {
        snprintf(valueStr, 32, "+%.2f", *value);
    }
    else {
        snprintf(valueStr, 32, "%.2f", *value);
    }

    // Calculate the width of the current value string
    float valueWidth = ImGui::CalcTextSize(valueStr).x + ImGui::GetStyle().FramePadding.x * 2;

    // Set cursor position to keep the right edge aligned
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - valueWidth + inputTextWidth);


    // Save the current style colors
    ImVec4 originalFrameBg = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
    ImVec4 originalFrameBgHovered = ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered);
    ImVec4 originalFrameBgActive = ImGui::GetStyleColorVec4(ImGuiCol_FrameBgActive);

    // Set the input text background to be transparent
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));

    ImGui::PushItemWidth(60); // Adjust width for input box
    if (ImGui::InputText("##value", valueStr, IM_ARRAYSIZE(valueStr), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll)) {
        *value = strtof(valueStr, NULL);
    }
    ImGui::PopItemWidth();

    // Restore the original style colors
    ImGui::PopStyleColor(3);

    // Render the slider on the next line
    ImGui::SliderFloat("##slider", value, min, max, ""); // Empty label to avoid displaying value within the slider

    ImGui::EndGroup();
    ImGui::PopID(); // Pop the unique identifier for this slider
    return true;
}

