#include <glad/glad.h>
#include "stb_image.h"
#include "ImGuiImage.h"


// Constructor
ImGuiImage::ImGuiImage(int w, int h, int c)
    : width(w), height(h), channels(c), brightness(0), contrast(0), drawing(false), apply_above(false), filter_mode(false), line_drawn(false),
    pixels(new unsigned char[w * h * c]), originalPixels(new unsigned char[w * h * c]), texture(0), isMousePressed(false) {
    if (!pixels || !originalPixels) {
        std::cerr << "Failed to allocate memory for image pixels" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(pixels, 0, width * height * channels);
    memset(originalPixels, 0, width * height * channels);

    glGenTextures(1, &texture);
    glGenTextures(1, &copyTexture);
    
    adjustmentShader = new ComputeShader("D:\\Code\\PhotoEditor\\DarkRoom\\example_glfw_opengl3\\AdjustPixels.comp");
    histogramShader = new ComputeShader("D:\\Code\\PhotoEditor\\DarkRoom\\example_glfw_opengl3\\Histogram.comp");
    linearGradientFilterShader = new ComputeShader("D:\\Code\\PhotoEditor\\DarkRoom\\example_glfw_opengl3\\LinearGradientFilter.comp");
}

// Copy constructor
ImGuiImage::ImGuiImage(const ImGuiImage& other)
    : width(other.width), height(other.height), channels(other.channels),
    brightness(other.brightness), contrast(other.contrast), white(other.white), black(other.black), shadows(other.shadows),
    pixels(new unsigned char[other.width * other.height * other.channels]),
    originalPixels(new unsigned char[other.width * other.height * other.channels]),
    texture(0)
{
    std::memcpy(pixels, other.pixels, width * height * channels);
    std::memcpy(originalPixels, other.originalPixels, width * height * channels);
    glGenTextures(1, &texture);
    glGenTextures(1, &copyTexture);
}

// Destructor
ImGuiImage::~ImGuiImage()
{
    delete[] pixels;
    delete[] originalPixels;
    
    if (texture)
    {
        glDeleteTextures(1, &texture);
    }

    if (copyTexture)
    {
        glDeleteTextures(1, &copyTexture);
    }
}

// Copy assignment operator
ImGuiImage& ImGuiImage::operator=(const ImGuiImage& other)
{
    if (this == &other) {
        return *this; // Self-assignment check
    }
    delete[] pixels;
    delete[] originalPixels;
    glDeleteTextures(1, &texture);

    width = other.width;
    height = other.height;
    channels = other.channels;
    brightness = other.brightness;
    contrast = other.contrast;

    pixels = new unsigned char[width * height * channels];
    originalPixels = new unsigned char[width * height * channels];
    std::memcpy(pixels, other.pixels, width * height * channels);
    std::memcpy(originalPixels, other.originalPixels, width * height * channels);
    glGenTextures(1, &texture);
    //UploadToGPU();

    return *this;
}

// Load image from file
ImGuiImage ImGuiImage::LoadFromFile(const std::string& filePath)
{
    int w, h, c;
    
    unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &c, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return ImGuiImage(0, 0, 0);
    }

    c = 4;
    ImGuiImage img(w, h, c);

    std::memcpy(img.pixels, data, w * h * c);
    std::memcpy(img.originalPixels, data, w * h * c);
    stbi_image_free(data);
    img.UploadToGPU();
    return img;
}

