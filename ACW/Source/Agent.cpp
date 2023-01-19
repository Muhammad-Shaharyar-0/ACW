#include "Agent.h"
#include "World.h"


void Agent::Update(float pSeconds)
{
    
    StateMachine(pSeconds);
	// Update your Agent here 
	PhysicsUpdate(IntegrationMethod::Euler,pSeconds);



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
             agent_State = State::Idle;
           
         }
   /*      else
         {
             if (path.type == PathType::Tower)
             {
                 printf("At tower state\n");
                 state = AgentState::AtTower;
             }
             else
             {
                 printf("Picked up powerup, old health %.2f", health);
                 TakeDamage(-world->PickupPowerUp(path.entityIndex));
                 printf(", new health %.2f\n", health);
                 state = AgentState::Idle;
             }
         }*/
        break;
    }
    //case AgentState::AtTower:
    //{
    //    searchTime -= pDt;
    //    if (searchTime <= 0)
    //    {
    //        searchTime = 3.0f;
    //        SearchResult search = world->SearchArea(GetPosition(), 2);

    //        if (search.size() > 0)
    //        {
    //            int enemyAgentIdx = -1;
    //            float distSqr = 999999999.9f;
    //            for (int i = 0; i < search.size(); ++i)
    //            {
    //                if (search[i]->type == EntityType::Agent)
    //                {
    //                    if (search[i]->team != team)
    //                    {
    //                        float distSqr2 = DistanceSquared(search[i]);
    //                        if (distSqr2 < distSqr)
    //                        {
    //                            enemyAgentIdx = i;
    //                            distSqr = distSqr2;
    //                        }
    //                    }
    //                }
    //                else if (search[i]->type == EntityType::Tower)
    //                {
    //                    if (((Tower*)search[i])->IsCaptured(team))
    //                    {
    //                        printf("Returning idle\n");
    //                        state = AgentState::Idle;
    //                    }
    //                }
    //            }
    //            // Found agent;
    //            if (enemyAgentIdx != -1)
    //                targetAgent = (Agent*)search[enemyAgentIdx];
    //            else
    //                targetAgent = NULL;
    //        }
    //    }

    //    if (targetAgent)
    //    {
    //        attackTime -= pDt;
    //        if (attackTime <= 0)
    //        {
    //            attackTime = 1.0f;
    //            bool targetDead = targetAgent->TakeDamage(15.0f);
    //            if (targetDead)
    //            {
    //                targetAgent = NULL;
    //            }
    //        }
    //    }

    //    break;
    //}
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
