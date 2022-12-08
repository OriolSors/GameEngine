#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleRenderExercise.h"
#include "ModuleCamera.h"
#include "Game/MathGeoLib/Math/float4x4.h"
#include "Game/MathGeoLib/Math/float3x3.h"
#include "Game/ImGui/imgui.h"
#include "Game/ImGui/imgui_impl_sdl.h"
#include "Game/ImGui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

ModuleEditor::ModuleEditor()
{

}

ModuleEditor::~ModuleEditor()
{
    fpsLOG.clear();
    msLOG.clear();
}

bool ModuleEditor::Init()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    return true;
}

update_status ModuleEditor::PreUpdate(float deltaTime)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float deltaTime)
{
    float horizontalFOV = App->camera->GetHorizontalFOV();
    float aspectRatio = App->camera->GetAspectRatio();
    float3 pos = App->camera->GetPos();
    float distanceNearClippingPlane = App->camera->GetDistanceNearClippingPlane();
    float distanceFarClippingPlane = App->camera->GetDistanceFarClippingPlane();
    float rotationSpeed = App->camera->GetRotationSpeed();
    float cameraSpeedMultiplier = App->camera->GetCameraSpeedMultiplier();

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Configuration"))
    {
        if (ImGui::CollapsingHeader("Camera Settings")) {
            if (ImGui::SliderFloat("Horizontal FOV", &horizontalFOV, 80.0f, 120.0f)) {
                App->camera->SetHorizontalFOV(horizontalFOV);
            }
            if (ImGui::SliderFloat("Aspect Ratio", &aspectRatio, 1.0f, 2.0f)) {
                App->camera->SetAspectRatio(aspectRatio);
            }
            if (ImGui::SliderFloat("Near Clipping Plane distance", &distanceNearClippingPlane, 0.15f, 5.0f)) {
                App->camera->SetPlaneDistances(distanceNearClippingPlane, distanceFarClippingPlane);
            }
            if (ImGui::SliderFloat("Far Clipping Plane distance", &distanceFarClippingPlane, 10.0f, 250.0f)) {
                App->camera->SetPlaneDistances(distanceNearClippingPlane, distanceFarClippingPlane);
            }
            if (ImGui::InputFloat("X: ", &pos.x)) {
                App->camera->SetPosition(pos);
            }
            if (ImGui::InputFloat("Y: ", &pos.y)) {
                App->camera->SetPosition(pos);
            }
            if (ImGui::InputFloat("Z: ", &pos.z)) {
                App->camera->SetPosition(pos);
            }

            if (ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.25f, 5.0f)) {
                App->camera->SetRotationSpeed(rotationSpeed);
            }

            if (ImGui::SliderFloat("Camera Multiplier Speed", &cameraSpeedMultiplier, 1.5f, 5.0f)) {
                App->camera->SetCameraSpeedMultiplier(cameraSpeedMultiplier);
            }

        }
        if (ImGui::CollapsingHeader("Application")) {
            if (ImGui::InputText("App name", App->windowName, IM_ARRAYSIZE(App->windowName)))
            {
                SDL_SetWindowTitle(App->window->window, App->windowName);
            }
            ImGui::InputText("Organization", App->organization, IM_ARRAYSIZE(App->organization));
            ImGui::SliderInt("Max FPS", &App->maxFPS, 10, 240);
            ImGui::Text("Limit Framerate:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%i", App->maxFPS);

            char title[25];
            sprintf_s(title, 25, "Framerate %.1f", fpsLOG[fpsLOG.size()-1]);
            ImGui::PlotHistogram("##framerate", &fpsLOG[0], fpsLOG.size(), fpsIndexLOG, title, 0.0f, 100.0f, ImVec2(310, 100));
            sprintf_s(title, 25, "Milliseconds %0.1f", msLOG[msLOG.size() - 1]);
            ImGui::PlotHistogram("##milliseconds", &msLOG[0], msLOG.size(), fpsIndexLOG, title, 0.0f, 40.0f, ImVec2(310, 100));
            
            
        }
        if (ImGui::CollapsingHeader("Window")) {
            int w = App->window->GetWidth();
            int h = App->window->GetHeight();

            float brightness = App->window->GetBrightness();

            bool fullscreen = App->window->IsFullscreen();
            bool resizable = App->window->IsResizable();
            bool borderless = App->window->IsBorderless();
            bool fullScreenDesktop = App->window->IsFullScreenDesktop();

            if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
                App->window->SetBrightness(brightness);

            if (ImGui::SliderInt("Width", &w, 640, 4096))
                App->window->SetWidth(w);

            if (ImGui::SliderInt("Height", &h, 480, 2160))
                App->window->SetHeigth(h);

            ImGui::Text("Refresh rate:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%i", App->window->GetRefreshRate());

            if (ImGui::Checkbox("Fullscreen", &fullscreen))
                App->window->SetFullscreen(fullscreen);

            ImGui::SameLine();

            if (ImGui::Checkbox("Resizable", &resizable))
                App->window->SetResizable(resizable);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Restart to apply");

            if (ImGui::Checkbox("Borderless", &borderless))
                App->window->SetBorderless(borderless);

            ImGui::SameLine();

            if (ImGui::Checkbox("Full Desktop", &fullScreenDesktop))
                App->window->SetFullScreenDesktop(fullScreenDesktop);
        }
        if (ImGui::CollapsingHeader("Hardware")) {
            
            ImGui::Text("SDL version:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), App->sdlVersion);
            
            ImGui::Separator();

            ImGui::Text("CPUs:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%i (Cache: %i kb)", App->numCPU, App->sizeCache);

            ImGui::Text("System RAM:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%.1f Gb", App->sizeRam);

            ImGui::Text("Caps:");
            const char* caps[] =
            {
                "3DNow", 
                "AltiVec",  
                "AVX", 
                "AVX2",   
                "MMX",
                "RDTSC", 
                "SSE", 
                "SSE2", 
                "SSE3", 
                "SSE41", 
                "SSE42"
            };
            for (int i = 0; i < IM_ARRAYSIZE(caps); ++i)
            {
                if (App->caps[i])
                {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), caps[i]);
                }
                
            }

            ImGui::Separator();

            ImGui::Text("GPU:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%s", glGetString(GL_VENDOR));

            ImGui::Text("Brand:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%s", glGetString(GL_RENDERER));

            ImGui::Text("GPU OpenGL Version:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%s", glGetString(GL_VERSION));

            ImGui::Text("VRAM Budget:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%.1f Mb", App->vramBudget);

            ImGui::Text("VRAM Usage:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%.1f Mb", App->vramUsage);

            ImGui::Text("VRAM Available:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%.1f Mb", App->vramAvailable);

            ImGui::Text("VRAM Reserved:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%.1f Mb", App->vramReserved);
        }
        
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Properties"))
    {
        if (ImGui::CollapsingHeader("Transformation")) {
            float4x4 model = App->exercise->GetModelMatrix();
            float3 position = model.TranslatePart();
            ImGui::InputFloat3("Position", position.ptr(), "%.3f", ImGuiInputTextFlags_ReadOnly);
            float3 rotation = model.RotatePart().ToEulerXYZ();
            ImGui::InputFloat3("Rotation", rotation.ptr(), "%.3f", ImGuiInputTextFlags_ReadOnly);
            float3 scale = model.ExtractScale();
            ImGui::InputFloat3("Scale", scale.ptr(), "%.3f", ImGuiInputTextFlags_ReadOnly);

            
        }
        if (ImGui::CollapsingHeader("Geometry")) {
            Model& model = App->exercise->GetModel();
            unsigned meshIndex = 0;
            for (Mesh* mesh : model.GetMeshes())
            {
                char meshName[20];
                sprintf(meshName, "Mesh %i", meshIndex);
                if (ImGui::TreeNode(meshName))
                {
                    ImGui::Text("Vertices:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%i", mesh->GetNumVertices());

                    ImGui::Text("Triangles:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%i", mesh->GetNumIndices() / 3); //triangles = 3 indices per face

                    ImGui::TreePop();
                }
                meshIndex += 1;
            }

            vec center = model.GetCentreAABB();
            ImGui::InputFloat3("Bounding Box Center", center.ptr(), "%.3f", ImGuiInputTextFlags_ReadOnly);

            float diagonal = model.GetDiagonalAABB();
            ImGui::Text("Diagonal:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%.3f", diagonal);
        }
        if (ImGui::CollapsingHeader("Texture")) {
            Model& model = App->exercise->GetModel();
            unsigned materialIndex = 0;
            for (unsigned material : model.GetMaterials())
            {
                char materialName[20];
                sprintf(materialName, "Material %i", materialIndex);
                if (ImGui::TreeNode(materialName))
                {
                    int width = model.GetWidthMaterials()[materialIndex];
                    int height = model.GetHeightMaterials()[materialIndex];
                   

                    ImGui::Text("Width: ");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%i", width);
                    ImGui::Text("Height: ");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), "%i", height);
                    ImGui::Image((void*)model.GetMaterials()[materialIndex], ImVec2(200, 200));

                    ImGui::TreePop();
                }

                materialIndex += 1;
            }
        }
        ImGui::EndMenu();
    }

        
    if (ImGui::BeginMenu("Console"))
    {
        ImGui::TextUnformatted(logBuffer.c_str());
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("General"))
    {
        if (ImGui::MenuItem("Quit"))
        {
            return UPDATE_STOP;
        }
        if (ImGui::MenuItem("GitHub page"))
        {
            ShellExecuteA(NULL, "open", "https://github.com/OriolSors/GameEngine", NULL, NULL, SW_SHOWNORMAL);
        }
        if (ImGui::CollapsingHeader("About"))
        {
            ImGui::Text("Engine Name:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), App->appName);
            ImGui::Text("Description:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), App->description);
            ImGui::Text("Author:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), App->author);
            ImGui::Text("Libraries:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), App->libraries);
            ImGui::Text("License:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0), App->license);
        }

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float deltaTime)
{
    ImGui::EndFrame();
    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return true;
}

void ModuleEditor::PlotFPS(float fps, float ms) {

    if (fpsCounter == MAX_LOG)
    {
        fpsLOG.erase(fpsLOG.begin());
        msLOG.erase(msLOG.begin());
    }
    else {
        ++fpsCounter;
    }

    fpsLOG.push_back(fps);
    msLOG.push_back(ms);
}

