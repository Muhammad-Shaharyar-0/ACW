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
    PhysicsUpdate(IntegrationMethod::Euler,pDeltaTime);
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
        // Get the vector pointing from the center of the cylinder to the center of the sphere
        glm::vec3 dist = position - agent->position;

        // Project the distance vector onto the cylinder's axis to get the closest point on the cylinder
        float distAlongAxis = glm::dot(dist, agent->cylinderAxis);
        glm::vec3 closestPoint = agent->position + agent->cylinderAxis * distAlongAxis;

        // Check if the closest point is within the cylinder's height
        if (distAlongAxis < 0 || distAlongAxis > agent->height)
            return false;

        // Get the distance between the sphere and the closest point on the cylinder
        float distSq = glm::dot(closestPoint - position, closestPoint - position);

        // Compare the distance squared with the sum of the sphere and cylinder radii squared
        if (distSq <= (m_Radius + agent->radius) * (m_Radius + agent->radius))
        {
            return true;
        }

    }
    Floor* cube = dynamic_cast<Floor*>(&other);
    if (cube) {
        // Find the closest point on the cube to the sphere center
      //  glm::vec3 sphereCenter = position + glm::vec3(0, m_Radius, 0);
        glm::vec3 closestPoint
        (
            glm::clamp(position.x, cube->position.x - cube->halfsize, cube->position.x + cube->halfsize),
            glm::clamp(position.y, cube->position.y - cube->halfsize, cube->position.y + cube->halfsize),
            glm::clamp(position.z, cube->position.z - cube->halfsize, cube->position.z + cube->halfsize)
        );

        // Calculate distance between sphere center and closest point
        float dist = glm::distance(position, closestPoint);

        // Compare distance to sphere radius
        if (dist <= m_Radius) {
            // Sphere and cube are colliding
            return true;
        }


        
    }
    return false;

}
void Ball::resolveCollision(GameEntity& other) {
    //Ball* otherSphere = dynamic_cast<Ball*>(&other);
    //if (otherSphere) {
    //    // Calculate the collision normal
    //    glm::vec3 collisionNormal = glm::normalize(otherSphere->position - position);

    //    // Calculate the relative velocity between the two spheres
    //    glm::vec3 relativeVelocity = otherSphere->velocity - velocity;

    //    // Calculate the impulse scalar
    //    float impulseScalar = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal) / (1 / mass + 1 / otherSphere->mass);

    //    // Apply the impulse to both spheres
    //    glm::vec3 impulse = impulseScalar * collisionNormal;
    //    velocity += impulse / mass;
    //    otherSphere->velocity -= impulse / otherSphere->mass;

    //    // Update the net force and acceleration of the objects
    //    force = impulse / DELTA_T;
    //    accerleration = force / mass;
    //    otherSphere->force = -impulse / DELTA_T;
    //    otherSphere->accerleration = otherSphere->force / otherSphere->mass;
    //}
    Ball* otherSphere = dynamic_cast<Ball*>(&other);
    if (otherSphere) 
    {
        //find distance between sphere centers
        float dist = glm::length(position - otherSphere->position);
        if (dist < m_Radius + otherSphere->m_Radius) {
            //spheres are colliding
            glm::vec3 normal = glm::normalize(position - otherSphere->position);
            // Calculate relative velocity
            glm::vec3 relativeVelocity = otherSphere->velocity - velocity;

            // Calculate impulse scalar
            float impulseScalar = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / (1 / mass + 1 / otherSphere->mass);

            // Calculate impulse
            glm::vec3 impulse = impulseScalar * normal;

            // Apply collision response
           velocity -= impulse / mass;
            otherSphere->velocity += impulse / otherSphere->mass;

            //update netforce
            force -= impulse;
            otherSphere->force += impulse;
        }
    }
    
    Agent* agent = dynamic_cast<Agent*>(&other);
    if (agent)
    {
        // Calculate the vector from the sphere to the cylinder
        glm::vec3 d = position - agent->position;
        // Project d onto the cylinder axis
        float dist = glm::dot(d, agent->cylinderAxis);
        // Clamp dist to the cylinder "radius"
        dist = glm::clamp(dist, -agent->height/2, agent->height / 2);
        // Find the closest point on the cylinder
        glm::vec3 closestPoint = agent->position + dist * agent->cylinderAxis;
        // Calculate the distance between the sphere and the closest point on the cylinder
        float distSq = glm::length(closestPoint - position) * glm::length(closestPoint - position);
        // Check if the sphere is colliding with the cylinder
        if (distSq < (m_Radius + agent->radius) * (m_Radius + agent->radius)) {
            // Calculate the collision normal
            glm::vec3 normal = glm::normalize(closestPoint - position);
            // Calculate the impulse scalar
            float j = (1 + elasticity) * glm::dot(velocity - agent->velocity, normal) / (1 / mass + 1 / agent->mass);
            // Apply the impulse
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

        // Calculate the minimum translation distance
        glm::vec3 distanceVector = position - closestPoint;
        float distance = glm::length(distanceVector);
        float overlap = m_Radius - distance;
        glm::vec3 mtd = distanceVector * (overlap / distance);

        // Update velocities
        float elasticity = 0.5f;  // Coefficient of restitution
        glm::vec3 impulse = mtd * (elasticity + 1.0f) * (1.0f / (1.0f / mass));
        velocity -= impulse / mass;

        // Move spheres out of collision
       position += mtd;
       SetPosition(position);
        // Find the vector from the sphere's center to the closest point on the cube
     //   glm::vec3 sphereCenter = position + glm::vec3(0, m_Radius, 0);
        //glm::vec3 closestPoint = cube->position + glm::clamp(position - cube->position, -cube->halfsize, cube->halfsize);
        //glm::vec3 normal = glm::normalize(position - closestPoint);

        //// Calculate relative velocity
        //glm::vec3 relativeVelocity = velocity - cube->velocity;

        //// Calculate impulse scalar
        //float impulseScalar = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * (1 / mass + 1 / cube->mass));

        //// Calculate impulse
        //glm::vec3 impulse = impulseScalar * normal;

        //// Apply collision response
        //velocity += impulse / mass;
        //cube->velocity -= impulse / cube->mass;

        ////update netforce
        //force += impulse;
        //cube->force -= impulse;
    }

}