#include <glad/glad.h>
#include "stb_image.h"
#include "GLFWImage.h"
/*
// Constructor
GLFWImage::GLFWImage(int w, int h, int c)
    : width(w), height(h), channels(c), brightness(0), contrast(0), pixels(new unsigned char[w * h * c]), texture(0) {
    memset(pixels, 0, width * height * channels);
    glGenTextures(1, &texture);
}

// Copy constructor
GLFWImage::GLFWImage(const GLFWImage& other)
    : width(other.width), height(other.height), channels(other.channels),
    brightness(other.brightness), contrast(other.contrast),
    pixels(new unsigned char[other.width * other.height * other.channels]),
    texture(0) {
    std::memcpy(pixels, other.pixels, width * height * channels);
    glGenTextures(1, &texture);
    uploadToGPU();
}

// Destructor
GLFWImage::~GLFWImage() {
    delete[] pixels;
    glDeleteTextures(1, &texture);
}

// Copy assignment operator
GLFWImage& GLFWImage::operator=(const GLFWImage& other) {
    if (this == &other) {
        return *this; // Self-assignment check
    }
    delete[] pixels;
    glDeleteTextures(1, &texture);

    width = other.width;
    height = other.height;
    channels = other.channels;
    brightness = other.brightness;
    contrast = other.contrast;

    pixels = new unsigned char[width * height * channels];
    std::memcpy(pixels, other.pixels, width * height * channels);
    glGenTextures(1, &texture);
    uploadToGPU();

    return *this;
}


// Load image from file
GLFWImage GLFWImage::loadFromFile(const char* filePath) {
    int w, h, c;
    unsigned char* data = stbi_load(filePath, &w, &h, &c, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return GLFWImage(0, 0, 0);
    }
    GLFWImage img(w, h, c);
    std::memcpy(img.pixels, data, w * h * c);
    stbi_image_free(data);
    img.uploadToGPU();
    return img;
}

// Copy pixels from another image
void GLFWImage::copyPixelsFrom(const GLFWImage& other) {
    if (width != other.width || height != other.height || channels != other.channels) {
        delete[] pixels;
        width = other.width;
        height = other.height;
        channels = other.channels;
        pixels = new unsigned char[width * height * channels];
    }
    std::memcpy(pixels, other.pixels, width * height * channels);
    uploadToGPU();
}

// Adjust brightness
void GLFWImage::adjustBrightness(int b) {
    brightness = b;
    for (int i = 0; i < width * height * channels; ++i) {
        int newValue = pixels[i] + brightness;
        pixels[i] = static_cast<unsigned char>(std::max(0, std::min(255, newValue)));
    }
    uploadToGPU();
}

// Adjust contrast
void GLFWImage::adjustContrast(int c) {
    contrast = c;
    float factor = (259 * (contrast + 255)) / (255 * (259 - contrast));
    for (int i = 0; i < width * height * channels; ++i) {
        int newValue = factor * (pixels[i] - 128) + 128;
        pixels[i] = static_cast<unsigned char>(std::max(0, std::min(255, newValue)));
    }
    uploadToGPU();
}

// Upload texture to GPU
void GLFWImage::uploadToGPU() {
    glBindTexture(GL_TEXTURE_2D, texture);
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}
*/
GLFWImage::GLFWImage() : texture(0), pixels(nullptr), width(0), height(0), channels(0), brightness(0), contrast(0){}

GLFWImage::GLFWImage(GLuint t, unsigned char* p, int w, int h, int c, int b, int cont) : pixels(p), texture(t), width(w), height(h), channels(c), brightness(b), contrast(cont)
{
    if (p) {
        std::memcpy(pixels, p, width * height * 4);
    }
}

GLFWImage::GLFWImage(const GLFWImage& other) : width(other.width), height(other.height), pixels(other.pixels) {
    AllocatePixels();
    //std::memcpy(pixels, other.pixels, width * height * 4);
}

GLFWImage::GLFWImage(int w, int h, const unsigned char* p) : width(w), height(h) {
    AllocatePixels();
    if (p) {
        std::memcpy(pixels, p, width * height * 4);
    }
}

