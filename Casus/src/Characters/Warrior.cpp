#include "Warrior.h"
#include "src/Graphics/TextureManager.h"
#include "SDL.h"

Warrior::Warrior(Properties *props): Character(props)
{
    m_Row=0;
    m_FrameCount=6;
    m_AnimSpeed=120;
}

void Warrior::Draw()
{
    TextureManager::GetInstance()->DrawFrame(m_TextureID,m_Transform->X, m_Transform->Y,m_Width,m_Height,m_Row,m_Frame);
}

void Warrior::Clean()
{
    TextureManager::GetInstance()->Clean();
}

void Warrior::Update(float deltaTime)
{
    m_Frame=(SDL_GetTicks()/m_AnimSpeed)%m_FrameCount;
}
