#include "Filter.h"

/*
void Filter::InvertColors(unsigned char* data, int width, int height, int channels) {
    int numPixels = width * height * channels;
    for (int i = 0; i < numPixels; i += channels) {
        data[i] = 255 - data[i];       // Red
        data[i + 1] = 255 - data[i + 1]; // Green
        data[i + 2] = 255 - data[i + 2]; // Blue
        // Alpha channel remains unchanged
    }
}

void Filter::Grayscale(unsigned char* data, int width, int height, int channels) {
    int numPixels = width * height * channels;
    for (int i = 0; i < numPixels; i += channels) {
        unsigned char gray = 0.299f * data[i] + 0.587f * data[i + 1] + 0.114f * data[i + 2];
        data[i] = data[i + 1] = data[i + 2] = gray;
        // Alpha channel remains unchanged
    }
}

int Filter::Brightness(unsigned char* data, int width, int height, int channels, int brightness, int prevNum) {

    int numPixels = width * height * channels;

    brightness = brightness / 10;

    if (brightness > 0)
    {
        if (brightness > prevNum)
            for (int i = 0; i < numPixels; i += channels) {
                data[i] = std::min(255, std::max(0, data[i] + brightness));       // Red
                data[i + 1] = std::min(255, std::max(0, data[i + 1] + brightness)); // Green
                data[i + 2] = std::min(255, std::max(0, data[i + 2] + brightness)); // Blue
                // Alpha channel remains unchanged
            }

        if (brightness < prevNum)
        {
            for (int i = 0; i < numPixels; i += channels) {
                data[i] = std::min(255, std::max(0, data[i] - brightness));       // Red
                data[i + 1] = std::min(255, std::max(0, data[i + 1] - brightness)); // Green
                data[i + 2] = std::min(255, std::max(0, data[i + 2] - brightness)); // Blue
            }
        }
    }

    if (brightness < 0)
    {
        if (brightness > prevNum)
            for (int i = 0; i < numPixels; i += channels) {
                data[i] = std::min(255, std::max(0, data[i] - brightness));       // Red
                data[i + 1] = std::min(255, std::max(0, data[i + 1] - brightness)); // Green
                data[i + 2] = std::min(255, std::max(0, data[i + 2] - brightness)); // Blue
                // Alpha channel remains unchanged
            }

        if (brightness < prevNum)
        {
            for (int i = 0; i < numPixels; i += channels) {
                data[i] = std::min(255, std::max(0, data[i] + brightness));       // Red
                data[i + 1] = std::min(255, std::max(0, data[i + 1] + brightness)); // Green
                data[i + 2] = std::min(255, std::max(0, data[i + 2] + brightness)); // Blue
            }
        }
    }

    //brightness *= 100;

    return brightness;
}

void Filter::NBrightness(unsigned char* destData, unsigned char* srcData, int width, int height, int channels, int brightness) {
    int numPixels = width * height * channels;
    for (int i = 0; i < numPixels; i += channels) {
        destData[i] = std::min(255, std::max(0, srcData[i] + brightness));       // Red
        destData[i + 1] = std::min(255, std::max(0, srcData[i + 1] + brightness)); // Green
        destData[i + 2] = std::min(255, std::max(0, srcData[i + 2] + brightness)); // Blue
        // Alpha channel remains unchanged
        if (channels == 4) {
            destData[i + 3] = srcData[i + 3]; // Copy alpha channel
        }
    }
}

/*void Filter::AdjustBrightness(GLFWImage& image, int brightness) {
    int width = image.GetWidth();
    int height = image.GetHeight();
    unsigned char* pixels = const_cast<unsigned char*>(image.GetPixels());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 4;
            for (int i = 0; i < 3; ++i) {
                int value = pixels[index + i] + brightness;
                if (value > 255) value = 255;
                if (value < 0) value = 0;
                pixels[index + i] = static_cast<unsigned char>(value);
            }
        }
    }
}*/
/*
void Filter::AdjustBrightness(unsigned char* pixels, int width, int height, int channels, unsigned char* pixels1, int brightness)
{
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 4;
            for (int i = 0; i < 3; ++i) {
                int value = pixels[index + i] + brightness;
                if (value > 255) value = 255;
                if (value < 0) value = 0;
                pixels[index + i] = value;
            }
        }
    }
    
}

void Filter::Contrast(unsigned char* data, int width, int height, int channels, float contrast) {
    int numPixels = width * height * channels;
    for (int i = 0; i < numPixels; i += channels) {
        for (int j = 0; j < channels - 1; ++j) { // Avoid modifying alpha channel
            int pixelValue = data[i + j];
            data[i + j] = std::min(255, std::max(0, static_cast<int>(128 + contrast * (pixelValue - 128))));
        }
    }
}

void Filter::AdjustWhites(unsigned char* data, int width, int height, int channels, int adjustment) {
    int numPixels = width * height * channels;
    for (int i = 0; i < numPixels; i += channels) {
        for (int j = 0; j < channels - 1; ++j) { // Avoid modifying alpha channel
            if (data[i + j] > 128) { // Apply adjustment to lighter pixels
                data[i + j] = std::min(255, std::max(0, data[i + j] + adjustment));
            }
        }
    }
}

void Filter::AdjustBlacks(unsigned char* data, int width, int height, int channels, int adjustment) {
    int numPixels = width * height * channels;
    for (int i = 0; i < numPixels; i += channels) {
        for (int j = 0; j < channels - 1; ++j) { // Avoid modifying alpha channel
            if (data[i + j] < 128) { // Apply adjustment to darker pixels
                data[i + j] = std::min(255, std::max(0, data[i + j] + adjustment));
            }
        }
    }
}
*/
