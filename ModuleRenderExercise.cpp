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
}

bool ModuleRenderExercise::Init()
{  
    const char* v_shader_file = App->program->Load("HelloWorld_vs.glsl");
    const char* f_shader_file = App->program->Load("HelloWorld_fs.glsl");

    unsigned v_shader = App->program->CompileShader(GL_VERTEX_SHADER, v_shader_file);
    unsigned f_shader = App->program->CompileShader(GL_FRAGMENT_SHADER, f_shader_file);

    program = App->program->CreateProgram(v_shader, f_shader);

    //CreateTriangleVBO();
    //modelMatrix = float4x4::FromTRS(float3::zero, float4x4::identity, float3(0.05f, 0.05f, 0.05f));
    modelMatrix = float4x4::identity;
    
    model->Load("Assets/Textures/baker_house_model/BakerHouse.fbx");
    lookAtPoint = model->GetCentreAABB();
    ENGINE_LOG("LOOK AT: (X= %.6f, Y= %.6f, Z= %.6f)", lookAtPoint.x, lookAtPoint.y, lookAtPoint.z);
    return true;
}

update_status ModuleRenderExercise::Update(float deltaTime)
{
    //RenderTriangle();

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
float3 ModuleRenderExercise::GetLookAtPoint() {

    return lookAtPoint;
}

void ModuleRenderExercise::SetModel(const char* pathModel)
{
    model->Clear();
    model->Load(pathModel);
    lookAtPoint = model->GetCentreAABB();
    ENGINE_LOG("Model changed");
}

void ModuleRenderExercise::CreateTriangleVBO() {
    float vtx_data[] = {
        -1.0f, -1.0f, 0.0f, // ← v0 pos
        0.0f, 0.0f,         // ← v0 texcoord

        1.0f, -1.0f, 0.0f,  // ← v1 pos
        1.0f, 0.0f,         // ← v1 texcoord

        0.0f, 1.0f, 0.0f,   // ← v2 pos
        0.5f, 1.0f          // ← v2 texcoord
    };


    //glGenBuffers(1, &vbo);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

    /*
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // set ebo active
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

    */

}

void ModuleRenderExercise::RenderTriangle()
{

    float4x4 proj = App->camera->GetProjectionMatrix();
    float4x4 view = App->camera->GetViewMatrix();

    float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
        float4x4::RotateZ(pi / 4.0f),
        float3(2.0f, 1.0f, 0.0f));

    glClearColor(0.15f, 0.15f, 0.15f, 1);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(program);
    
    glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
    glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

    //glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(float) * 3*6));
    
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, App->texture->texture_object);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0);

    int w;
    int h;

    SDL_GetWindowSize(App->window->window, &w, &h);

    App->debugDraw->Draw(view, proj, w, h);
    
    
}

void ModuleRenderExercise::DestroyVBO()
{
	//glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);
}
