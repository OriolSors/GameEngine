#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "GL\glew.h"
#include "SDL.h"
#include "Game/MathGeoLib/Geometry/Frustum.h"
#include <iostream>


ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
    delete model;
}

bool ModuleRenderExercise::Init()
{  
    const char* v_shader_file = App->program->Load("Assets/HelloWorld_vs.glsl");
    const char* f_shader_file = App->program->Load("Assets/HelloWorld_fs.glsl");

    unsigned v_shader = App->program->CompileShader(GL_VERTEX_SHADER, v_shader_file);
    unsigned f_shader = App->program->CompileShader(GL_FRAGMENT_SHADER, f_shader_file);

    program = App->program->CreateProgram(v_shader, f_shader);

    modelMatrix = float4x4::identity;
    
    model->Load("Assets/Textures/baker_house_model/BakerHouse.fbx");
    App->camera->Focus(model);
    return true;
}

update_status ModuleRenderExercise::Update(float deltaTime)
{

    float4x4 proj = App->camera->GetProjectionMatrix();
    float4x4 view = App->camera->GetViewMatrix();

    glClearColor(0.15f, 0.15f, 0.15f, 1);

    int w;
    int h;

    SDL_GetWindowSize(App->window->window, &w, &h);

    model->Draw();

    App->debugDraw->Draw(view, proj, w, h);

    return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    ENGINE_LOG("Destroying VBO");

    DestroyVBO();

    return true;
}

unsigned ModuleRenderExercise::GetProgram() {
    return program;
}

float4x4 ModuleRenderExercise::GetModelMatrix() {
    
    return modelMatrix;
}


void ModuleRenderExercise::SetModel(const char* pathModel)
{
    model->Clear();
    model->Load(pathModel);
    App->camera->Focus(model);
    ENGINE_LOG("Model changed");
}


void ModuleRenderExercise::DestroyVBO()
{
	
    glDeleteProgram(program);
}
