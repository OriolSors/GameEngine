#pragma once
#include "Module.h"
#include "Model.h"
#include "Game/MathGeoLib/Math/float4x4.h"
class ModuleRenderExercise :
    public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();

    bool Init();
    update_status Update(float deltaTime);
    bool CleanUp();

    void CreateTriangleVBO();
    void RenderTriangle();
    void DestroyVBO();

    unsigned GetProgram();
    float4x4 GetModelMatrix();

    Model& GetModel() { return *model; }
    void SetModel(const char* pathModel);

private:
    unsigned program;
    Model* model = new Model();
    float4x4 modelMatrix = float4x4::identity;
};

