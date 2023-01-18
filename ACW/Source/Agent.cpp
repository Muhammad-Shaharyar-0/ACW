#include "Agent.h"



void Agent::Update(float pSeconds)
{
	// Update your Agent here 
	PhysicsUpdate(IntegrationMethod::Euler);
	force = glm::vec3(0, 0, 0);
	//velocity = glm::vec3(0, velocity.y, 0);


}

void Agent::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Agent::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}