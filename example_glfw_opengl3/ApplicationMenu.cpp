

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ApplicationMenu.h"
#include "ToolManager.h"

ApplicationMenu::ApplicationMenu(GLFWwindow* renderWindow, ToolManager* toolManager)
{
    this->renderWindow = renderWindow;
    this->toolManager = toolManager;

}

ApplicationMenu::~ApplicationMenu()
{
}

void ApplicationMenu::update()
{
    
}



GLuint createTextureFromImage(const GLFWimage& image) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload image data to texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}


GLuint LoadTextureFromFile(const char* filename) {
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Debugging: Check OpenGL errors before glTexImage2D
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error before glTexImage2D: " << err << std::endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    
    // Debugging: Check OpenGL errors after glTexImage2D
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error after glTexImage2D: " << err << std::endl;
        stbi_image_free(data);
        return 0;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return textureID;
}



    void RenderUI(GLFWwindow* window, bool load_image, GLuint my_image_texture)
    {
     
    }

    GLuint loadNewImage(GLFWwindow* window)
    {
        GLuint texture = 0;
        std::string filePath = openFileDialog(window, false);

        if (!filePath.empty()) {
            texture = LoadTextureFromFile(filePath.c_str());
            return texture;
        }

        return texture;
    }