GLuint ImGuiImage::LoadTexture(const char* filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

void ImGuiImage::LoadTextures()
{
    open_hand_texture = LoadTexture("D:\\Code\\PhotoEditor\\DarkRoom\\example_glfw_opengl3\\Textures\\openHand.png");
}

// Copy pixels from another image
void ImGuiImage::CopyPixelsFrom(const ImGuiImage& other)
{
    if (width != other.width || height != other.height || channels != other.channels) {
        delete[] pixels;
        delete[] originalPixels;
        width = other.width;
        height = other.height;
        channels = other.channels;
        pixels = new unsigned char[width * height * channels];
        originalPixels = new unsigned char[width * height * channels];
    }

    std::memcpy(pixels, other.pixels, width * height * channels);
    std::memcpy(originalPixels, other.originalPixels, width * height * channels);
    //UploadToGPU();
}

void ImGuiImage::useComputeShader(ComputeShader* shader, const std::string& uniformName, float value)
{
    if (shader && shader->ID != 0) {
        shader->use();
        shader->setUniform(uniformName, value);

        glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);  // TODO:: GL_READ_WRITE
        glBindImageTexture(1, copyTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8); // TODO:: GL_READ_WRITE

        int workGroupSizeX = (width + 15) / 16;
        int workGroupSizeY = (height + 15) / 16;

        glDispatchCompute(workGroupSizeX, workGroupSizeY, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
    else {
        std::cerr << "ERROR::SHADER::USE_FAILED: Invalid shader or shader program." << std::endl;
    }
}

void ImGuiImage::useComputeShader1fv(ComputeShader* shader, const std::string& uniformName, int num, float value[])
{
    if (shader && shader->ID != 0) {
        shader->use();
        shader->set1fv(uniformName, num, value);
      
        glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);  // TODO:: GL_READ_WRITE
        glBindImageTexture(1, copyTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8); // TODO:: GL_READ_WRITE  

        int workGroupSizeX = (width + 15) / 16;
        int workGroupSizeY = (height + 15) / 16;

        glDispatchCompute(workGroupSizeX, workGroupSizeY, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
    else {
        std::cerr << "ERROR::SHADER::USE_FAILED: Invalid shader or shader program." << std::endl;
    }
}

// Adjust Black Point
void ImGuiImage::AdjustBlack(float bl)
{
    black = bl;
    std::cout << "BLACK" << std::endl;
    float blackFactor = black / 5.0f;
    useComputeShader(linearGradientFilterShader, "blackFactor", blackFactor);
}

// Adjust Brightness
void ImGuiImage::AdjustBrightness(float b)
{
    brightness = b;

    float brightnessFactor = brightness / 5.0f;
    useComputeShader(linearGradientFilterShader, "brightnessFactor", brightnessFactor);
}

// Adjust Contrast
void ImGuiImage::AdjustContrast(float c)
{
    contrast = c;
    if (c == -5.0f) return;

    float contrastFactor = 1.0f + contrast / 5.0f;
    useComputeShader(linearGradientFilterShader, "contrastFactor", contrastFactor);
}

// Adjust Saturation
void ImGuiImage::AdjustSaturation(float s)
{
    saturate = s;
    if (s == -5.0f) return;

    float saturateFactor = 1.0f + saturate / 5.0f;
    useComputeShader(linearGradientFilterShader, "saturateFactor", saturateFactor);
}

// Adjust Shadows
void ImGuiImage::AdjustShadows(float s)
{
    shadows = s;

    float shadowFactor = shadows / 5.0f;
    useComputeShader(linearGradientFilterShader, "shadowFactor", shadowFactor);
}

// Adjust White Point
void ImGuiImage::AdjustWhite(float w)
{
    white = w;

    float whiteFactor = white / 5.0f;
    useComputeShader(linearGradientFilterShader, "whiteFactor", whiteFactor);
}




// Adjust Black Point
void ImGuiImage::AdjustFilterBlack(float bl)
{
    filterBlack = bl;

    float blackFactor = filterBlack / 5.0f;
    useComputeShader(linearGradientFilterShader, "FblackFactor", blackFactor);
}

// Adjust Brightness
void ImGuiImage::AdjustFilterBrightness(float b)
{
    filterBrightness = b;

    float brightnessFactor = filterBrightness / 5.0f;
    useComputeShader(linearGradientFilterShader, "FbrightnessFactor", brightnessFactor);
}

// Adjust Contrast
void ImGuiImage::AdjustFilterContrast(float c)
{
    filterContrast = c;
    if (c == -5.0f) return;

    float contrastFactor = 1.0f + filterContrast / 5.0f;
    useComputeShader(linearGradientFilterShader, "FcontrastFactor", contrastFactor);
}

// Adjust Saturation
void ImGuiImage::AdjustFilterSaturation(float s)
{
    filterSaturate = s;
    if (s == -5.0f) return;

    float saturateFactor = 1.0f + filterSaturate / 5.0f;
    useComputeShader(linearGradientFilterShader, "FsaturateFactor", saturateFactor);
}

// Adjust Shadows
void ImGuiImage::AdjustFilterShadows(float s)
{
    filterShadows = s;

    float shadowFactor = filterShadows / 5.0f;
    useComputeShader(linearGradientFilterShader, "FshadowFactor", shadowFactor);
}

// Adjust White Point
void ImGuiImage::AdjustFilterWhite(float w)
{
    filterWhite = w;

    float whiteFactor = filterWhite / 5.0f;
    useComputeShader(linearGradientFilterShader, "FwhiteFactor", whiteFactor);
}

//void ImGuiImage::AdjustHSL(float r, float o, float y, float g, float a, float b, float p, float m)
void ImGuiImage::AdjustHSL(float hue_shifts[], float sat_shifts[], float lum_shifts[], float hue_left[], float hue_right[])
{
    useComputeShader1fv(linearGradientFilterShader, "hue_shifts", 8, hue_shifts);
    useComputeShader1fv(linearGradientFilterShader, "sat_shifts", 8, sat_shifts);
    useComputeShader1fv(linearGradientFilterShader, "lum_shifts", 8, lum_shifts);
    useComputeShader1fv(linearGradientFilterShader, "hue_left", 8, hue_left);
    useComputeShader1fv(linearGradientFilterShader, "hue_right", 8, hue_right);
}

// Upload texture to GPU
void ImGuiImage::UploadToGPU()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    
    glBindTexture(GL_TEXTURE_2D, copyTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}


// Render the image using ImGui
void ImGuiImage::RenderImGui(const ImVec2& size)
{

    ImGui::Image((void*)(intptr_t)texture, size);
}

// Render the image using ImGui with UV coordinates
void ImGuiImage::RenderImGui(const ImVec2& size, ImVec2& uv0, ImVec2& uv1)
{
    ImGui::Image((void*)(intptr_t)texture, size, uv0, uv1);
}

float ImGuiImage::clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void ImGuiImage::UpdateLinePoints() {
    float dx = line_end.x - line_start.x;
    float dy = line_end.y - line_start.y;
    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length;
    dy /= length;

    // Keep the line within the image bounds
    line_start.x = clamp(line_start.x, image_pos.x, image_pos.x + imageSize.x);
    line_start.y = clamp(line_start.y, image_pos.y, image_pos.y + imageSize.y);
    line_end.x = line_start.x + dx * length;
    line_end.y = line_start.y + dy * length;
    line_end.x = clamp(line_end.x, image_pos.x, image_pos.x + imageSize.x);
    line_end.y = clamp(line_end.y, image_pos.y, image_pos.y + imageSize.y);
}



void ImGuiImage::RenderCenteredImage(float original_width, float original_height, float window_width, float window_height, float padding, ImVec2 uv0, ImVec2 uv1, float zoom_level)
{
    // Calculate the aspect ratio of the image
    float aspect_ratio = original_height / original_width;

    // Calculate the new width and height to fit the window width while maintaining the aspect ratio
    float available_width = window_width - 2 * padding;
    float new_width = available_width;
    float new_height = new_width * aspect_ratio;

    
    
    // Ensure the new height does not exceed the window height
    if (new_height > window_height) {
        new_height = window_height;
        new_width = new_height / aspect_ratio;
    }

    // Calculate the padding to center the image vertically
    float padding_x = (window_width - new_width) / 2.0f;
    float padding_y = (window_height - new_height) / 2.0f;

    // Create a dummy item to act as padding on the left
    ImGui::SetCursorPosX(padding_x);
    ImGui::SetCursorPosY(padding_y);

    // Render the image with UV coordinates for zoom
    ImGui::Image((void*)(intptr_t)texture, ImVec2(new_width, new_height), uv0, uv1);

    texWidth = ImGui::GetItemRectSize().x;
    texHeight = ImGui::GetItemRectSize().y;

    //std::cout << new_width << " " << new_height << std::endl;
    //std::cout << texWidth << " " << texHeight << std::endl;

    //std::cout << " " << std::endl;

    
    image_pos = ImGui::GetCursorScreenPos();
    imageSize = ImVec2(ImGui::GetItemRectSize());

    //imageSize = ImVec2(texWidth, texHeight);
    //prev_image_size = image_size;

    if (prev_image_size.x != imageSize.x || prev_image_size.y != imageSize.y)
    {
        ScaleLines(imageSize, prev_image_size);
        prev_image_size = imageSize;
    }

    windowSize = ImVec2(window_width, window_height);
    //imageSize = ImVec2(new_width, new_height);
    isHovered = ImGui::IsItemHovered();
}

void ImGuiImage::HandleZoom(ImVec2 click_pos, ImVec2 image_pos, ImVec2 image_size, ImVec2& uv0, ImVec2& uv1, float& zoom_level, int& zoom_factor, bool& is_zoomed)
{
    
    if (is_zoomed)
    {
        // Reset to original state
        zoom_level = 1.0f;
        uv0 = ImVec2(0.0f, 0.0f);
        uv1 = ImVec2(1.0f, 1.0f);
        is_zoomed = false;
        zoomFlag = false;
    }

    else if (!is_zoomed && !line_drawn)
    {
        ImVec2 image_uv_size = ImVec2Subtract(uv1, uv0);

        // Calculate the click position in UV coordinates
        ImVec2 click_uv = ImVec2Add(uv0, ImVec2Multiply(image_uv_size, ImVec2Divide(ImVec2Subtract(click_pos, image_pos), image_size)));

        // Update the zoom level
        zoom_level *= zoom_factor;

        // Calculate the new UV coordinates
        ImVec2 zoom_uv_size = ImVec2Divide(image_uv_size, zoom_factor);
        uv0 = ImVec2Subtract(click_uv, ImVec2Multiply(zoom_uv_size, 0.5f));
        uv1 = ImVec2Add(click_uv, ImVec2Multiply(zoom_uv_size, 0.5f));

        // Ensure UV coordinates are clamped within [0, 1]
        if (uv0.x < 0.0f) { uv0.x = 0.0f; uv1.x = zoom_uv_size.x; }
        if (uv0.y < 0.0f) { uv0.y = 0.0f; uv1.y = zoom_uv_size.y; }
        if (uv1.x > 1.0f) { uv1.x = 1.0f; uv0.x = 1.0f - zoom_uv_size.x; }
        if (uv1.y > 1.0f) { uv1.y = 1.0f; uv0.y = 1.0f - zoom_uv_size.y; }

        is_zoomed = true;
        zoomFlag = true;
    }
}

void ImGuiImage::RenderHistogram()
{
    std::cout << "HISTOGRAM" << std::endl;
    glGenBuffers(1, &histBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, histBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * 256 * sizeof(uint32_t), NULL, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, histBuffer);

    histogramShader->use();

    // Bind the image texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(histogramShader->ID, "image"), 0);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, histBuffer);

    std::vector<uint32_t> zeroData(4 * 256, 0);

    // Initialize histogram buffer to zero
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, static_cast<unsigned long long>(4) * 256 * sizeof(uint32_t), zeroData.data());

    int workGroupSizeX = (width + 15) / 16;
    int workGroupSizeY = (height + 15) / 16;

    glDispatchCompute((GLuint)ceil(width / 16.0), (GLuint)ceil(height / 16.0), 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Read back histogram data
    std::vector<uint32_t> l(4 * 256);
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, histBuffer);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, static_cast<unsigned long long>(4) * 256 * sizeof(uint32_t), l.data());

    std::vector<uint32_t> histLuminance(l.begin(), l.begin() + 256);
    std::vector<uint32_t> histRed(l.begin() + 256, l.begin() + 512);
    std::vector<uint32_t> histGreen(l.begin() + 512, l.begin() + 768);
    std::vector<uint32_t> histBlue(l.begin() + 768, l.begin() + 1024);

    static bool showLuminance = true;
    static bool showRed = true;
    static bool showGreen = true;
    static bool showBlue = true;

    ImGui::Begin("Histograms");
    ImGui::Checkbox("Luminance", &showLuminance);
    ImGui::Checkbox("Red", &showRed);
    ImGui::Checkbox("Green", &showGreen);
    ImGui::Checkbox("Blue", &showBlue);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();        

    if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
    if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;

    draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(0, 0, 0, 255));
    draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(255, 255, 255, 255));

    float max_value = 0;
    if (showLuminance) max_value = max(max_value, static_cast<float>(*std::max_element(histLuminance.begin(), histLuminance.end())));
    if (showRed) max_value = max(max_value, static_cast<float>(*std::max_element(histRed.begin(), histRed.end())));
    if (showGreen) max_value = max(max_value, static_cast<float>(*std::max_element(histGreen.begin(), histGreen.end())));
    if (showBlue) max_value = max(max_value, static_cast<float>(*std::max_element(histBlue.begin(), histBlue.end())));

    const float bin_width = canvas_size.x / 256.0f;
    auto plot_histogram_filled = [&](const std::vector<uint32_t>& hist, ImU32 color) {
        for (int i = 0; i < 256; ++i) {
            float x = canvas_pos.x + i * bin_width;
            float y = canvas_pos.y + canvas_size.y - (hist[i] / max_value) * canvas_size.y;
            draw_list->AddRectFilled(ImVec2(x, canvas_pos.y + canvas_size.y), ImVec2(x + bin_width, y), color);
        }
        };

    auto plot_histogram_line = [&](const std::vector<uint32_t>& hist, ImU32 color) {
        for (int i = 0; i < 255; ++i) {
            float x0 = canvas_pos.x + i * bin_width;
            float x1 = canvas_pos.x + (i + 1) * bin_width;
            float y0 = canvas_pos.y + canvas_size.y - (hist[i] / max_value) * canvas_size.y;
            float y1 = canvas_pos.y + canvas_size.y - (hist[i + 1] / max_value) * canvas_size.y;

            draw_list->AddLine(ImVec2(x0, y0), ImVec2(x1, y1), color, 2.0f);
        }
    };

    if (showLuminance) plot_histogram_filled(histLuminance, IM_COL32(75, 75, 75, 255));
    if (showRed) plot_histogram_filled(histRed, IM_COL32(255, 0, 0, 64));
    if (showGreen) plot_histogram_filled(histGreen, IM_COL32(0, 255, 0, 64));
    if (showBlue) plot_histogram_filled(histBlue, IM_COL32(0, 0, 255, 64));

    if (showLuminance) plot_histogram_line(histLuminance, IM_COL32(255, 255, 255, 255));
    if (showRed) plot_histogram_line(histRed, IM_COL32(255, 0, 0, 255));
    if (showGreen) plot_histogram_line(histGreen, IM_COL32(0, 255, 0, 255));
    if (showBlue) plot_histogram_line(histBlue, IM_COL32(0, 0, 255, 255));


    ImGui::End();
}

