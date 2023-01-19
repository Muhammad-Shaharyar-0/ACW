#include "World.h"
#include<cmath>
World* World::_instance = nullptr;

World::World()
{


	glm::vec3 postion(0, 0, 0);


	for (size_t i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_DEPTH; j++)
		{
			int x = rand() % (2 - 0) + 1;
			_grid[i][j].SetPosition(glm::vec3(i, 0, j));
			_grid[i][j].difficulty = x;
			AddEntity(&_grid[i][j]);
			_gridColours[i][j] = glm::vec3(x, 0, 1);
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



	SpawnPosition[0] = glm::vec3(3, 0, 15);
	SpawnPosition[1] = glm::vec3(27, 0, 15);
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		int x = rand() % (50 - 25) + 1;
		int z = rand() % (500 - 100) + 1;
		_agents[i].mass += x;
		_agents[i].speed += z;
		if (i < NUMBER_OF_AGENTS / 2)
		{
		
			_agents[i].Translate(SpawnPosition[0]);
			_agents[i].team = Team::Red;
			_agents[i].GetColour() = glm::vec3(1, 0, 0);
		}
		else
		{
			_agents[i].Translate(SpawnPosition[1]);
			_agents[i].team = Team::Blue;
			_agents[i].GetColour() = glm::vec3(0, 0, 1);
		}
		entities.push_back(&_agents[i]);

	}



	_Towers[0].Translate(glm::vec3(15, 0.0f, 5));
	entities.push_back(&_Towers[0]);
	_grid[15][15].isWalkable = false;
	_grid[15][15].isOccupied = true;
	_grid[15][15].occupiedBy = &_Towers[0];
	_Towers[1].Translate(glm::vec3(15, 0.0f, 25));
	entities.push_back(&_Towers[1]);
	_grid[15][15].isWalkable = false;
	_grid[15][15].isOccupied = true;
	_grid[15][15].occupiedBy = &_Towers[1];
	
	if (_instance == nullptr) {
		_instance = this;
	}
}


World::~World()
{

}

void World::Update(float pSeconds)
{
	matchTime += pSeconds;

	GameEntity* itemToRemove = nullptr;
	for (int i = 0; i < entities.size(); i++)
	{

		Ball* ball = dynamic_cast<Ball*>(entities[i]);
		if (ball)
		{
			for (int j = 0; j < entities.size(); j++)
			{
				if (i != j)
				{
					if (entities[i]->checkCollision(*entities[j]))
					{
						Agent* agent = dynamic_cast<Agent*>(entities[j]);
						if (agent)
						{
							itemToRemove = entities[i];
						}
						entities[i]->resolveCollision(*entities[j]);
					//	entities[j]->resolveCollision(*entities[i]);
					}
				}


			}
		}
		Floor* cube = dynamic_cast<Floor*>(entities[i]);
		if (cube == nullptr)
			entities[i]->Update(pSeconds);

		
	}
	RemoveEntity(itemToRemove);

}

void World::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->Render(pHelper);
	}


}

void World::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);

	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->RenderGui(pHelper);
	}

}

void World::AddEntity(GameEntity* item)
{
	entities.push_back(item);
}

void World::RemoveEntity(GameEntity* item)
{
	entities.erase(std::remove(entities.begin(), entities.end(), item), entities.end());
	delete item;
}


bool World::FindTargetAgent(glm::vec3 towerPos, int range, Team team,glm::vec3& nearest)
{
	
	float closestDistSqr = INFINITY;
	bool isFound = false;
	for (int i = 0; i < NUMBER_OF_AGENTS; ++i)
	{
		if (_agents[i].team != team)
		{

			glm::vec3 dist = towerPos - _agents[i].position;
			float distSqr = glm::dot(dist, dist);
			if (distSqr < closestDistSqr)
			{
				if (distSqr <= range*range)
				{
					nearest = _agents[i].position;
					closestDistSqr = distSqr;
					isFound = true;
				}
			
			}
		}
	}
	return isFound;
}