GLFWImage::GLFWImage(GLuint t, int w, int h, int c) : texture(t), width(w), height(h), channels(c) {
    AllocatePixels();
    // No need to initialize pixels, leaving them uninitialized
}

GLFWImage::GLFWImage(GLFWImage&& other) noexcept : channels(other.channels), width(other.width), height(other.height), pixels(other.pixels), brightness(other.brightness), contrast(other.contrast), texture(other.texture) {
    other.pixels = nullptr;
    other.width = 0;
    other.height = 0;
    other.brightness = 0;
    other.contrast = 0;
    other.channels = 0;
    other.texture = 0;
}

GLFWImage& GLFWImage::operator=(const GLFWImage& other) {
    if (this != &other) {
        FreePixels();
        width = other.width;
        height = other.height;
        AllocatePixels();
        std::memcpy(pixels, other.pixels, width * height * 4);
    }
    return *this;
}

GLFWImage& GLFWImage::operator=(GLFWImage&& other) noexcept {
    if (this != &other) {
        FreePixels();
        width = other.width;
        height = other.height;
        pixels = other.pixels;
        other.pixels = nullptr;
        other.width = 0;
        other.height = 0;
    }
    return *this;
}


GLuint GLFWImage::CreateTexture(const char* filePath)
{
    pixels = stbi_load(filePath, &width, &height, &channels, 0);
    if (!pixels) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return 0;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Debugging: Check OpenGL errors before glTexImage2D
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error before glTexImage2D: " << err << std::endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA : GL_RGB, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Debugging: Check OpenGL errors after glTexImage2D
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error after glTexImage2D: " << err << std::endl;
        stbi_image_free(pixels);
        return 0;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

GLFWImage GLFWImage::loadImageFromFile(const std::string& filepath) {
    int w, h, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &w, &h, &channels, STBI_rgb_alpha);
    if (!data) {
        throw std::runtime_error("Failed to load image from file: " + filepath);
    }

    GLFWImage image(w, h, data);
    stbi_image_free(data);
    return image;
}

GLFWImage::~GLFWImage() {
    Cleanup();
    FreePixels();
}

void GLFWImage::EditPixels(void (*editFunc)( unsigned char*, int, int, int)) {
    editFunc(pixels, width, height, channels);
}

void GLFWImage::EditPixels(void (*editFunc)( unsigned char*, int, int, int, int)) {
    editFunc(pixels, width, height, channels, brightness);
}

void GLFWImage::EditPixels(void (*editFunc)( unsigned char*, int, int, int, float)) {
    editFunc(pixels, width, height, channels, contrast);
}

void GLFWImage::UpdateTexture(GLuint tex, const unsigned char* dat) const
{
    glBindTexture(GL_TEXTURE_2D, tex);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, dat);
    glGenerateMipmap(GL_TEXTURE_2D);
}

GLFWImage GLFWImage::CopyNewImage(const GLFWImage& im)
{
    return GLFWImage(im.texture, im.pixels, im.width, im.height, im.channels, im.brightness, im.contrast);
}

int GLFWImage::GetWidth() const {
    return width;
}

int GLFWImage::GetHeight() const {
    return height;
}

int GLFWImage::GetChannels() const {
    return channels;
}

GLuint GLFWImage::GetTextureID() const {
    return texture;
}

unsigned char* GLFWImage::GetPixels() const {
    return pixels;
}

void GLFWImage::SetImage(GLuint tex, unsigned char* pix, int wid, int hei, int chan) {
    FreePixels();
    texture = tex;
    width = wid;
    height = hei;
    channels = chan;

    GLFWImage::AllocatePixels();
    if (pix) {
        std::memcpy(pixels, pix, width * height * 4);
    }
}

void GLFWImage::AllocatePixels() {
    if (width > 0 && height > 0) {
        pixels = new unsigned char[width * height * 4];
    }
    else {
        pixels = nullptr;
    }
}

void GLFWImage::FreePixels() {
    delete[] pixels;
    pixels = nullptr;
}

void GLFWImage::Cleanup()
{
    if (pixels) {
        stbi_image_free(pixels);
    }
    if (texture) {
        glDeleteTextures(1, &texture);
    }
    FreePixels();
}

