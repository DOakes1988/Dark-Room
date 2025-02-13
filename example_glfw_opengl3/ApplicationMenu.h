#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
//#include "AssetManager.h"
//#include "GUI/PanelElement.h"
#include <glad/glad.h>
#include "Application.h"

//#include "../glad/include/glad/glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <fstream>


GLuint LoadTextureFromFile(const char* filename);

GLuint loadNewImage(GLFWwindow* window);


class ToolManager;

class ApplicationMenu
{
public:

    ApplicationMenu(GLFWwindow* renderWindow, ToolManager* toolManager);
    ~ApplicationMenu();

    void update();


private:
    ToolManager* toolManager;

    GLFWwindow* renderWindow;
    ImColor foreColor;
    ImColor backColor;

    bool foreSelect = true;
};


