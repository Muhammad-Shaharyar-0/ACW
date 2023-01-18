#include "Tower.h"



Tower::Tower()
{
    type = EntityType::Tower;
    team = Team::Netural;
    m_BaseHeight = 3.0f;
    shootingDelay = 1.0f;
    projectileSpeed = 15.0f;
    influence = 0.0f;
}

void Tower::Update(float pDeltaTime)
{
    //shootingDelay -= pDeltaTime;

    ////return;
    //if (shootingDelay <= 0)
    //{
    //    glm::vec3 outPos;
    //  
    //    if (world->FindTargetAgent(position, team, outPos))
    //    {
    //       
    //    //    printf("Found target %.2f %.2f\n", outPos.x, outPos.z);
    //    //    glm::vec3 towerPeak = position;
    //    //    towerPeak.y += m_BaseHeight + m_LipHeight;

    //    //    glm::vec3 dir = glm::normalize(outPos - towerPeak);

    //    //    Ball ball = ShootProjectile(pDeltaTime, dir);
    //    ////    World::instance->AddBall(ball);

    //    }
    //    shootingDelay = 1.0f;
    //}
}

Ball Tower::ShootProjectile(float dt, glm::vec3 dir)
{
    Ball ball;
    glm::vec3 ballPos = position;
    ballPos.y += m_BaseHeight + m_LipHeight;

    ball.SetPosition(ballPos);
    applyForce(dir * projectileSpeed);

	return ball;
}
