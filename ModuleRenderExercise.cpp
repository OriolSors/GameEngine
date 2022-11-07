#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"
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
    
    CreateTriangleVBO();

    char* v_shader_file = App->program->Load("HelloWorld_vs.glsl");
    char* f_shader_file = App->program->Load("HelloWorld_fs.glsl");

    unsigned v_shader = App->program->CompileShader(GL_VERTEX_SHADER, v_shader_file);
    unsigned f_shader = App->program->CompileShader(GL_FRAGMENT_SHADER, f_shader_file);

    program = App->program->CreateProgram(v_shader, f_shader);

    return true;
}

update_status ModuleRenderExercise::Update()
{
    RenderVBO();
    return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    ENGINE_LOG("Destroying VBO");

    DestroyVBO();

    return true;
}

void ModuleRenderExercise::CreateTriangleVBO() {
    float4x4 proj;
    float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void ModuleRenderExercise::RenderVBO()
{
    Frustum frustum;
    //frustum.type = FrustumType::PerspectiveFrustum;
    frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
    frustum.SetPos(float3::zero);
    frustum.SetFront(-float3::unitZ);
    frustum.SetUp(float3::unitY);
    frustum.SetViewPlaneDistances(0.1f, 100.0f);
    frustum.SetPerspective(2.f * atanf(tanf(math::pi / 4.0f * 0.5f) * SCREEN_WIDTH/SCREEN_HEIGHT), math::pi / 4.0f);

    float4x4 proj = frustum.ProjectionMatrix();

    float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
        float4x4::RotateZ(pi / 4.0f),
        float3(2.0f, 1.0f, 0.0f));
    float4x4 view = float4x4::LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY, float3::unitY);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
    glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ModuleRenderExercise::DestroyVBO()
{
	glDeleteBuffers(1, &vbo);
}
