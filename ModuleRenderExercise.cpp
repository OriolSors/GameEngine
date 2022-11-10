#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
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
    char* v_shader_file = App->program->Load("HelloWorld_vs.glsl");
    char* f_shader_file = App->program->Load("HelloWorld_fs.glsl");

    unsigned v_shader = App->program->CompileShader(GL_VERTEX_SHADER, v_shader_file);
    unsigned f_shader = App->program->CompileShader(GL_FRAGMENT_SHADER, f_shader_file);

    program = App->program->CreateProgram(v_shader, f_shader);

    CreateTriangleVBO();

    return true;
}

update_status ModuleRenderExercise::Update()
{
    RenderTriangle();
    return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    ENGINE_LOG("Destroying VBO");

    DestroyVBO();

    return true;
}

void ModuleRenderExercise::CreateTriangleVBO() {
    float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

    /*
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    */

}

void ModuleRenderExercise::RenderTriangle()
{
    float aspect = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
    Frustum frustum;
    frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
    frustum.SetPos(float3(0.0f, 2.0f, 10.0f));
    frustum.SetFront(-float3::unitZ);
    frustum.SetUp(float3::unitY);
    frustum.SetViewPlaneDistances(0.1f, 100.0f);
    frustum.SetPerspective(2.f * atanf(tanf(VFOV * 0.5f) * aspect), VFOV);


    float4x4 proj = frustum.ProjectionMatrix();

    float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
        float4x4::RotateZ(pi / 4.0f),
        float3(2.0f, 1.0f, 0.0f));
    float4x4 view = frustum.ViewMatrix();

    glClearColor(0.15f, 0.15f, 0.15f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(program);
    
    glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
    glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    App->debugDraw->Draw(view, proj, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
}

void ModuleRenderExercise::DestroyVBO()
{
	glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);
}
