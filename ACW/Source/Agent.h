#pragma once
#include <vector>
#include "GameEntity.h"
#include "Floor.h"

enum class State
{
    Idle,
    Moving,
    Capturing,
    Attacking,
    Respawning,
};

enum class Target
{
    Tower,
    PowerUp,
    None,
};

using namespace std;
class Agent :
    public GameEntity
{
private:
    float health = 10;
    float initialhealth = health;
  
    State agent_State= State::Idle;

    float respawnTime;
    
    vector<Floor*> path;
    int pathIndex=0;
    Target targetType=Target::None;

    float speed=500;

public:
    float radius = 0.5; // radius of the cylinder
    float height = 1; // height of the cylinder
    Agent()
    {
        type = EntityType::Agent;
        respawnTime = 5.0;
    }
	void Update(const float pSeconds);
    void Stop();
    void StateMachine(float pSeconds);
    void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
    bool TakeDamage(float damage);
    void Move(const float pSeconds);


    bool checkCollision(GameEntity& other);

    glm::vec3 GetPosition() const;

    void resolveCollision(GameEntity& other);

    glm::vec3 GetClosestTowerPosition();

    glm::vec3 GetCenter()
    {
        return glm::vec3(position.x, position.y + radius, position.z);
    }

};


