#include "Floor.h"


Floor::Floor()
{	
	glm::vec3& colour = GetColour();
	colour.x = 0.6f;
	colour.y = 0.3f;
	colour.z = 0.0f;
	type = EntityType::Floor;
	gCost = 0;
	hCost = 0;
	isWalkable = true;
	isOccupied = false;
	occupiedBy = nullptr;
	mass = 500;
}

void Floor::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Floor::RenderGui(const IGuiHelpers& pHelper)
{
//	pHelper.RenderGui(this);
}

void Floor::Update(float pDeltaTime)
{
}

int Floor::getCost() const
{
	return hCost+gCost+difficulty;
}