#include "World.h"

World* World::_instance = nullptr;

World::World()
{


	glm::vec3 postion(0, 0, 0);

	//for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	//{
	//	_agents[i].Translate(postion);
	//	postion.x += 2;

	//	if (i % 2 == 0)
	//	{
	//		_agents[i].GetColour() = glm::vec3(1, 0, 0);
	//	}
	//	else
	//	{
	//		_agents[i].GetColour() = glm::vec3(0, 0, 1);
	//	}

	//	
	//}
	for (size_t i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_DEPTH; j++)
		{
			int x = rand() % (3 - 0) + 1;
			_grid[i][j].SetPosition(glm::vec3(i, 0, j));
			_grid[i][j].difficulty = x;
			AddEntity(&_grid[i][j]);
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

	SpawnPosition[0] = glm::vec3(3, 0, 15);
	SpawnPosition[1] = glm::vec3(27, 0, 4);
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
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

		//_grid[x][z].isWalkable = true;
		//_grid[x][z].isOccupied = true;
		//_grid[x][z].occupiedBy =&_agents[i];
	}




	for (int i = 0; i < NUMBER_OF_Towers; i++)
	{
	/*	int x = rand() % (GRID_WIDTH - 3) + 1;
		int z = rand() % (GRID_DEPTH - 3) + 1;
		while (_grid[x][z].isOccupied == true)
		{
			 x = rand() % (GRID_WIDTH - 3) + 1;
			 z = rand() % (GRID_DEPTH - 3) + 1;
		}*/
		_Towers[i].Translate(glm::vec3(15, 0.0f, 15));
		entities.push_back(&_Towers[i]);
		_grid[15][15].isWalkable = false;
		_grid[15][15].isOccupied = true;
		_grid[15][15].occupiedBy = &_Towers[i];
	}
	
	if (_instance == nullptr) {
		_instance = this;
	}
}


World::~World()
{

}

void World::Update(float pSeconds)
{
	//_agents[0].applyForce(glm::vec3(20.25f, 0, 0));
//	_agents[1].applyForce(glm::vec3(-10.25f, 0, 0));
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
						entities[i]->resolveCollision(*entities[j]);
						entities[j]->resolveCollision(*entities[i]);
					}
				}


			}
		}
		Floor* cube = dynamic_cast<Floor*>(entities[i]);
		if (cube == nullptr)
			entities[i]->Update(pSeconds);
	}
	//for (size_t i = 0; i < entities.size(); i++)
	//{
	//	
	//}
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
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->Render(pHelper);
	}
	//for (size_t i = 0; i < GRID_WIDTH; i++)
	//{
	//	for (int j = 0; j < GRID_DEPTH; j++)
	//	{
	//		_grid[i][j].Render(pHelper);
	//	}
	//}
	//for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	//{
	//	pHelper.Render(&_agents[i]);
	//}
	//for (int i = 0; i < NUMBER_OF_Towers; i++)
	//{
	//	pHelper.Render(&_Towers[i]);
	//}

}

void World::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);

	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->RenderGui(pHelper);
	}
	//for (size_t i = 0; i < GRID_WIDTH; i++)
	//{
	//	for (int j = 0; j < GRID_DEPTH; j++)
	//	{
	//		_grid[i][j].RenderGui(pHelper);
	//	}
	//}
}

void World::AddEntity(GameEntity* item)
{
	entities.push_back(item);
}

void World::RemoveEntity(GameEntity* item)
{
	entities.erase(std::remove(entities.begin(), entities.end(), item), entities.end());

}


