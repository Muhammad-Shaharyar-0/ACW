#pragma once
#include "Ball.h"
#include "Pipe.h"



class Tower : public Pipe
{
private:

    float shootingDelay;
    float projectileSpeed;

    float influence;
    int range;

public:

    Tower();


    void Update(float pDeltaTime);

    //void DestroyProjectile(Ball* projectile);

    Ball* ShootProjectile(float dt, glm::vec3 dir);

    bool checkCollision(GameEntity& other) { return false; };

    void resolveCollision(GameEntity& other) {};

};

