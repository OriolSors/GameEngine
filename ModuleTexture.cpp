#include "ModuleTexture.h"
#include "GL\glew.h"
#include "Game/DirectXTex/DirectXTex.h"
#include <assimp/scene.h>

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
    return true;
}

update_status ModuleTexture::PreUpdate(float deltaTime)
{
    return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update(float deltaTime)
{

    return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate(float deltaTime)
{
    return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
    glDeleteTextures(1, &texture_object);
    return true;
}

bool ModuleTexture::Load(const char* textureFile, int& w, int& h)
{
    auto img = new DirectX::ScratchImage();

    wchar_t* vOut = new wchar_t[strlen(textureFile) + 1];
    mbstowcs_s(NULL, vOut, strlen(textureFile) + 1, textureFile, strlen(textureFile));

    HRESULT res = DirectX::LoadFromDDSFile(vOut, DirectX::DDS_FLAGS_NONE, &md, *img);


    if (FAILED(res))res = DirectX::LoadFromTGAFile(vOut, DirectX::TGA_FLAGS_NONE, &md, *img);

    if (FAILED(res))res = DirectX::LoadFromWICFile(vOut, DirectX::WIC_FLAGS_NONE, &md, *img);

    if (FAILED(res)) return false;

    DirectX::ScratchImage* destImg = new DirectX::ScratchImage();
    res = FlipRotate(img->GetImages(), img->GetImageCount(), md, DirectX::TEX_FR_FLIP_VERTICAL, *destImg);

    glGenTextures(1, &texture_object);
    glBindTexture(GL_TEXTURE_2D, texture_object);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);


    switch (destImg->GetMetadata().format)
    {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_BGRA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B5G6R5_UNORM:
            internalFormat = GL_RGB8;
            format = GL_BGR;
            type = GL_UNSIGNED_BYTE;
            break;
        default:
            assert(false && "Unsupported format");
    }

    w = md.width;
    h = md.height;
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, md.width, md.height, 0, format, type, destImg->GetPixels());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    delete img;
    delete destImg;
    delete vOut;

    return true;
}
