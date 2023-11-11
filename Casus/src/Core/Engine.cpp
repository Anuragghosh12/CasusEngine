﻿#include "Engine.h"
#include <iostream>

#include "../../src/Graphics/TextureManager.h"

Engine* Engine::s_Instance=nullptr;

Engine* Engine::GetInstance()
{
    return s_Instance=(s_Instance!=nullptr)?s_Instance:new Engine();
}

bool Engine::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO)!=0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!=0)
    {
        SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
        return false;
    }
    m_Window = SDL_CreateWindow("Casus Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if(m_Window==nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    m_Renderer=SDL_CreateRenderer(m_Window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_Renderer==nullptr)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    TextureManager::GetInstance()->Load("moon", "assets/moon.png");
    return m_IsRunning=true;
    
    
}

bool Engine::Clean()
{
    return true;
}


void Engine::Update()
{
    
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer,124,218,254,255);
    SDL_RenderClear(m_Renderer);

    TextureManager::GetInstance()->Draw("moon",100,100,300,300);
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            Quit();
        break;
    }
}
void Engine::Quit()
{
    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
}

Engine::Engine()
{
    m_IsRunning=false;
}
