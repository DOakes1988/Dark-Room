#include "Application.h"
#include "ToolManager.h"
#include <vector>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <wtypes.h>


#define GL_SILENCE_DEPRECATION

const char* glsl_version = "#version 430";

Application::Application()
{
}

Application::~Application()
{
    
}

bool Application::init(std::string windowName)
{
   
    return true;
}

void Application::run()
{
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        // Create window with graphics context
        GLFWwindow* window = glfwCreateWindow(3000, 1700, "Main Window", nullptr, nullptr);


        if (!window) {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            return;
        }

        glfwSetErrorCallback(glfw_error_callback);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize OpenGL functions!" << std::endl;
            return;
        }

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
   
        // Our state
        bool load_image = false;
        bool show_demo_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);        

        GLuint my_image_texture = 0;
        std::string filePath = "";
        
        static ImVec4 color;
        float exposure = 0.0f;
        float contrast = 0.0f;
        float saturation = 0.0f;
        float whites = 0.0f;
        float blacks = 0.0f;
        float shadows = 0.0f;

        float filterExposure = 0.0f;
        float filterContrast = 0.0f;
        float filterSaturation = 0.0f;
        float filterWhites = 0.0f;
        float filterBlacks = 0.0f;
        float filterShadows = 0.0f;
        
        static int zoomFactor = 2;

        int imageWidth = 2250;
        int imageHeight = 1500;

        float relativeMouseX = 0.0f;
        float relativeMouseY = 0.0f;

        float zoomCenterX = 0.0f;
        float zoomCenterY = 0.0f;
        float zoom_level = 1.0f;

        static Slider exposureSlider("Exposure", -5.0f, 5.0f);
        static Slider contrastSlider("Contrast", -5.0f, 5.0f);
        static Slider shadowSlider("Shadows", -5.0f, 5.0f);
        static Slider whiteSlider("White", -5.0f, 5.0f);
        static Slider blackSlider("Black", -5.0f, 5.0f);
        static Slider saturationSlider("Saturation", -5.0f, 5.0f);
        
        static Slider filterExposureSlider("FExposure", -5.0f, 5.0f);
        static Slider filterContrastSlider("FContrast", -5.0f, 5.0f);
        static Slider filterShadowSlider("FShadows", -5.0f, 5.0f);
        static Slider filterWhiteSlider("FWhite", -5.0f, 5.0f);
        static Slider filterBlackSlider("FBlack", -5.0f, 5.0f);
        static Slider filterSaturationSlider("FSaturation", -5.0f, 5.0f);
        
        ImVec2 uv0 = ImVec2(0, 0);
        ImVec2 uv1 = ImVec2(1, 1);

        ImGuiImage image(imageWidth, imageHeight, 4);
        ImTextureID imageTexture = 0;

        bool zoomFlag = false;
        bool imageLoaded = false;

        float hue_left[8] = { 330.0f / 360.0f, 0.0f, 30.0f / 360.0f, 60.0f / 360.0f, 120.0f / 360.0f, 180.0f / 360.0f, 240.0f / 360.0f, 270.0f / 360.0f };
        float hue_right[8] = { 30.0f / 360.0f, 60.0f / 360.0f, 120.0f / 360.0f, 150.0f / 360.0f, 210.0f / 360.0f, 270.0f / 360.0f, 330.0f / 360.0f, 360.0f / 360.0f };

        ColorAdjustmentUI adjustmentUI;
        //LinearGradientFilter linear_gradient_filter;
        bool linearGradientFilterFlag = false;
 
        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            // Poll Events
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Allows Window Docking
            Docking();

            static ImVec4 color;
            static bool opt_padding = false;


            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            static float f = 1.0f;
            static int a = 1;

            static float hue_shifts[8] = { 0.0f };
            static float sat_shifts[8] = { 0.0f };
            static float lum_shifts[8] = { 0.0f };

            if (show_demo_window)
            {
                ImGui::ShowDemoWindow();
            }

            ImGui::Begin("  ");
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::Checkbox("Open File", &load_image);

            if (load_image)
            {
                std::cout << "Image Loaded!!" << std::endl;
                blacks = 0.0f;
                exposure = 0.0f;
                contrast = 0.0f;
                saturation = 0.0f;
                shadows = 0.0f;
                whites = 0.0f;

                filterBlacks = 0.0f;
                filterExposure = 0.0f;
                filterContrast = 0.0f;
                filterSaturation = 0.0f;
                filterShadows = 0.0f;
                filterWhites = 0.0f;

                for (int i = 0; i < 8; i++)
                {
                    hue_shifts[i] = 0.0f;
                    sat_shifts[i] = 0.0f;
                    lum_shifts[i] = 0.0f;
                }

                filePath = openFileDialog(window, false);

                if (!filePath.empty()) {
                    image = ImGuiImage::LoadFromFile(filePath);
                    if (!image.IsValid()) {  // ? Check if loading failed
                        std::cerr << "Failed to load image!" << std::endl;
                        imageLoaded = false;
                    }
                    else {
                        imageLoaded = true;
                    }
                }

                image.AdjustBlack(blacks);
                image.AdjustBrightness(exposure);
                image.AdjustContrast(contrast);
                image.AdjustSaturation(saturation);
                image.AdjustShadows(shadows);
                image.AdjustWhite(whites);

                image.AdjustFilterBrightness(filterExposure);
                image.AdjustFilterContrast(filterContrast);
                image.AdjustFilterShadows(filterShadows);
                image.AdjustFilterWhite(filterWhites);
                image.AdjustFilterBlack(filterBlacks);
                image.AdjustFilterSaturation(filterSaturation);

                image.UploadToGPU();
                load_image = false;
                //imageLoaded = true;
            }

            

            exposureSlider.Render(&exposure, 1);
            contrastSlider.Render(&contrast, 2);
            shadowSlider.Render(&shadows, 3);
            whiteSlider.Render(&whites, 4);
            blackSlider.Render(&blacks, 5);
            saturationSlider.Render(&saturation, 6);

            image.AdjustBrightness(exposure);
            image.AdjustContrast(contrast);
            image.AdjustShadows(shadows);
            image.AdjustWhite(whites);
            image.AdjustBlack(blacks);
            image.AdjustSaturation(saturation);

            adjustmentUI.Render(hue_shifts, sat_shifts, lum_shifts, hue_left, hue_right);
            image.AdjustHSL(hue_shifts, sat_shifts, lum_shifts, hue_left, hue_right);

            ImGui::End();

            ImGui::Begin(" ");

            filterExposureSlider.Render(&filterExposure, 7);
            filterContrastSlider.Render(&filterContrast, 8);
            filterShadowSlider.Render(&filterShadows, 9);
            filterWhiteSlider.Render(&filterWhites, 10);
            filterBlackSlider.Render(&filterBlacks, 11);
            filterSaturationSlider.Render(&filterSaturation, 12);

            image.AdjustFilterBrightness(filterExposure);
            image.AdjustFilterContrast(filterContrast);
            image.AdjustFilterShadows(filterShadows);
            image.AdjustFilterWhite(filterWhites);
            image.AdjustFilterBlack(filterBlacks);
            image.AdjustFilterSaturation(filterSaturation);

            ImGui::End();

            ImGui::Begin("Image Window", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

            ImGui::Text("Zoom  ");
            ImGui::SameLine();
            if (ImGui::Button("-") && zoomFactor > 0)
                zoomFactor -= 1;

            ImGui::SameLine();
            if (ImGui::Button("+"))
                zoomFactor += 1;

            ImGui::SameLine();
            ImGui::Text("zoomFactor: %d", zoomFactor);

            float window_width = ImGui::GetContentRegionAvail().x;
            float window_height = ImGui::GetContentRegionAvail().y;
            float aspect_ratio = imageWidth / imageHeight;
            float new_width = window_width;
            float new_height = new_width * aspect_ratio;
            float padding = 20.0f;

            

            


            ImVec2 image_pos = ImGui::GetCursorScreenPos();
            ImVec2 image_size = ImVec2((float)window_width, (float)window_height);
           
            // Padding for window centering
            //ImGui::Dummy(ImVec2(padding, padding));
            //ImGui::SameLine();

            image.LinearGradientFilter(image, image_size);

            image.RenderCenteredImage(imageWidth, imageHeight, new_width, new_height, 20.0f, uv0, uv1, zoom_level);

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                if (imageLoaded)
                {
                    ImVec2 click_pos = ImGui::GetMousePos();
                    image.HandleZoom(click_pos, image_pos, image_size, uv0, uv1, zoom_level, zoomFactor, zoomFlag);
                }
            }

            image.RenderHistogram();

            ImGui::End();

            ImGui::Begin("Color");
            ImGui::ColorPicker4("Color Picker", (float*)&color, 0, nullptr);
            ImGui::End();

            ImGui::End();

            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            glfwSwapBuffers(window);
        }

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
}
