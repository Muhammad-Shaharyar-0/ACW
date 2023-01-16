#include "Agent.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"


void Agent::Update(float pSeconds)
{
	// Update your Agent here 
}

void Agent::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Agent::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}