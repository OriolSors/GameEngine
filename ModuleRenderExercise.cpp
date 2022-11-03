#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"
#include "GL\glew.h"
#include "SDL.h"
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
    //CreateTriangleVBO();

    return true;
}

update_status ModuleRenderExercise::Update()
{
    RenderVBO();
    return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    LOG("Destroying VBO");

    DestroyVBO();

    return true;
}

void ModuleRenderExercise::CreateTriangleVBO() {

    float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void ModuleRenderExercise::RenderVBO()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ModuleRenderExercise::DestroyVBO()
{
	glDeleteBuffers(1, &vbo);
}
