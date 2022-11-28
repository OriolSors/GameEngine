#include "ModuleTexture.h"
#include "GL\glew.h"
#include "Game/DirectXTex-main/DirectXTex/DirectXTex.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
    glGenTextures(1, &texture_object);
    glBindTexture(GL_TEXTURE_2D, texture_object);
    //glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8,512, 512,0,GL_BGR,GL_UNSIGNED_BYTE,const void* data);
  
    return true;
}

update_status ModuleTexture::PreUpdate()
{
    return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
    return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
    return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
    glDeleteTextures(1, &texture_object);
    return true;
}