bool World::FindTargetAgent(glm::vec3 towerPos, int range, Team team,glm::vec3& nearest)
{
	////ivec3 towerTile = WorldToGridPos(towerPos);
	//bool found = false;
	//float distanceSqr = 999999999.9f;
	//GameEntity* unit;
	//for (int x = -range; x <= range; x += 1)
	//{
	//	for (int z = -range; z <= range; z += 1)
	//	{
	//		// Check if within bounds
	//		if (isWithinBounds(towerPos.x + x, towerPos.z + z))
	//		{
	//			//printf("[%i, %i]", xx + x, zz + z);
	//			unit = _grid[int(towerPos.x + x)][int(towerPos.z + z)].occupiedBy;
	//			if (unit)
	//			{
	//				if (unit->type == EntityType::Agent && unit->team != team)
	//				{
	//					glm::vec3 d = unit->position - towerPos;
	//					float newDistSqr = dot(d, d);
	//					if (newDistSqr < distanceSqr)
	//					{
	//						found = true;
	//						nearest = unit->position;
	//						distanceSqr = newDistSqr;
	//					}
	//				}
	//			}

	//		}
	//	}
	//}
	//return found;
	float closestDistSqr = 99999999.9;
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
	
	float closestDistSqr = 99999999.9;
	bool isFound = false;
	for (int i = 0; i < NUMBER_OF_Towers; ++i)
	{
		if (_Towers[i].team != team)
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
		//// Offset by 1 tile so we are not at center of tower
		//vec3 pos = towers[towerId]->GetPosition();
		//out = WorldToGridPos(pos);

		if (start.x > closestPos.x)
		{
			closestPos.x += 1;
		}
		else if (start.x < closestPos.x)
		{
			closestPos.x -= 1;
		}

		if (start.y > closestPos.y)
		{
			closestPos.y += 1;
		}
		else if (start.y < closestPos.y)
		{
			closestPos.y -= 1;
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

int ManhattanDistance(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

void RetracePath(Floor* startFloor, Floor* endFloor, std::vector<Floor*>& path) {
	Floor* currentFloor = endFloor;
	while (currentFloor != startFloor) {
		path.push_back(currentFloor);
		currentFloor = currentFloor->parent;
	}
	path.push_back(startFloor);
	std::reverse(path.begin(), path.end());
}

class FloorComparator {
public:
	bool operator() (const Floor* lhs, const Floor* rhs) const {
		return (lhs->getCost()) > (rhs->getCost());
	}
};

template <typename T>
bool IsInVector(const std::vector<T>& vector, const T& element) {
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}

vector<Floor*> World::PathFinding(glm::vec3 start, glm::vec3 end)
{
	// Initialize open and closed lists
	vector<Floor*> openList;
	vector<Floor*> closedList;

	// Add starting point to open list
	_grid[(int)start.x][(int)start.z].gCost = 0;
	_grid[(int)start.x][(int)start.z].hCost = abs(end.x - start.x) + abs(end.z - start.z);
	_grid[(int)start.x][(int)start.z].parent = nullptr;
	openList.push_back(&_grid[(int)start.x][(int)start.z]);

	// While open list is not empty
	while (!openList.empty()) {
		// Find cell with lowest fCost in open list
		int lowestFCostIndex = 0;
		for (int i = 0; i < openList.size(); i++) {
			if (openList[i]->getCost() < openList[lowestFCostIndex]->getCost()) {
				lowestFCostIndex = i;
			}
		}

		// Move current cell to closed list and remove from open list
		Floor* currentFloor = openList[lowestFCostIndex];
		openList.erase(openList.begin() + lowestFCostIndex);
		closedList.push_back(currentFloor);

		// Check if current cell is the end point
		if (currentFloor == &_grid[(int)end.x][(int)end.z]) {
			// Retrace path and return it
			vector<Floor*> path;
			Floor* current = currentFloor;
			while (current != nullptr) {
				path.push_back(current);
				current = current->parent;
			}
			return path;
		}

		// Check neighboring cells
		for (int x = -1; x <= 1; x++) {
			for (int z = -1; z <= 1; z++) {
				// Check if neighbor is out of bounds or not walkable
				if (!isWithinBounds(currentFloor->position.x + x, currentFloor->position.z + z) || !_grid[(int)currentFloor->position.x + x][(int)currentFloor->position.z + z].isWalkable) {
					continue;
				}
				// Check if neighbor is already in closed list
				Floor* neighborFloor = &_grid[(int)currentFloor->position.x + x][(int)currentFloor->position.z + z];
				if (IsInVector(closedList, neighborFloor)) {
					continue;
				}

				// Calculate gCost and hCost for neighbor
				float gCost = currentFloor->gCost + 1;
				float hCost = abs(end.x - (currentFloor->position.x + x)) + abs(end.z - (currentFloor->position.z + z));

				// Check if neighbor is already in open list
				bool isInOpenList = false;
				for (int i = 0; i < openList.size(); i++) {
					if (openList[i] == neighborFloor) {
						isInOpenList = true;
						// Check if new gCost is lower than previous gCost
						if (gCost < neighborFloor->gCost) {
							neighborFloor->gCost = gCost;
							neighborFloor->parent = currentFloor;
						}
						break;
					}
				}

				// If neighbor is not in open list, add it
				if (!isInOpenList) {
					neighborFloor->gCost = gCost;
					neighborFloor->hCost = hCost;
					neighborFloor->parent = currentFloor;
					openList.push_back(neighborFloor);
				}
			}
		}
	}

	// If end point is not reached or found, return empty path
	return vector<Floor*>();

}

bool World::isWithinBounds(int x, int z)
{
	return (x >= 0 && x < GRID_WIDTH&&
		z >= 0 && z < GRID_DEPTH);
}