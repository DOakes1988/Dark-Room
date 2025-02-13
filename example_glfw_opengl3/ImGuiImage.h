#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include "ComputeShader.h"
#include "Common.h"
#include <vector>
#include "imgui_internal.h"
#include <cmath>

class ImGuiImage {
public:
    
    float gradient_intensity = 1.0f;

    ImGuiImage(int w, int h, int c);
    ImGuiImage(const ImGuiImage& other);
    ~ImGuiImage();

    ImGuiImage& operator=(const ImGuiImage& other);
    static ImGuiImage LoadFromFile(const std::string& filePath);
    GLuint ImGuiImage::LoadTexture(const char* filename);
    void ImGuiImage::LoadTextures();
    
    void CopyPixelsFrom(const ImGuiImage& other);
    void AdjustBrightness(float b);
    void AdjustContrast(float c);
    void AdjustWhite(float w);
    void AdjustBlack(float b);
    void AdjustShadows(float s);
    void AdjustSaturation(float s);

    void AdjustFilterBrightness(float fb);
    void AdjustFilterContrast(float fc);
    void AdjustFilterWhite(float fw);
    void AdjustFilterBlack(float fb);
    void AdjustFilterShadows(float fs);
    void AdjustFilterSaturation(float fs);

    void ImGuiImage::AdjustHSL(float hue_shifts[], float sat_shifts[], float lum_shifts[], float hue_left[], float hue_right[]);

    void RenderHistogram();

    void RenderImGui(const ImVec2& size);
    void RenderImGui(const ImVec2& size, ImVec2& uv0, ImVec2& uv1);
    void RenderCenteredImage(float original_width, float original_height, float window_width, float window_height, float padding, ImVec2 uv0, ImVec2 uv1, float zoom_level);
    void HandleZoom(ImVec2 click_pos, ImVec2 image_pos, ImVec2 image_size, ImVec2& uv0, ImVec2& uv1, float& zoom_level, int& zoom_factor, bool& is_zoomed);

    void UploadToGPU();
    GLuint GetTexture() const;
    void LinearGradientFilter(ImGuiImage& img, ImVec2 size);
    void UpdateLinePoints();
    ImVec2 SetWindowSize(ImVec2 size);
    bool IsValid() const;

private:
    int width;
    int height;
    int channels;
    float brightness;
    float contrast;
    float white;
    float black;
    float shadows;
    float saturate;
    float hue;
    float saturation;
    float luminance;

    float filterBrightness;
    float filterContrast;
    float filterWhite;
    float filterBlack;
    float filterSaturate;
    float filterShadows;

    float angle;
    float texWidth;
    float texHeight;



    bool drawing;
    bool apply_above;
    bool filter_mode;
    bool line_drawn;
    bool isDragging;

    bool selecting_line;
    bool line_transparent;
    bool hover_line;
    bool hover_middle;

    bool hover_rotate_left;
    bool hover_rotate_right;
    bool isRotatingLeft;
    bool isRotatingRight;
    bool isMiddleDrag;
    bool isMousePressed;

    bool isHovered;
    bool zoomFlag;
    bool lineFlag;

    unsigned char* pixels;
    unsigned char* originalPixels;

    ImVec2 initial_line_start = ImVec2(0, 0);
    ImVec2 initial_line_end = ImVec2(0, 0);

    GLuint texture, copyTexture;
    GLuint open_hand_texture;
    ImVec2 textureSize = ImVec2(10, 10); 
    GLuint histRedTex, histGreenTex, histBlueTex, histLuminanceTex;
    GLuint histBuffer;

    ComputeShader* adjustmentShader;
    ComputeShader* histogramShader;
    ComputeShader* linearGradientFilterShader;

    ImVec2 line_start, line_end;
    ImVec2 image_pos;
    ImVec2 prev_image_size = ImVec2(0, 0);
    ImVec2 center;
    ImVec2 windowSize = { 0, 0 };
    ImVec2 imageSize = { 0, 0 };
    ImVec2 initial_mouse_pos;


    
    

    void useComputeShader(ComputeShader* shader, const std::string& uniformName, float value);
    void useComputeShader1fv(ComputeShader* shader, const std::string& uniformName, int num, float value[]);
    float clamp(float value, float min, float max);
    void ImGuiImage::ScaleLines(const ImVec2& new_image_size, const ImVec2& old_image_size);
    void HandleResize(ImVec2 prevSize);


};
