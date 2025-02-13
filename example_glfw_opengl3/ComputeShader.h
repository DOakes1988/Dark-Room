#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <d3d11.h>
#include "Common.h"


class ComputeShader {
public:
    unsigned int ID;
    ComputeShader(const std::string& shaderPath);
    ~ComputeShader();

    void use();
    void setUniform(const std::string& name, float value);

    static void bindTexture(unsigned int texture, unsigned int binding);

    static int genBuffer();
    static void bindBuffer(unsigned int buf, unsigned int index);
    static void setBuffer(unsigned int buf, size_t size, void* data);

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const ImVec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat3(const std::string& name, const Matrix3x3& mat) const;
    void set1fv(const std::string& name, int num, float value[]) const;


private:
    
    GLuint loadComputeShader(const std::string& shaderPath);
    void checkCompileErrors(unsigned int shader);
    void checkLinkErrors(unsigned int program);
};
