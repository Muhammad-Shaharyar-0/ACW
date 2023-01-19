#include "Camera.h"


void Camera::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Camera::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

void Camera::Update(float pDeltaTime)
{
}
