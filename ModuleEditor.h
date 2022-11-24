#pragma once
#include "Module.h"
#include "Globals.h"
#include "Game/MathGeoLib/Math/float3.h"
class ModuleEditor :
    public Module
{
public:
    ModuleEditor();
    ~ModuleEditor();

    bool Init();
    update_status PreUpdate();
    update_status Update();
    update_status PostUpdate();
    bool CleanUp();

private:
    float horizontalFOV;
    float aspectRatio;
    float3 pos;
    float distanceNearClippingPlane;
    float distanceFarClippingPlane;
    float rotationSpeed;
};
