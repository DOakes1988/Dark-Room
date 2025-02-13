#pragma once
#include <glad/glad.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "Common.h"
//#include "Tool/TestTool.h"
//#include "Tool/PaintTool.h"
//#include "Tool/SelectTool.h"
//#include "Tool/MosaicTool.h"
#include "ToolManager.h"
#include "GLFWImage.h"
#include "Filter.h"
//#include "Tool/FilterTool.h"
//#include "Tool/WarpTool.h"
//#include "Tool/Zoom.h"
//#include "Tool/HistogramTool.h"
//#include "Tool/EditTool.h"
#include "ApplicationMenu.h"
#include "ImGuiImage.h"
//#include "AssetManager.h"
//#include "ComputeShader.h"
//#include "../glad/include/glad/glad.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <d3d11.h>
#include <tchar.h>
#include "ColorAdjustmentUI.h"
#include "Slider.h"
#include "LinearGradientFilter.h"
//#include "ComputeShader.h"


class Application
{
public:
    Application();
    ~Application();

    //creates an GLFW window with a name
    bool init(std::string windowName);

    //starts
    void run();

private:
    GLFWwindow* window;

    ToolManager* toolManager;
    ApplicationMenu* applicationMenu;
};