GLuint ImGuiImage::GetTexture() const
{
    return texture;
}


void ImGuiImage::LinearGradientFilter(ImGuiImage& img, ImVec2 size)
{   
    GLuint tex = open_hand_texture;
    LoadTextures();

    
    if (ImGui::Button("Linear Gradient Filter")) {
        filter_mode = !filter_mode; // Toggle filter mode
        if (!filter_mode) {
            line_start = ImVec2(0, 0);
            line_end = ImVec2(0, 0);
            line_drawn = false; // Reset the line when filter mode is turned off
            hover_rotate_left = hover_rotate_right = hover_middle = false;
            isMousePressed = false;
        }
    }

    

    if (filter_mode) {
        ImGui::Checkbox("Apply Above", &apply_above);

        if (ImGui::Button("Clear Line")) {
            line_start = initial_line_start = ImVec2(0, 0);
            line_end = initial_line_end = ImVec2(0, 0);
            hover_rotate_left = hover_rotate_right = hover_middle = false;
            isMousePressed = false;
            line_drawn = false;
        }

        ImGui::SliderFloat("Gradient Intensity", &gradient_intensity, 1.0f, 10.0f);  // Slider to adjust gradient intensity

        hover_rotate_left = hover_rotate_right = hover_middle = false;
        ImVec2 hover_position = ImVec2(0, 0);

        // Function to calculate the perpendicular distance from a point to a line
        auto point_line_distance = [](ImVec2 point, ImVec2 line_start, ImVec2 line_end) -> float {
            float a = line_end.y - line_start.y;
            float b = line_start.x - line_end.x;
            float c = line_end.x * line_start.y - line_start.x * line_end.y;
            return std::abs(a * point.x + b * point.y + c) / std::sqrt(a * a + b * b);
            };

        // Check hover state for interactive points
        if (!isMousePressed && line_drawn)
        {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            float threshold = 25.0f; // Distance threshold for hover detection

            // Hover check for mid line
            if (ImLengthSqr(ImVec2Subtract(mouse_pos, ImVec2((line_start.x + line_end.x) / 2, (line_start.y + line_end.y) / 2))) < 225.0f) {
                hover_middle = true;
                hover_line = true;
                hover_rotate_left = hover_rotate_right = false;
                hover_position = ImVec2((line_start.x + line_end.x) / 2, line_start.y);
            }
            else if (point_line_distance(mouse_pos, line_start, line_end) <= threshold)
            {
                if (mouse_pos.x < (line_start.x + line_end.x) / 2 - 25.f && mouse_pos.x > line_start.x + 12.f)
                {
                    hover_line = true;
                    hover_rotate_left = true;
                    hover_rotate_right = hover_middle = false;
                }
                if (mouse_pos.x > (line_start.x + line_end.x) / 2 + 25.f && mouse_pos.x < line_end.x + 12.f)
                {
                    hover_line = true;
                    hover_rotate_right = true;
                    hover_rotate_left = hover_middle = false;
                }

                hover_position = mouse_pos;
            }
            else {
                hover_line = false;
            }
        }

        // Capture mouse clicks to draw the line
        if (ImGui::IsMouseClicked(0) && !line_drawn && isHovered && !hover_rotate_left && !hover_rotate_right && !hover_middle)
        {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            //mouse_pos.y = clamp(mouse_pos.y, image_pos.y, image_pos.y + texHeight);

            line_start = ImVec2(image_pos.x, mouse_pos.y);
            line_end = ImVec2(image_pos.x + imageSize.x, mouse_pos.y);

            line_drawn = true;
            line_transparent = false;

            // Store initial positions for scaling
            initial_line_start = line_start;
            initial_line_end = line_end;
        }

        // Draw the line and the interactive hands
        if (line_start.x != 0 || line_end.x != 0)
        {
            ImU32 line_color = line_transparent ? IM_COL32(255, 255, 255, 128) : IM_COL32(255, 255, 255, 255);
            ImGui::GetForegroundDrawList()->AddLine(ImVec2(line_start.x + 12.0f, line_start.y), ImVec2(line_end.x + 12.0f, line_end.y), line_color, 2.0f);
            
            ImVec2 texture_half_size = ImVec2(textureSize.x / 2, textureSize.y / 2);
            bool is_mouse_down = ImGui::IsMouseDown(0);
            GLuint texture = open_hand_texture;

            // Mid handle - always visible and centered on the middle line
            ImGui::GetForegroundDrawList()->AddCircle(ImVec2((line_start.x + line_end.x) / 2, (line_start.y + line_end.y) / 2), 5.0f, line_color, 12, 2.0f);
            
            if (hover_line)
            {
                if (ImGui::IsMouseClicked(0))
                {
                    isMousePressed = true;
                    if (hover_rotate_left || hover_rotate_right || hover_middle)
                    {
                        isDragging = true;
                        initial_mouse_pos = ImGui::GetMousePos();
                        if (hover_rotate_left)
                        {
                            isRotatingLeft = true;
                            isRotatingRight = false;
                            isMiddleDrag = false;
                        }
                        else if (hover_rotate_right)
                        {
                            isRotatingLeft = false;
                            isRotatingRight = true;
                            isMiddleDrag = false;
                        }
                        else if (hover_middle)
                        {
                            isRotatingLeft = false;
                            isRotatingRight = false;
                            isMiddleDrag = true;
                        }
                    }
                }

                // Rotation logic
                if (isDragging && ImGui::IsMouseDragging(0))
                {
                    ImVec2 current_mouse_pos = ImGui::GetMousePos();
                    if (isRotatingLeft)
                    {
                        //line_start.y = clamp(line_start.y + current_mouse_pos.y - initial_mouse_pos.y, image_pos.y, image_pos.y + imageSize.y);
                        line_start.y = line_start.y + current_mouse_pos.y - initial_mouse_pos.y;
                    }
                    else if (isRotatingRight)
                    {
                        //line_end.y = clamp(line_end.y + current_mouse_pos.y - initial_mouse_pos.y, image_pos.y, image_pos.y + imageSize.y);
                        line_end.y = line_end.y + current_mouse_pos.y - initial_mouse_pos.y;
                    }
                    else if (isMiddleDrag)
                    {
                        //line_start.y = clamp(line_start.y + current_mouse_pos.y - initial_mouse_pos.y, image_pos.y, image_pos.y + imageSize.y);
                        //line_end.y = clamp(line_end.y + current_mouse_pos.y - initial_mouse_pos.y, image_pos.y, image_pos.y + imageSize.y);

                        line_start.y = line_start.y + current_mouse_pos.y - initial_mouse_pos.y;
                        line_end.y = line_end.y + current_mouse_pos.y - initial_mouse_pos.y;
                    }
                    initial_mouse_pos = current_mouse_pos; // Update initial position for smooth rotation
                }

                if (ImGui::IsMouseReleased(0))
                {
                    isDragging = false;
                    isMousePressed = false;
                    isRotatingLeft = false;
                    isRotatingRight = false;
                    isMiddleDrag = false;
                }

                // Draw the hand texture at the hover position if any line is being hovered
                if (hover_rotate_left || hover_rotate_right)
                {
                    ImGui::GetForegroundDrawList()->AddImage((void*)(intptr_t)texture, ImVec2(hover_position.x - texture_half_size.x, hover_position.y - texture_half_size.y), ImVec2(hover_position.x + texture_half_size.x, hover_position.y + texture_half_size.y));
                }
            }
        }
    }    

    std::cout << line_start.x << " " << line_start.y << std::endl;
    linearGradientFilterShader->use();
    glUniform2f(glGetUniformLocation(linearGradientFilterShader->ID, "line_start"), line_start.x, line_start.y);
    glUniform2f(glGetUniformLocation(linearGradientFilterShader->ID, "line_end"), line_end.x, line_end.y);
    glUniform2f(glGetUniformLocation(linearGradientFilterShader->ID, "image_pos"), image_pos.x, image_pos.y);
    glUniform2f(glGetUniformLocation(linearGradientFilterShader->ID, "image_size"), imageSize.x, imageSize.y);
    glUniform1i(glGetUniformLocation(linearGradientFilterShader->ID, "apply_above"), apply_above);
    glUniform1i(glGetUniformLocation(linearGradientFilterShader->ID, "linear_gradient_active"), filter_mode);
    glUniform1f(glGetUniformLocation(linearGradientFilterShader->ID, "gradient_intensity"), gradient_intensity);

    // Calculate angle and center
    center = ImVec2((line_start.x + line_end.x) / 2, (line_start.y + line_end.y) / 2);
    angle = atan2(line_end.y - line_start.y, line_end.x - line_start.x);
    glUniform1f(glGetUniformLocation(linearGradientFilterShader->ID, "angle"), angle);
    glUniform2f(glGetUniformLocation(linearGradientFilterShader->ID, "center"), center.x, center.y);

    int workGroupSizeX = (width) / 16;
    int workGroupSizeY = (height) / 16;
    
    glDispatchCompute(workGroupSizeX, workGroupSizeY, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ImGuiImage::ScaleLines(const ImVec2& new_size, const ImVec2& old_size)
{
    float width_ratio = new_size.x / old_size.x;
    float height_ratio = new_size.y / old_size.y;

    // Calculate the center of the line
    ImVec2 line_center = ImVec2((line_start.x + line_end.x) / 2, (line_start.y + line_end.y) / 2);

    // Calculate the original distances from the center
    ImVec2 start_to_center = ImVec2(line_start.x - line_center.x, line_start.y - line_center.y);
    ImVec2 end_to_center = ImVec2(line_end.x - line_center.x, line_end.y - line_center.y);

    // Scale the distances by the width and height ratios
    ImVec2 scaled_start_to_center = ImVec2(start_to_center.x * width_ratio, start_to_center.y * height_ratio);
    ImVec2 scaled_end_to_center = ImVec2(end_to_center.x * width_ratio, end_to_center.y * height_ratio);

    // Compute the new center of the line based on the scaling
    line_center = ImVec2((line_center.x - image_pos.x) * width_ratio + image_pos.x, (line_center.y - image_pos.y) * height_ratio + image_pos.y);

    // Compute the new line start and end points
    //line_start = ImVec2(line_center.x + scaled_start_to_center.x, line_center.y + scaled_start_to_center.y);
    line_end = ImVec2(line_center.x + scaled_end_to_center.x, line_center.y + scaled_end_to_center.y);

    line_start = ImVec2(image_pos.x * width_ratio, line_start.y * width_ratio);
    line_end = ImVec2(image_pos.x + new_size.x, line_end.y * width_ratio);
}

bool ImGuiImage::IsValid() const {
    return (pixels != nullptr) && (width > 0) && (height > 0);
}