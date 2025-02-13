#include "Common.h"
//#include "../DarkRoom/libs/imgui/imgui.h"
//#include "../DarkRoom/libs/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <Windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <commdlg.h>

ImVec2 operator*(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x * b.x, a.y * b.y);
}

ImVec2 operator*=(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a * b);
}

ImVec2 operator*(const ImVec2& a, const ImVec2u& b)
{
    return ImVec2(a.x * b.x, a.y * b.y);
}

ImVec2 operator/(const ImVec2& a, const ImVec2u& b)
{
    return ImVec2(a.x / b.x, a.y / b.y);
}

ImVec2 operator/(const ImVec2i& a, const ImVec2u& b)
{
    return ImVec2((float)a.x / (float)b.x, (float)a.y / (float)b.y);
}

ImVec2 operator/(const ImVec2u& a, const ImVec2u& b)
{
    return ImVec2((float)a.x / b.x, (float)a.y / b.y);
}

ImVec2 operator-(const ImVec2i& a, const ImVec2& b)
{
    return ImVec2(a.x - b.x, a.y - b.y);
}

ImVec2 operator/(const ImVec2i& a, const float& b)
{
    return ImVec2(a.x / b, a.y / b);
}

ImVec2u operator/(const ImVec2u& a, const int& b)
{
    return ImVec2u(a.x / b, a.y / b);
}

ImVec2i operator*(const ImVec2i a, const float& b)
{
    return ImVec2i(a.x * b, a.y * b);
}

ImVec2 ImVec2Multiply(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x * b.x, a.y * b.y);
}

ImVec2 ImVec2Divide(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x / b.x, a.y / b.y);
}

ImVec2 ImVec2Multiply(const ImVec2& a, float b)
{
    return ImVec2(a.x * b, a.y * b);
}

ImVec2 ImVec2Divide(const ImVec2& a, float b)
{
    return ImVec2(a.x / b, a.y / b);
}

ImVec2 ImVec2Add(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x + b.x, a.y + b.y);
}

ImVec2 ImVec2Subtract(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x - b.x, a.y - b.y);
}

float hue2rgb(float p, float q, float t)
{

    if (t < 0)
        t += 1;
    if (t > 1)
        t -= 1;
    if (t < 1. / 6)
        return p + (q - p) * 6 * t;
    if (t < 1. / 2)
        return q;
    if (t < 2. / 3)
        return p + (q - p) * (2. / 3 - t) * 6;

    return p;

}

ImColor hsl2rgb(float h, float s, float l)
{
    ImColor rgb;

    if (0 == s)
    {
        rgb.Value.x = rgb.Value.y = rgb.Value.z = l * 255;
    }
    else
    {
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        rgb.Value.x = hue2rgb(p, q, h + 1. / 3) * 255;
        rgb.Value.y = hue2rgb(p, q, h) * 255;
        rgb.Value.z = hue2rgb(p, q, h - 1. / 3) * 255;
    }

    return rgb;
}

float* rgb2hsl(ImColor rgb)
{
    float r = rgb.Value.x / 255.0;
    float g = rgb.Value.y / 255.0;
    float b = rgb.Value.z / 255.0;

    float cmax = fmax(r, fmax(g, b));
    float cmin = fmin(r, fmin(g, b));
    float delta = cmax - cmin;

    float lightness = (cmax + cmin) / 2.0;

    float hue, saturation;
    if (delta == 0)
    {
        hue = 0;
        saturation = 0;
    }
    else
    {
        if (lightness < 0.5)
        {
            saturation = delta / (cmax + cmin);
        }
        else
        {
            saturation = delta / (2.0 - cmax - cmin);
        }
        if (cmax == r)
        {
            hue = fmod((g - b) / delta, 6.0);
        }
        else if (cmax == g)
        {
            hue = ((b - r) / delta) + 2.0;
        }
        else
        {
            hue = ((r - g) / delta) + 4.0;
        }
        hue /= 6.0;
    }

    float* hsl = new float[3];
    hsl[0] = hue;
    hsl[1] = saturation;
    hsl[2] = lightness;

    return hsl;
}


std::string openFileDialog(GLFWwindow* parentWindow, bool selectMultiple)
{
    OPENFILENAME ofn;
    char szFile[260] = { 0 };
    HWND hwnd = NULL;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("All\0*.*\0JPEG\0*.JPG\0PNG\0*.PNG\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | (selectMultiple ? OFN_ALLOWMULTISELECT : 0);

    if (GetOpenFileName(&ofn) == TRUE)
    {
        return std::string(ofn.lpstrFile);
    }

    return std::string();
}

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Callback functions with correct signatures
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (ImGui::GetIO().WantCaptureMouse) // Check if ImGui wants to capture mouse input
        return;

    std::cout << "Window " << window << " resized: width = " << width << ", height = " << height << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (ImGui::GetIO().WantCaptureKeyboard) // Check if ImGui wants to capture keyboard input
        return;

    if (action == GLFW_PRESS)
        std::cout << "Key pressed: " << key << std::endl;
    else if (action == GLFW_RELEASE)
        std::cout << "Key released: " << key << std::endl;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            std::cout << "Left mouse button pressed\n";
            // Handle left mouse button press
        }
        else if (action == GLFW_RELEASE) {
            std::cout << "Left mouse button released\n";
            // Handle left mouse button release
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            std::cout << "Right mouse button pressed\n";
            // Handle right mouse button press
        }
        else if (action == GLFW_RELEASE) {
            std::cout << "Right mouse button released\n";
            // Handle right mouse button release
        }
    }
    // Add more conditions for other mouse buttons if needed
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    //std::cout << "Cursor position: (" << xpos << ", " << ypos << ")\n";
    // Handle cursor position changes
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << "Scroll: x = " << xoffset << ", y = " << yoffset << std::endl;
}

GLFWimage* createGLFWImage(int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = new unsigned char[width * height * 4]; // 4 channels (RGBA)

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 4;
            image.pixels[index + 0] = r; // Red
            image.pixels[index + 1] = g; // Green
            image.pixels[index + 2] = b; // Blue
            image.pixels[index + 3] = a; // Alpha
        }
    }

    return &image;
}

void Docking()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
            if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
            if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

