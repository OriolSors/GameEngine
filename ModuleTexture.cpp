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
    DirectX::TexMetadata md;
    auto img = new DirectX::ScratchImage();
    bool loaded = true;

    
    HRESULT res = DirectX::LoadFromDDSFile(L"./Assets/baboon.png", DirectX::DDS_FLAGS_NONE, nullptr, *img);

    
    if (FAILED(res))res = DirectX::LoadFromTGAFile(L"./Assets/baboon.png", DirectX::TGA_FLAGS_NONE, nullptr, *img);
        
    if (FAILED(res))res = DirectX::LoadFromWICFile(L"./Assets/baboon.png", DirectX::WIC_FLAGS_NONE, nullptr, *img);

    

    DirectX::ScratchImage destImg;
    res = FlipRotate(img->GetImages(), img->GetImageCount(), img->GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, destImg);

    
    glGenTextures(1, &texture_object);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_object);
    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, destImg.GetMetadata().width, destImg.GetMetadata().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, destImg.GetPixels());
    glGenerateMipmap(GL_TEXTURE_2D);

    
    return true;
}

update_status ModuleTexture::PreUpdate()
{
    return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texture_object);

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
