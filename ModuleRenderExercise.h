#pragma once
#include "Module.h"
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

private:
    unsigned program;
    unsigned int vbo;
};

