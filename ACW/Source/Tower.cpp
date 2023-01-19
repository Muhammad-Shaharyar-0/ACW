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
   

    Shoot(pDeltaTime);
    InfluenceLogic(pDeltaTime);
 
  

   

}
void Tower::Shoot(float pDeltaTime)
{
    shootingDelay -= pDeltaTime;
    if (shootingDelay <= 0)
    {
        glm::vec3 outPos;

        if (World::_instance->FindTargetAgent(position, range, team, outPos))
        {

            glm::vec3 topofTower = position;
            topofTower.y += m_BaseHeight + m_LipHeight;
            outPos.y += 1;
            glm::vec3 dir = glm::normalize(outPos - topofTower);

            Ball* ball = ShootProjectile(pDeltaTime, dir);
            World::_instance->AddEntity(ball);
            shootingDelay = 2.0f;

        }
    }
}

void Tower::InfluenceLogic(float pDeltaTime)
{
    float teaminfluence[2]
    {
        0,
        0
    };

    vector<GameEntity*> agents = World::_instance->SearchGrid(position, 2, EntityType::Agent);
    if (agents.size() > 0)
    {
        for (int i = 0; i < agents.size(); ++i)
        {
            teaminfluence[(int)agents[i]->team] += pDeltaTime * 1.5f;
        }
    }

    float diffinInfluence = teaminfluence[(int)Team::Blue] - teaminfluence[(int)Team::Red];

    if (capturingTeam == Team::Blue)
    {
        influence += diffinInfluence;
    }
    else if (capturingTeam == Team::Red)
    {
        influence -= diffinInfluence;
    }
    else
    {
        if (diffinInfluence > 0)
        {
            capturingTeam = Team::Blue;
        }
        else
        {
            capturingTeam = Team::Red;

        }
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