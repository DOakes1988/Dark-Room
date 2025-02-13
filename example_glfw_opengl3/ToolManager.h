#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "Common.h"
#include "Tool.h"
//#include "PanelElement.h"
class ApplicationMenu;
//#include "ApplicationMenu.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <map>

#define PANEL_ROWS 4

class ToolManager 
{
public:

    ToolManager(GLFWwindow* renderWindow);

    void addTool(Tool* tool);

   

    void update();

    void restartTool();

    void setApplicationMenu(ApplicationMenu* applicationMenu);

    ~ToolManager();

private:

    void windowResize();

    Tool* currentTool;

    GLFWwindow* renderWindow;

    ApplicationMenu* applicationMenu;
};

