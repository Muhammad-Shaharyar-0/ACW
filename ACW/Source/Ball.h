#pragma once
#include "GameEntity.h"
class Ball :
    public GameEntity
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

    bool checkCollision(GameEntity& other) {
        Ball* otherSphere = dynamic_cast<Ball*>(&other);
        if (otherSphere) {
            float distance = glm::length(position - otherSphere->position);
            float radiiSum = m_Radius + otherSphere->m_Radius;
            if (distance < radiiSum) {
                return true;
            }
        }
        return false;
    }
    void resolveCollision(GameEntity& other)  {
        Ball* otherSphere = dynamic_cast<Ball*>(&other);
        if (otherSphere) {
            // Calculate the collision normal
            glm::vec3 collisionNormal = glm::normalize(otherSphere->position - position);

            // Calculate the relative velocity between the two spheres
            glm::vec3 relativeVelocity = otherSphere->velocity - velocity;

            // Calculate the impulse scalar
            float impulseScalar = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal) / (1 / mass + 1 / otherSphere->mass);

            // Apply the impulse to both spheres
            glm::vec3 impulse = impulseScalar * collisionNormal;
            velocity += impulse / mass;
            otherSphere->velocity -= impulse / otherSphere->mass;

            // Update the net force and acceleration of the objects
            force = impulse / DELTA_T;
            accerleration = force / mass;
            otherSphere->force = -impulse / DELTA_T;
            otherSphere->accerleration = otherSphere->force / otherSphere->mass;
        }
    }
};

