#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class GLFWImage {
public:

    /*
    GLuint texture;
    unsigned char* pixels;
    int width;
    int height;
    int channels;
    int brightness;
    int contrast;

    GLFWImage(int w, int h, int c);
    GLFWImage(const GLFWImage& other);
    ~GLFWImage();

    GLFWImage& operator=(const GLFWImage& other);
    static GLFWImage loadFromFile(const char* filePath);
    void copyPixelsFrom(const GLFWImage& other);
    void adjustBrightness(int b);
    void adjustContrast(int c);
    void uploadToGPU();
     
    */
    
    GLFWImage();
    GLFWImage(GLuint texture, int width, int height, int channels);
    GLFWImage(int width, int height, const unsigned char* pixels);
    GLFWImage(GLuint texture, unsigned char* pixels, int width, int height, int channels, int brightness, int contrast);
    GLFWImage(const GLFWImage& other);
    GLFWImage(GLFWImage&& other) noexcept;
    static GLFWImage loadImageFromFile(const std::string& filepath);

    GLFWImage& operator = (const GLFWImage& other);
    GLFWImage& operator = (GLFWImage&& other) noexcept;

    ~GLFWImage();
    GLuint GetTextureID() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetChannels() const;
    unsigned char* GetPixels() const;

    GLuint CreateTexture(const char* filePath);
    void SetImage(GLuint tex, unsigned char* pix, int wid, int hei, int chan);
    
    static GLFWImage CopyNewImage(const GLFWImage& image);
    
    void EditPixels(void (*editFunc)( unsigned char*, int, int, int));
    void EditPixels(void (*editFunc)( unsigned char*, int, int, int, int));
    void EditPixels(void (*editFunc)( unsigned char*, int, int, int, int, int));
    void EditPixels(void (*editFunc)(unsigned char*, int, int, int, int, float));
    void EditPixels(void (*editFunc)( unsigned char*, int, int, int, float));
    void UpdateTexture(GLuint, const unsigned char* ) const;

    
   

private:
    GLuint texture;
    unsigned char* pixels;
    int width, height, channels, brightness, prevNum;
    float contrast = 1.5f;
    
    void AllocatePixels();
    void FreePixels();

    void Cleanup();
    

    };

