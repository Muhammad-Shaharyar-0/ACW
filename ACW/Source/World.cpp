#include "World.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

World::World()
{
	glm::vec3 postion(0, 0, 0);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		_agents[i].Translate(postion);
		postion.x += 2;

		if (i % 2 == 0)
		{
			_agents[i].GetColour() = glm::vec3(1, 0, 0);
		}
		else
		{
			_agents[i].GetColour() = glm::vec3(0, 0, 1);
		}
	}

	for (int i = 0; i < GRID_WIDTH; i++)
	{
		_gridHeights[i][0] = 3;
		_gridHeights[i][GRID_DEPTH - 1] = 3;
	}

	for (int i = 0; i < GRID_DEPTH; i++)
	{
		_gridHeights[0][i] = 3;
		_gridHeights[GRID_WIDTH - 1][i] = 3;
	}

	glm::vec3 colours[3] = { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) };

	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_DEPTH; j++)
		{
			_gridColours[i][j] = colours[rand() % 3];
		}
	}

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		float x = rand() % (GRID_WIDTH - 3) + 1;
		float z = rand() % (GRID_DEPTH - 3) + 1;
		_agents[i].Translate(glm::vec3(x, 0, z));
	}
}

World::~World()
{

}

void World::Update(float pSeconds)
{
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		_agents[i].Update(pSeconds);
	}
}

void World::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.Render(&_agents[i]);
	}
}

void World::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.RenderGui(&_agents[i]);
	}
}