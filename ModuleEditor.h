#pragma once
#include "Module.h"
#include "Globals.h"
#include "Game/MathGeoLib/Math/float3.h"
#include "Timer.h"
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

private:
    float horizontalFOV;
    float aspectRatio;
    float3 pos;
    float distanceNearClippingPlane;
    float distanceFarClippingPlane;
    float rotationSpeed;
    float cameraSpeedMultiplier;

    Timer timer;
};
