#pragma once
#include "GameEntity.h"
#include "Agent.h"

class World :
	public GameEntity
{
	public:
		const static int GRID_WIDTH = 25;
		const static int GRID_DEPTH = 20;

	private:
		const static int NUMBER_OF_AGENTS = 4;

		int _gridHeights[GRID_WIDTH][GRID_DEPTH];
		glm::vec3 _gridColours[GRID_WIDTH][GRID_DEPTH];

		Agent _agents[NUMBER_OF_AGENTS];
	public:

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
};

