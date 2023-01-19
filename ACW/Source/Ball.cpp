#include "Ball.h"
#include "World.h"
Ball::Ball()
{
	glm::vec3& colour = GetColour();
	colour.x = 0.8f;
	colour.y = 0.1f;
	colour.z = 0.2f;
    type = EntityType::Ball;
	m_Radius = 0.5f;
    mass = 25;
}

void Ball::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Ball::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

void Ball::Update(float pDeltaTime)
{
    PhysicsUpdate(pDeltaTime);
    force = glm::vec3(0, 0, 0);
  //if (position.y <= 0)
  //  {
  //    World::_instance->RemoveEntity(this);
  //    delete this;
  //  }
}

bool Ball::checkCollision(GameEntity& other) 
{
    Ball* otherSphere = dynamic_cast<Ball*>(&other);
    if (otherSphere) 
    {
        float dist = glm::distance(position, otherSphere->position);

        if (dist <= m_Radius + otherSphere->m_Radius) 
        {
            // Spheres are colliding
         //   World::_instance->RemoveEntity(this);
            return true;
        }
    
    }
    Agent* agent = dynamic_cast<Agent*>(&other);
    if (agent) 
    {
        glm::vec3 sphereCenter = position + glm::vec3(0, m_Radius, 0);
        glm::vec3 dist = position - agent->position;

        float distAlongAxis = glm::dot(dist, agent->cylinderAxis);
        glm::vec3 closestPoint = agent->position + agent->cylinderAxis * distAlongAxis;

        if (distAlongAxis < 0 || distAlongAxis > agent->height)
            return false;

        float distSq = glm::dot(closestPoint - position, closestPoint - position);

       if (distSq <= (m_Radius + agent->radius) * (m_Radius + agent->radius))
        {
            return true;
        }

    }
    Floor* cube = dynamic_cast<Floor*>(&other);
    if (cube) {
       glm::vec3 closestPoint
        (
            glm::clamp(position.x, cube->position.x - cube->halfsize, cube->position.x + cube->halfsize),
            glm::clamp(position.y, cube->position.y - cube->halfsize, cube->position.y + cube->halfsize),
            glm::clamp(position.z, cube->position.z - cube->halfsize, cube->position.z + cube->halfsize)
        );

      float dist = glm::distance(position, closestPoint);

       if (dist <= m_Radius) {
            return true;
        }


        
    }
    return false;

}
void Ball::resolveCollision(GameEntity& other) {
    Ball* otherSphere = dynamic_cast<Ball*>(&other);
    if (otherSphere) 
    {
        float dist = glm::length(position - otherSphere->position);
        if (dist < m_Radius + otherSphere->m_Radius) {
           glm::vec3 normal = glm::normalize(position - otherSphere->position);
           glm::vec3 relativeVelocity = otherSphere->velocity - velocity;

           float impulseScalar = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / (1 / mass + 1 / otherSphere->mass);

            glm::vec3 impulse = impulseScalar * normal;

          velocity -= impulse / mass;
            otherSphere->velocity += impulse / otherSphere->mass;

            force -= impulse;
            otherSphere->force += impulse;
        }
    }
    
    Agent* agent = dynamic_cast<Agent*>(&other);
    if (agent)
    {
       glm::vec3 d = position - agent->position;
        float dist = glm::dot(d, agent->cylinderAxis);
        dist = glm::clamp(dist, -agent->height/2, agent->height / 2);
        glm::vec3 closestPoint = agent->position + dist * agent->cylinderAxis;
        float distSq = glm::length(closestPoint - position) * glm::length(closestPoint - position);
       if (distSq < (m_Radius + agent->radius) * (m_Radius + agent->radius)) {
            glm::vec3 normal = glm::normalize(closestPoint - position);
            float j = (1 + elasticity) * glm::dot(velocity - agent->velocity, normal) / (1 / mass + 1 / agent->mass);
            velocity += j / mass * normal;
            agent->velocity -= j / agent->mass * normal;
        }
        agent->TakeDamage(25.0f);
    }
       
    Floor* cube = dynamic_cast<Floor*>(&other);
    if (cube) 
    {
       
        // Find the closest point on the cube to the sphere center
        glm::vec3 closestPoint(
            glm::clamp(position.x, cube->position.x - cube->halfsize, cube->position.x + cube->halfsize),
            glm::clamp(position.y, cube->position.y - cube->halfsize, cube->position.y + cube->halfsize),
            glm::clamp(position.z, cube->position.z - cube->halfsize, cube->position.z + cube->halfsize)
        );

        glm::vec3 distanceVector = position - closestPoint;
        float distance = glm::length(distanceVector);
        float overlap = m_Radius - distance;
        glm::vec3 mtd = distanceVector * (overlap / distance);

       float elasticity = 0.5f; 
        glm::vec3 impulse = mtd * (elasticity + 1.0f) * (1.0f / (1.0f / mass));
        velocity -= impulse / mass;

        // Move spheres out of collision
       position += mtd;
       SetPosition(position);
     //   glm::vec3 sphereCenter = position + glm::vec3(0, m_Radius, 0);
        //glm::vec3 closestPoint = cube->position + glm::clamp(position - cube->position, -cube->halfsize, cube->halfsize);
        //glm::vec3 normal = glm::normalize(position - closestPoint);

       //glm::vec3 relativeVelocity = velocity - cube->velocity;

       //float impulseScalar = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * (1 / mass + 1 / cube->mass));

        //glm::vec3 impulse = impulseScalar * normal;
        //velocity += impulse / mass;
        //cube->velocity -= impulse / cube->mass;

        //force += impulse;
        //cube->force -= impulse;
    }

}