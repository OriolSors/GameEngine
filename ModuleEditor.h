#pragma once
#include "Module.h"
#include "Globals.h"
#include "Game/MathGeoLib/Math/float3.h"
#include "Game/ImGui/imgui.h"
#include "Timer.h"
#include <vector>
class ModuleEditor :
    public Module
{
public:
    ModuleEditor();
    ~ModuleEditor();

    bool Init();
    update_status PreUpdate(float deltaTime);
    update_status Update(float deltaTime);
    update_status PostUpdate(float deltaTime);
    bool CleanUp();

    void PlotFPS(float fps, float ms);

private:
    std::vector<float> fpsLOG;
    std::vector<float> msLOG;

    int fpsIndexLOG = 0;
    int fpsCounter = 0;
};
