#pragma once
#include "GameEntity.h"

class Agent :
    public GameEntity
{
private:
    float radius=0.5; // radius of the cylinder
    float height=1; // height of the cylinder
public:

    Agent()
    {
        type = EntityType::Tower;
    }
	void Update(const float pSeconds);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);

    glm::vec3 GetPosition() const
    {
        glm::mat4 model = GetModel();
        glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
        return translation;
    }

    bool checkCollision(GameEntity& other)  {
        Agent* otherCylinder = dynamic_cast<Agent*>(&other);
        if (otherCylinder) {
            float distance = glm::length(position - otherCylinder->position);
            float radiiSum = radius + otherCylinder->radius;
            float heightDiff = abs(height - otherCylinder->height);
            if (distance < radiiSum && heightDiff < radiiSum) {
                return true;
            }
        }
        return false;
    }
    void resolveCollision(GameEntity& other)  {
        Agent* otherCylinder = dynamic_cast<Agent*>(&other);
        if (otherCylinder) {
            // Calculate the collision normal
            glm::vec3 collisionNormal = glm::normalize(otherCylinder->position - position);

            // Calculate the relative velocity between the two cylinders
            glm::vec3 relativeVelocity = otherCylinder->velocity - velocity;

            // Calculate the impulse scalar
            float dot = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal);
            float div = (1 / ((mass + 1) / otherCylinder->mass));
            float impulseScalar =  dot/div ;

            //// Apply the impulse to both cylinders
            //glm::vec3 impulse = impulseScalar * collisionNormal;
            //velocity += impulse / mass;

            //otherCylinder->velocity -= impulse / otherCylinder->mass;

            //force = impulse / DELTA_T;
            //accerleration = force / mass;
            //otherCylinder->force = -impulse / DELTA_T;
            //otherCylinder->accerleration = otherCylinder->force / otherCylinder->mass;
            force = glm::vec3(0, 0, 0);
            accerleration = glm::vec3(0, 0, 0);
            velocity = glm::vec3(0, 0, 0);

        }
    }
};


