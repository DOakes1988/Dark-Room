#include "ToolManager.h"
#include "ApplicationMenu.h"

ToolManager::ToolManager(GLFWwindow* renderWindow)
{
    this->renderWindow = renderWindow;

    currentTool = nullptr;
}

void ToolManager::addTool(Tool* tool)
{
    //button->setSize({ 1.0 / PANEL_ROWS, 1.0 / PANEL_ROWS * 0.666 });

    //fit the buttons side by side and place them vertically when running out of horizontal room
    //button->setPosition(ImVec2(((int)tools.size() % PANEL_ROWS) * (1.0 / PANEL_ROWS), (int)(tools.size() / PANEL_ROWS) * (1.0 / PANEL_ROWS) * 0.666));

    //create a tool config container for the tool
    tool->setMenu(applicationMenu);
    tool->init();

}

void ToolManager::update()
{
    if (currentTool)
    {
        currentTool->run();
    }
}

void ToolManager::restartTool()
{
    if (currentTool)
    {
        currentTool->stop();
    }
}

void ToolManager::setApplicationMenu(ApplicationMenu* applicationMenu)
{
    this->applicationMenu = applicationMenu;
}

ToolManager::~ToolManager()
{
}

void ToolManager::windowResize()
{
}
