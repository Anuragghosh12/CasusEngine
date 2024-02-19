#pragma once
#include "Character.h"
#include "src/Animation/Animation.h"


class Warrior: public Character
{
public:
    Warrior(Properties *props);
    virtual void Draw();
    virtual void Clean();
    virtual void Update(float deltaTime);
private:
    //int m_Row,m_Frame,m_FrameCount,m_AnimSpeed;


    Animation* m_Animation;
};
