#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
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

update_status ModuleEditor::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();

    horizontalFOV = App->camera->GetHorizontalFOV();
    aspectRatio = App->camera->GetAspectRatio();
    pos = App->camera->GetPos();
    distanceNearClippingPlane = App->camera->GetDistanceNearClippingPlane();
    distanceFarClippingPlane = App->camera->GetDistanceFarClippingPlane();
    rotationSpeed = App->camera->GetRotationSpeed();

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    /*
    if (ImGui::BeginMenu("Help")) {
        ImGui::MenuItem("Gui Demo");
        ImGui::EndMenu();
    }
    */
    
    
    ImGui::MenuItem("Gui Demo");

    if (ImGui::SliderFloat("Horizontal FOV", &horizontalFOV, 80.0f, 120.0f)) {
        App->camera->SetHorizontalFOV(horizontalFOV);
    }
    if (ImGui::SliderFloat("Aspect Ratio", &aspectRatio, 1.0f, 2.0f)) {
        App->camera->SetAspectRatio(aspectRatio);
    }
    if (ImGui::SliderFloat("Near Clipping Plane distance", &distanceNearClippingPlane, 0.1f, 5.0f)) {
        App->camera->SetPlaneDistances(distanceNearClippingPlane, distanceFarClippingPlane);
    }
    if (ImGui::SliderFloat("Far Clipping Plane distance", &distanceFarClippingPlane, 10.0f, 15.0f)) {
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

    if (ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 1.0f, 5.0f)) {
        App->camera->SetRotationSpeed(rotationSpeed);
    }
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
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