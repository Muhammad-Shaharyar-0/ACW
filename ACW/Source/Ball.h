#pragma once
#include "GameEntity.h"
class Ball : public GameEntity
{
private:
    glm::vec3 m_Velocity;
    float m_Radius;
public:
    Ball();

    glm::vec3 GetPosition() const
    {
        glm::mat4 model = GetModel();
        glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
        return translation;
    }

 

    glm::vec3 GetVelocity() const
    {
        return m_Velocity;
    }

    void SetVelocity(glm::vec3 pVelocity)
    {

    }

    float GetRadius() const
    {
        return m_Radius;
    }

    void SetRadius(float pRadius)
    {
        m_Radius = pRadius;
    }

    void Render(const IRenderHelpers&) const;
    void RenderGui(const IGuiHelpers&);

    void Update(float pDeltaTime);

    bool checkCollision(GameEntity& other);

    void resolveCollision(GameEntity& other);


};

