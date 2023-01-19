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
    float health = 90;
    float initialhealth = health;
  
    State agent_State= State::Idle;

    float respawnTime;
    float AttackTime;
    float SearchingTime;

    vector<Floor*> path;
    int pathIndex=0;
    Target targetType=Target::None;

    

public:
    float radius = 0.5;
    float height = 2; 
    float speed = 1500;
    glm::vec3 cylinderAxis = glm::normalize(glm::vec3(0, height, 0));
    Agent()
    {
        type = EntityType::Agent;
        respawnTime = 5.0;
        mass = 500;
        SearchingTime=2.0f;
        AttackTime = 1.5f;
    }
	void Update(const float pSeconds);
    void Stop();
    void StateMachine(float pSeconds);
    void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
    bool TakeDamage(float damage);
 


    bool checkCollision(GameEntity& other);

    glm::vec3 GetPosition() const;

    void resolveCollision(GameEntity& other);

    glm::vec3 GetClosestTowerPosition();

    glm::vec3 GetCenter()
    {
        return glm::vec3(position.x, position.y + radius, position.z);
    }

};


