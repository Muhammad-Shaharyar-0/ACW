#pragma once
#include "GameEntity.h"
#include "Ball.h"
#include "Agent.h"
class Floor : public GameEntity
{


public:


        Floor* parent;
        bool isWalkable = true;
        bool isOccupied = false;
        GameEntity* occupiedBy = nullptr;
        float gCost;
        float hCost;
        float difficulty=0;
        float halfsize=0.5f;
        Floor();
        void Render(const IRenderHelpers&) const;
        void RenderGui(const IGuiHelpers&);
        void Update(float pDeltaTime);
        int getCost() const;
        bool checkCollision(GameEntity& other) { return false; };

        void resolveCollision(GameEntity& other) {};
};

