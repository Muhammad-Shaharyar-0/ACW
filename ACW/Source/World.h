#pragma once
#include <vector>
#include "GameEntity.h"
#include "Agent.h"
#include "Tower.h"

using namespace std;

struct cell
{
	bool isWalkable = true;
	bool isOccupied = false;
	GameEntity* occupiedBy = nullptr;
};

class World : public GameEntity
{
public:
	const static int GRID_WIDTH = 30;
	const static int GRID_DEPTH = 30;

private:
	const static int NUMBER_OF_AGENTS = 4;
	const static int NUMBER_OF_Towers = 4;
	int _gridHeights[GRID_WIDTH][GRID_DEPTH];
	glm::vec3 _gridColours[GRID_WIDTH][GRID_DEPTH];

	cell _grid[GRID_WIDTH][GRID_DEPTH];

	Agent _agents[NUMBER_OF_AGENTS];

	Tower _Towers[NUMBER_OF_Towers];

	vector<GameEntity*> entities;

	//static World* _instance;

public:



	//static World* _instance;







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

	void Update(float pSeconds);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
	bool FindTargetAgent(glm::vec3 towerPos, Team allyTeam, glm::vec3& nearest);
	bool isWithinBounds(int x, int z);
};



//World* World::_instance = nullptr;