#pragma once
#include <algorithm>
#include <vector>
#include <queue>
#include "GameEntity.h"
#include "Agent.h"
#include "Tower.h"
#include "Floor.h"

using namespace std;



class World : public GameEntity
{
public:
	const static int GRID_WIDTH = 30;
	const static int GRID_DEPTH = 30;

private:
	const static int NUMBER_OF_AGENTS = 4;
	const static int NUMBER_OF_Towers = 2;
	int _gridHeights[GRID_WIDTH][GRID_DEPTH];
	glm::vec3 _gridColours[GRID_WIDTH][GRID_DEPTH];

	Floor _grid[GRID_WIDTH][GRID_DEPTH];

	Agent _agents[NUMBER_OF_AGENTS];

	Tower _Towers[NUMBER_OF_Towers];

	vector<GameEntity*> entities;





public:

	glm::vec3 SpawnPosition[2];

	static World* _instance;

	float TeamScores[2];

	float matchTime=180;
	float currentTime;




	World();
	~World();

	int GetGridHeight(int pX, int pZ) const
	{
		return _gridHeights[pX][pZ];
	}

	glm::vec3 GetGridColour(int pX, int pZ) const
	{
		return _gridColours[pX][pZ];
	}

	bool isGameOver()
	{
		return currentTime >= matchTime;
	}
	void Update(float pSeconds);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
	void AddEntity(GameEntity* item);
	void RemoveEntity(GameEntity* item);
	vector<Floor*> PathFinding(glm::vec3 start, glm::vec3 end);
	vector<GameEntity*> SearchGrid(glm::vec3 startpos, int range, EntityType type);
	bool isWithinBounds(int x, int z);
	void AddScore(Team team, float score);
	bool FindTargetAgent(glm::vec3 towerPos, int range, Team team, glm::vec3& nearest);
	bool GetClosestTowerPosition(glm::vec3 start, Team team, glm::vec3& closestPos);
	vector<Floor*> GetClosestTowerPath(glm::vec3 start,Team team);
	bool checkCollision(GameEntity& other) { return false; };

	void resolveCollision(GameEntity& other) {};
};