bool World::GetClosestTowerPosition(glm::vec3 start, Team team,glm::vec3& closestPos)
{
	
	float closestDistSqr = INFINITY;
	bool isFound = false;
	for (int i = 0; i < NUMBER_OF_Towers; ++i)
	{
		if (_Towers[i].team != team || _Towers[i].team==Team::Netural)
		{
			glm::vec3 dist = start - _Towers[i].position;
			float distSqr = glm::dot(dist, dist);
			if (distSqr < closestDistSqr)
			{
				closestPos = _Towers[i].position;
				closestDistSqr = distSqr;
				isFound = true;
			}
		}
	}
	if (isFound)
	{
		if (start.x > closestPos.x)
		{
			closestPos.x += 1;
		}
		else if (start.x < closestPos.x)
		{
			closestPos.x -= 1;
		}

		if (start.z > closestPos.z)
		{
			closestPos.z += 1;
		}
		else if (start.z < closestPos.z)
		{
			closestPos.z -= 1;
		}
		return true;
	}
		
	return false;
}

vector<Floor*> World::GetClosestTowerPath(glm::vec3 start, Team team)
{
	glm::vec3 targetposition;
	if (GetClosestTowerPosition(start, team, targetposition))
	{
	  vector<Floor*> path = PathFinding(start, targetposition);
	  return path;
	}
	return vector<Floor*>();
}





template <typename T>
bool IsInVector(const std::vector<T>& vector, const T& element) {
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}

vector<Floor*> World::PathFinding(glm::vec3 start, glm::vec3 end)
{

	vector<Floor*> openList;
	vector<Floor*> closedList;


	_grid[(int)start.x][(int)start.z].gCost = 0;
	_grid[(int)start.x][(int)start.z].hCost = abs(end.x - start.x) + abs(end.z - start.z);
	_grid[(int)start.x][(int)start.z].parent = nullptr;
	openList.push_back(&_grid[(int)start.x][(int)start.z]);

	while (!openList.empty()) {
	
		int lowestFCostIndex = 0;
		for (int i = 0; i < openList.size(); i++) {
			if (openList[i]->getCost() < openList[lowestFCostIndex]->getCost()) {
				lowestFCostIndex = i;
			}
		}

		
		Floor* currentFloor = openList[lowestFCostIndex];
		openList.erase(openList.begin() + lowestFCostIndex);
		closedList.push_back(currentFloor);

	
		if (currentFloor == &_grid[(int)end.x][(int)end.z]) {
	
			vector<Floor*> path;
			Floor* current = currentFloor;
			while (current != nullptr) {
				path.push_back(current);
				current = current->parent;
			}
			return path;
		}

		for (int x = -1; x <= 1; x++) {
			for (int z = -1; z <= 1; z++) {
		
				if (!isWithinBounds(currentFloor->position.x + x, currentFloor->position.z + z) || !_grid[(int)currentFloor->position.x + x][(int)currentFloor->position.z + z].isWalkable) {
					continue;
				}
			
				Floor* neighborFloor = &_grid[(int)currentFloor->position.x + x][(int)currentFloor->position.z + z];
				if (IsInVector(closedList, neighborFloor)) {
					continue;
				}

		
				float gCost = currentFloor->gCost + 1;
				float hCost = abs(end.x - (currentFloor->position.x + x)) + abs(end.z - (currentFloor->position.z + z));

			
				bool isInOpenList = false;
				for (int i = 0; i < openList.size(); i++) {
					if (openList[i] == neighborFloor) {
						isInOpenList = true;
				
						if (gCost < neighborFloor->gCost) {
							neighborFloor->gCost = gCost;
							neighborFloor->parent = currentFloor;
						}
						break;
					}
				}


				if (!isInOpenList) {
					neighborFloor->gCost = gCost;
					neighborFloor->hCost = hCost;
					neighborFloor->parent = currentFloor;
					openList.push_back(neighborFloor);
				}
			}
		}
	}

	return vector<Floor*>();

}

vector<GameEntity*> World::SearchGrid(glm::vec3 startpos, int range, EntityType type)
{
	
	vector<GameEntity*> items;

	for (int i = 0; i < entities.size(); ++i)
	{
		if (entities[i]->type == type || type==EntityType::All)
		{
			glm::vec3 dist = startpos - entities[i]->position;
			float distSqr = glm::dot(dist, dist);
			if (distSqr <= range * range)
			{
				items.push_back(entities[i]);
			}
		}
		
	}
	return items;
}

bool World::isWithinBounds(int x, int z)
{
	return (x >= 0 && x < GRID_WIDTH&&
		z >= 0 && z < GRID_DEPTH);
}

void World::AddScore(Team team, float score)
{
	TeamScores[(int)team] += score;
}