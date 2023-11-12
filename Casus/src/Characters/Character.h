#pragma once
#include "src/Object/GameObject.h"

class Character :public GameObject
{
public:
    Character(Properties* props): GameObject(props){}
    virtual void Draw()=0;
    virtual void Clean()=0;
    virtual void Update(float deltaTime)=0;
    
protected:

    std::string m_Name;
};
