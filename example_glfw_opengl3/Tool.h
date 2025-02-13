#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "Common.h"
class ApplicationMenu;
//#include "ApplicationMenu.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class Tool 
{
public:

    Tool();

    virtual void init() = 0;
    virtual void stop() = 0;
    virtual void run() = 0;

   
    void setMenu(ApplicationMenu* applicationMenu);

protected:

    ApplicationMenu* applicationMenu;
};

