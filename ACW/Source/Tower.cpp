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
    vector<GameEntity*> agents = World::_instance->SearchGrid(position, 2, EntityType::Agent);

    float teaminfluence[2] = { 0, 0 };

    if (agents.size() > 0)
    {
        for (int i = 0; i < agents.size(); ++i)
        {
            teaminfluence[(int)agents[i]->team] += pDeltaTime * 2.5f;
        }
    }

    float diffinInfluence = teaminfluence[(int)Team::Red] - teaminfluence[(int)Team::Blue];

    if (capturingTeam == Team::Red)
    {
        influence += diffinInfluence;
    }
    else if (capturingTeam == Team::Blue)
    {
        influence -= diffinInfluence;
    }
    else
    {
        capturingTeam = diffinInfluence > 0 ? Team::Red : Team::Blue;
    }

    if (influence < 0)
    {
        if (capturingTeam == Team::Blue)
        {
            capturingTeam = Team::Red;
        }
          
        else if (capturingTeam == Team::Red)
        {
            capturingTeam = Team::Blue;
        }
           

        influence = -influence;
    }


    if (influence >= 5)
    {
        team = capturingTeam;
        World::_instance->AddScore(team, (pDeltaTime * influence) / 2.0f);
        if (influence > 10.0f)
            influence = 10.0f;
    }
    else
    {
        team = Team::Netural;
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

bool Tower::IsCaptured(Team t)
{
    return (influence >= 10 && t == team);
}