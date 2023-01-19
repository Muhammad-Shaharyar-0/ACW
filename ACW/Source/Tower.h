#pragma once
#include "Ball.h"
#include "Pipe.h"



class Tower : public Pipe
{
private:

    float shootingDelay;
    float projectileSpeed;

    float influence;

    Team capturingTeam;
    int range;

public:

    Tower();


    void Update(float pDeltaTime);

    void Shoot(float pDeltaTime);


    void InfluenceLogic(float pDeltaTime);

    Ball* ShootProjectile(float dt, glm::vec3 dir);

    bool IsCaptured(Team pTeam);

    bool checkCollision(GameEntity& other) { return false; };

    void resolveCollision(GameEntity& other) {};

};

