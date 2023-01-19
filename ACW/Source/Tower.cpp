#include "Tower.h"
#include "World.h"


Tower::Tower()
{
    type = EntityType::Tower;
    team = Team::Netural;
    m_BaseHeight = 3.0f;
    shootingDelay = 0.0f;
    projectileSpeed = 20000.0f;
    influence = 0.0f;
    range = 4;
}

void Tower::Update(float pDeltaTime)
{
    shootingDelay -= pDeltaTime;

    //return;
    if (shootingDelay <= 0)
    {
        glm::vec3 outPos;
      
        if (World::_instance->FindTargetAgent(position, range , team, outPos))
        {
           
            printf("Found target %.2f %.2f\n", outPos.x, outPos.z);
            glm::vec3 towerPeak = position;
            towerPeak.y += m_BaseHeight + m_LipHeight;
            outPos.y += 1;
            glm::vec3 dir = glm::normalize(outPos - towerPeak);

            Ball* ball =ShootProjectile(pDeltaTime, dir);
            World::_instance->AddEntity(ball);
            shootingDelay = 2.0f;

        }
    }
}

Ball* Tower::ShootProjectile(float dt, glm::vec3 dir)
{
    Ball* ball=new Ball();
    glm::vec3 ballPos = position;
    ballPos.y += m_BaseHeight + m_LipHeight+0.5f;

    ball->SetPosition(ballPos);
    ball->applyForce(dir * projectileSpeed);

	return ball;
}
