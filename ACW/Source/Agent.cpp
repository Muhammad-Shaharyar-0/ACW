#include "Agent.h"
#include "World.h"
#include <cmath>

void Agent::Update(float pSeconds)
{
    
    StateMachine(pSeconds);
	// Update your Agent here 
	PhysicsUpdate(IntegrationMethod::Euler,pSeconds);
    if (position.y < 0)
    {
    	position.y = 0;
    	velocity.y = 0;
    }


}
void Agent::Stop()
{

    force = glm::vec3(0, 0, 0);
    velocity = glm::vec3(0, 0, 0);
    accerleration = glm::vec3(0, 0, 0);

}
void Agent::StateMachine(float pSeconds)
{
    switch (agent_State)
    {
    case State::Respawning:
    {
        SetPosition(World::_instance->SpawnPosition[(int)team]);

        respawnTime -= pSeconds;
        if (respawnTime <= 0)
        {
            agent_State = State::Idle;
            respawnTime = 5.0;
        }


        break;
    }
    case State::Idle:
    {
        /* ivec3 targetTile;

         AgentStatus status = CalculateStatus(world->GetMatchTime());

         if (status.healthy > status.hurt)
         {
             path = world->ClosestTowerPath(GetPosition(), team);
         }
         else if (status.hurt > status.critical)
         {
             if (status.hurt > status.scoreUrgency)
             {
                 path = world->ClosestPowerUpPath(GetPosition());
             }
             else
             {
                 path = world->ClosestTowerPath(GetPosition(), team);
             }
         }
         else
         {
             if (status.critical > status.scoreUrgency)
             {
                 path = world->ClosestPowerUpPath(GetPosition());
             }
             else
             {
                 path = world->ClosestTowerPath(GetPosition(), team);
             }
         }*/
        path = World::_instance->GetClosestTowerPath(position, team);
        targetType = Target::Tower;
        if (path.size() > 0)
        {
            pathIndex = 0;
            agent_State = State::Moving;
        }
        break;
    }
    case State::Moving:
    {
         if (pathIndex!=path.size())
         {

             glm::vec3 targetPos = path[path.size()-1-pathIndex]->position;
             glm::vec3 moveDir = targetPos - position;
             float distSqr = dot(moveDir, moveDir);
             if (moveDir != glm::vec3(0, 0, 0))
                 moveDir = normalize(moveDir);

          //   Move(world, moveDir, pDt);
             applyForce(moveDir * speed);
             if (distSqr <= 0.50f)
             {
                // Stop();
                 pathIndex++;
             }
         }
         else
         {
             Stop();
             path.clear();
             if (targetType == Target::Tower)
             {
                 agent_State = State::Capturing;
             }         
         }

        break;
    }
    case State::Capturing:
    {
        SearchingTime -= pSeconds;
        Agent* target = nullptr;
        if (SearchingTime <= 0)
        {
            SearchingTime = 2.0f;
            vector<GameEntity*> entities = World::_instance->SearchGrid(GetPosition(), 2,EntityType::All);

            if (entities.size() > 0)
            {
                int index = -1;
                float closestDistSqr = INFINITY;
                for (int i = 0; i < entities.size(); ++i)
                {
                    if (entities[i]->type == EntityType::Agent)
                    {
                        if (entities[i]->team != team)
                        {
                            glm::vec3 dist = position - entities[i]->position;
                            float distSqr = glm::dot(dist, dist);
                            if (distSqr < closestDistSqr)
                            {
                                target = (Agent*)entities[i];
                                closestDistSqr = distSqr;

                            }
                        }
                       
                    }
                    else if (entities[i]->type == EntityType::Tower)
                    {
                        if (((Tower*)entities[i])->IsCaptured(team))
                        {
                            agent_State = State::Idle;
                        }
                    }
                }
            }
        }

        if (target)
        {
            AttackTime -= pSeconds;
            if (AttackTime <= 0)
            {
                AttackTime = 1.5f;
                if (target->TakeDamage(15.0f))
                {
                    target = NULL;
                }
            }
        }

        break;
    }
    }
}
void Agent::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Agent::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

bool Agent::TakeDamage(float damage)
{
    health -= damage;
    if (health <= 0)
    {
        health = initialhealth;
        agent_State = State::Respawning;
        return true;
    }
    return false;
}

void Agent::Move(const float pSeconds)
{
   
}

bool Agent::checkCollision(GameEntity& other) {
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
glm::vec3 Agent::GetPosition() const
{
    glm::mat4 model = GetModel();
    glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
    return translation;
}

void Agent::resolveCollision(GameEntity& other)
{
    Agent* otherCylinder = dynamic_cast<Agent*>(&other);
    if (otherCylinder) {
        // Calculate the collision normal
        glm::vec3 collisionNormal = glm::normalize(otherCylinder->position - position);

        // Calculate the relative velocity between the two cylinders
        glm::vec3 relativeVelocity = otherCylinder->velocity - velocity;

        // Calculate the impulse scalar
        float dot = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal);
        float div = (1 / ((mass + 1) / otherCylinder->mass));
        float impulseScalar = dot / div;

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

glm::vec3 Agent::GetClosestTowerPosition()
{

    return glm::vec3();
}
