#include "GameEntity.h"


unsigned int GameEntity::s_NextID = 0;

void GameEntity::RenderGui(const IGuiHelpers& pGuiHelper)
{
	pGuiHelper.RenderGui(this);
}