#pragma once
#include "Module.h"
#include "Model.h"
class ModuleRenderExercise :
    public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();

    bool Init();
    update_status Update();
    bool CleanUp();

    void CreateTriangleVBO();
    void RenderTriangle();
    void DestroyVBO();

    unsigned GetProgram();
private:
    unsigned program;
    Model* model = new Model();
};

