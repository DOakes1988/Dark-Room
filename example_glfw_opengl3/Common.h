#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <string>
#include <set>
#include <iostream>
#include <functional>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImVec2i.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

ImVec2 operator*(const ImVec2& a, const ImVec2& b);
ImVec2 operator*=(const ImVec2& a, const ImVec2& b);
ImVec2 operator*(const ImVec2& a, const ImVec2u& b);
ImVec2 operator/(const ImVec2& a, const ImVec2u& b);
ImVec2 operator/(const ImVec2i& a, const ImVec2u& b);
ImVec2 operator/(const ImVec2u& a, const ImVec2u& b);
ImVec2 operator-(const ImVec2i& a, const ImVec2& b);
ImVec2 operator/(const ImVec2i& a, const float& b);
ImVec2u operator/(const ImVec2u& a, const int& b);
ImVec2i operator*(const ImVec2i a, const float& b);
ImVec2 ImVec2Multiply(const ImVec2& a, const ImVec2& b);
ImVec2 ImVec2Divide(const ImVec2& a, const ImVec2& b);
ImVec2 ImVec2Multiply(const ImVec2& a, float b);
ImVec2 ImVec2Divide(const ImVec2& a, float b);
ImVec2 ImVec2Add(const ImVec2& a, const ImVec2& b);
ImVec2 ImVec2Subtract(const ImVec2& a, const ImVec2& b);


float hue2rgb(float p, float q, float t);

float* rgb2hsl(ImColor color);
void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLFWimage* createGLFWImage(int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void Docking();

//h s l range [0,1]
ImColor hsl2rgb(float h, float s, float l);

float* rgb2hsl(ImColor rgb);

std::string openFileDialog(GLFWwindow* parentWindow, bool selectMultiple = false);

typedef float Matrix3x3[3][3];
