#include "World.h"



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
		int x = rand() % (GRID_WIDTH - 3) + 1;
		int z = rand() % (GRID_DEPTH - 3) + 1;
		_agents[i].Translate(glm::vec3(x, 5, z));
		entities.push_back(&_agents[i]);
		_grid[x][z].isWalkable = true;
		_grid[x][z].isOccupied = true;
		_grid[x][z].occupiedBy =&_agents[i];
	}




	for (int i = 0; i < NUMBER_OF_Towers; i++)
	{
		int x = rand() % (GRID_WIDTH - 3) + 1;
		int z = rand() % (GRID_DEPTH - 3) + 1;
		_Towers[i].Translate(glm::vec3(x, -5.0f, z));
		entities.push_back(&_Towers[i]);
		_grid[x][z].isWalkable = false;
		_grid[x][z].isOccupied = true;
		_grid[x][z].occupiedBy = &_Towers[i];
	}
	
	//if (_instance == nullptr) {
	//	_instance = this;
	//}
}


World::~World()
{

}

void World::Update(float pSeconds)
{
	//_agents[0].applyForce(glm::vec3(20.25f, 0, 0));
//	_agents[1].applyForce(glm::vec3(-10.25f, 0, 0));
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{

		
		for (int j = 0; j < NUMBER_OF_AGENTS; j++)
		{
			if (i!=j)
			{
				if (_agents[i].checkCollision(_agents[j]))
				{
					_agents[i].resolveCollision(_agents[j]);
					_agents[j].resolveCollision(_agents[i]);
				}
			}


		}
		_agents[i].Update(pSeconds);

	}
	//for (auto& obj1 : _agents) {
	//	obj1->updateSimulation(IntegrationMethod::Euler);
	//	for (auto& obj2 : _agents) {
	//		if (obj1.get() != obj2.get()) {
	//			if (obj1->checkCollision(*obj2)) {
	//				obj1->resolveCollision(*obj2);
	//				obj2->resolveCollision(*obj1);
	//			}
	//		}
	//	}
	//}
}

void World::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.Render(&_agents[i]);
	}
	for (int i = 0; i < NUMBER_OF_Towers; i++)
	{
		pHelper.Render(&_Towers[i]);
	}

}

void World::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.RenderGui(&_agents[i]);
	}
	for (int i = 0; i < NUMBER_OF_Towers; i++)
	{
		pHelper.RenderGui(&_Towers[i]);
	}
}

bool World::FindTargetAgent(glm::vec3 towerPos, Team allyTeam, glm::vec3& nearest)
{
	//ivec3 towerTile = WorldToGridPos(towerPos);
	bool found = false;
	float distanceSqr = 999999999.9f;
	GameEntity* unit;
	for (int x = -4; x <= 4; x += 1)
	{
		for (int z = -4; z <= 4; z += 1)
		{
			int d = x * x + z * z;
			if (d <= 4 * 4 && d > 2 * 2)
			{
				// Check if within bounds
				if (isWithinBounds(towerPos.x + x, towerPos.z + z))
				{
					//printf("[%i, %i]", xx + x, zz + z);
					unit = _grid[int(towerPos.x + x)][int(towerPos.z + z)].occupiedBy;

					if (unit->type == EntityType::Agent && unit->team != allyTeam)
					{
						glm::vec3 d = unit->position - towerPos;
						float newDistSqr = dot(d, d);
						if (newDistSqr < distanceSqr)
						{
							found = true;
							nearest = unit->position;
							distanceSqr = newDistSqr;
						}
					}
				

				}
			}
		}
	}
	return found;
}

bool World::isWithinBounds(int x, int z)
{
	return (x >= 0 && x < GRID_WIDTH&&
		z >= 0 && z < GRID_DEPTH);
}