#include "GameEntity.h"


unsigned int GameEntity::s_NextID = 0;

void GameEntity::RenderGui(const IGuiHelpers& pGuiHelper)
{
	pGuiHelper.RenderGui(this);
}

void GameEntity::moveForward() {
	position += speed * glm::normalize(glm::vec3(cos(rotation.x), sin(rotation.y), sin(rotation.z)));
}

void GameEntity::applyForce(glm::vec3 f)
{
	force += f;
	//accerleration += (f / mass) * direction;
}
void GameEntity::PhysicsUpdate(IntegrationMethod integrationMethod, float pDeltaTime)
{
	/*applyForce(GRAVITY_MAGNITUDE * mass * GRAVITY_DIRECTION);
	accerleration = force / mass;
	switch (integrationMethod)
	{
	case IntegrationMethod::RungeKutta:
	{
		glm::vec3 k1 = velocity;
		glm::vec3 k2 = velocity + accerleration * DELTA_T / 2.0f;
		glm::vec3 k3 = velocity + accerleration * DELTA_T / 2.0f;
		glm::vec3 k4 = velocity + accerleration * DELTA_T;

		position += (k1 + 2.0f * k2 + 2.0f * k3 + k4) * DELTA_T / 6.0f;
		velocity += accerleration * DELTA_T;
		break;
	}
	case IntegrationMethod::SimplifiedEuler: {
		position += velocity * DELTA_T;
		velocity += accerleration * DELTA_T;
		break;
	}
	case IntegrationMethod::Euler:
	{
		velocity += accerleration * DELTA_T;
		position += velocity * DELTA_T;
		break;
	}
	}
	if (position.y < 0)
	{
		position.y = 0;
		velocity.y = 0;
		accerleration.y = 0;
	}

	SetPosition(position);*/
	// Store the current acceleration for later use
	applyForce(GRAVITY_MAGNITUDE * mass * GRAVITY_DIRECTION);
	glm::vec3 currentAcceleration = accerleration + force / mass;
	switch (integrationMethod) {
	case IntegrationMethod::RungeKutta: {
		// Runge-Kutta
		glm::vec3 k1 = currentAcceleration * pDeltaTime;
		glm::vec3 k2 = (currentAcceleration + k1 / 2.0f) * pDeltaTime;
		glm::vec3 k3 = (currentAcceleration + k2 / 2.0f) * pDeltaTime;
		glm::vec3 k4 = (currentAcceleration + k3) * pDeltaTime;
		velocity += (k1 + 2.0f * k2 + 2.0f * k3 + k4) / 6.0f;
		break;
	}
	case IntegrationMethod::Euler: {
		// Euler
		velocity += currentAcceleration * pDeltaTime;
		break;
	}
	case IntegrationMethod::SimplifiedEuler: {
		// Simplified Euler
		position += velocity * pDeltaTime + 0.5f * currentAcceleration * pDeltaTime * pDeltaTime;
		velocity += currentAcceleration * pDeltaTime;
		break;
	}
	}
	// Apply drag force
	velocity *= pow(dragCoefficient, pDeltaTime);
	// Limit the velocity
	velocity = glm::clamp(velocity, -maxvelocity, maxvelocity);
	// Update the position
	position += velocity * pDeltaTime;
	//if (position.y < 0)
	//{
	//	position.y = 0;
	//	velocity.y = 0;
	//}
	SetPosition(position);
	// Clear the force
	force = glm::vec3(0.0f);
	accerleration = glm::vec3(0.0f);
}