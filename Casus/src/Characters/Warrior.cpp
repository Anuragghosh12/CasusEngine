﻿#include "Warrior.h"
#include "src/Graphics/TextureManager.h"
#include "SDL.h"

Warrior::Warrior(Properties *props): Character(props)
{
    m_Animation=new Animation();
    m_Animation->SetProps(m_TextureID,0,6,120,SDL_FLIP_HORIZONTAL);
}

void Warrior::Draw()
{
    m_Animation->Draw(m_Transform->X,m_Transform->Y,m_Width,m_Height);
}

void Warrior::Clean()
{
    TextureManager::GetInstance()->Clean();
}

void Warrior::Update(float deltaTime)
{
    m_Animation->Update();
}
