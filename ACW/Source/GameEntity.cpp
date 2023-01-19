#include "GameEntity.h"


unsigned int GameEntity::s_NextID = 0;

void GameEntity::RenderGui(const IGuiHelpers& pGuiHelper)
{
	pGuiHelper.RenderGui(this);
}

void GameEntity::applyForce(glm::vec3 f)
{
	force += f;
	//accerleration += (f / mass) * direction;
}
void GameEntity::PhysicsUpdate(float pDeltaTime)
{
	applyForce(GRAVITY_MAGNITUDE * mass * GRAVITY_DIRECTION);
	glm::vec3 currentAcceleration = accerleration + force / mass;
	if (intergeration.x == 0)
	{
		integrationMethod = IntegrationMethod::RungeKutta;
	}
	else if (intergeration.x == 1)
	{
		integrationMethod = IntegrationMethod::Euler;
	}
	if (intergeration.x == 2)
	{
		integrationMethod = IntegrationMethod::SimplifiedEuler;
	}
	switch (integrationMethod) 
	{
	case IntegrationMethod::RungeKutta: 
	{
	
		glm::vec3 k1 = currentAcceleration * pDeltaTime;
		glm::vec3 k2 = (currentAcceleration + k1 / 2.0f) * pDeltaTime;
		glm::vec3 k3 = (currentAcceleration + k2 / 2.0f) * pDeltaTime;
		glm::vec3 k4 = (currentAcceleration + k3) * pDeltaTime;
		velocity += (k1 + 2.0f * k2 + 2.0f * k3 + k4) / 6.0f;
		break;
	}
	case IntegrationMethod::Euler: {

		velocity += currentAcceleration * pDeltaTime;
		break;
	}
	case IntegrationMethod::SimplifiedEuler: {

		position += velocity * pDeltaTime + 0.5f * currentAcceleration * pDeltaTime * pDeltaTime;
		velocity += currentAcceleration * pDeltaTime;
		break;
	}
	}

	velocity *= pow(dragCoefficient, pDeltaTime);

	velocity = glm::clamp(velocity, -maxvelocity, maxvelocity);

	position += velocity * pDeltaTime;

	SetPosition(position);

	force = glm::vec3(0.0f);
	accerleration = glm::vec3(0.0f);
}