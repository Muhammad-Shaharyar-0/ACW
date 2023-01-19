#include "Floor.h"


Floor::Floor()
{	
	glm::vec3& colour = GetColour();
	colour.x = 0.6f;
	colour.y = 0.3f;
	colour.z = 0.0f;
	type = EntityType::Floor;
	gCost = 0;
	hCost = 0;
	isWalkable = true;
	isOccupied = false;
	occupiedBy = nullptr;
	mass = 500;
}

void Floor::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Floor::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

void Floor::Update(float pDeltaTime)
{
}

int Floor::getCost() const
{
	return hCost+gCost+difficulty;
}
//
//bool Floor::checkCollision(GameEntity& other) {
//    Ball* otherSphere = dynamic_cast<Ball*>(&other);
//    if (otherSphere) {
//        float dist = glm::distance(position, otherSphere->position);
//
//        if (dist <= m_Radius + otherSphere->m_Radius)
//        {
//            // Spheres are colliding
//         //   World::_instance->RemoveEntity(this);
//            return true;
//        }
//    }
//    Agent* agent = dynamic_cast<Agent*>(&other);
//    if (agent) {
//        // Find distance between center of sphere and cylinder axis
//        float dist = glm::distance(position, agent->GetCenter());
//
//        // Check if sphere is outside cylinder's radius and height
//        if (dist > agent->radius + m_Radius || dist > agent->height / 2 + m_Radius) {
//            return false;
//        }
//
//        // Check if sphere center is inside cylinder's base circle
//        if (glm::length(glm::vec2(position.x, position.z) - glm::vec2(agent->GetCenter().x, agent->GetCenter().z)) < agent->radius) {
//            return true;
//        }
//
//        // Check if sphere center is within cylinder's top or bottom circles
//        if (glm::length(glm::vec2(position.x, position.y) - glm::vec2(agent->GetCenter().x, agent->GetCenter().y + agent->height / 2)) < agent->radius ||
//            glm::length(glm::vec2(position.x, position.y) - glm::vec2(agent->GetCenter().x, agent->GetCenter().y - agent->height / 2)) < agent->radius) {
//            return true;
//        }
//
//        // If none of the above conditions are true, sphere is not colliding with cylinder
//        return false;
//    }
//    Floor* cube = dynamic_cast<Floor*>(&other);
//    if (cube) {
//        // Find the closest point on the cube to the sphere center
//        glm::vec3 closestPoint
//        (
//            glm::clamp(position.x, cube->position.x - cube->halfsize, cube->position.x + cube->halfsize),
//            glm::clamp(position.y, cube->position.y - cube->halfsize, cube->position.y + cube->halfsize),
//            glm::clamp(position.z, cube->position.z - cube->halfsize, cube->position.z + cube->halfsize)
//        );
//
//        // Calculate distance between sphere center and closest point
//        float dist = glm::distance(position, closestPoint);
//
//        // Compare distance to sphere radius
//        if (dist <= m_Radius) {
//            // Sphere and cube are colliding
//            return true;
//        }
//    }
//
//    return false;
//}
//void Floor::resolveCollision(GameEntity& other) {
//    //Ball* otherSphere = dynamic_cast<Ball*>(&other);
//    //if (otherSphere) {
//    //    // Calculate the collision normal
//    //    glm::vec3 collisionNormal = glm::normalize(otherSphere->position - position);
//
//    //    // Calculate the relative velocity between the two spheres
//    //    glm::vec3 relativeVelocity = otherSphere->velocity - velocity;
//
//    //    // Calculate the impulse scalar
//    //    float impulseScalar = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal) / (1 / mass + 1 / otherSphere->mass);
//
//    //    // Apply the impulse to both spheres
//    //    glm::vec3 impulse = impulseScalar * collisionNormal;
//    //    velocity += impulse / mass;
//    //    otherSphere->velocity -= impulse / otherSphere->mass;
//
//    //    // Update the net force and acceleration of the objects
//    //    force = impulse / DELTA_T;
//    //    accerleration = force / mass;
//    //    otherSphere->force = -impulse / DELTA_T;
//    //    otherSphere->accerleration = otherSphere->force / otherSphere->mass;
//    //}
//    Ball* otherSphere = dynamic_cast<Ball*>(&other);
//    if (otherSphere) {
//    }
//
//    Agent* agent = dynamic_cast<Agent*>(&other);
//    if (agent) {
//    }
//
//    Floor* cube = dynamic_cast<Floor*>(&other);
//    if (cube)
//    {
//        // Find the closest point on the cube to the sphere center
//        glm::vec3 closestPoint(
//            glm::clamp(position.x, cube->position.x - cube->halfsize, cube->position.x + cube->halfsize),
//            glm::clamp(position.y, cube->position.y - cube->halfsize, cube->position.y + cube->halfsize),
//            glm::clamp(position.z, cube->position.z - cube->halfsize, cube->position.z + cube->halfsize)
//        );
//
//        // Calculate the minimum translation distance
//        glm::vec3 distanceVector = position - closestPoint;
//        float distance = glm::length(distanceVector);
//        float overlap = m_Radius - distance;
//        glm::vec3 mtd = distanceVector * (overlap / distance);
//
//        // Update velocities
//        float elasticity = 0.5f;  // Coefficient of restitution
//        glm::vec3 impulse = mtd * (elasticity + 1.0f) * (1.0f / (1.0f / mass));
//        velocity -= impulse / mass;
//
//        // Move spheres out of collision
//        position += mtd;
//        SetPosition(position);
//    }
//
//}