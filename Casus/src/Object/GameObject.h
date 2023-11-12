#pragma once
#include <SDL_render.h>

#include "IObject.h"
#include "src/Physics/Transform.h"



struct Properties
{
public:
    Properties(std::string textureID,int x,int y,int width, int height,SDL_RendererFlip flip=SDL_FLIP_NONE)
    {
        X=x;
        Y=y;
        Flip=flip;
        Height=height;
        Width=width;
        TextureID=textureID;
    }
public:
    float X,Y;
    int Width, Height;
    std::string TextureID;
    SDL_RendererFlip Flip;
};
class GameObject
{
public:
    GameObject(Properties* props): m_TextureID(props->TextureID),
    m_Width(props->Width),m_Height(props->Height),m_Flip(props->Flip)
    {
        m_Transform=new Transform(props->X, props->Y);
    }
    


    virtual void Draw()=0;
    virtual void Update(float deltaTime)=0;
    virtual void Clean()=0;

protected:
    Transform* m_Transform;
    int m_Width, m_Height;
    std::string m_TextureID;
    SDL_RendererFlip m_Flip;

    
    
};
