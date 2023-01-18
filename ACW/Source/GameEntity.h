#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

class IRenderHelpers;
class IGuiHelpers;


enum class EntityType
{
	Agent,
	Tower,
	Ball,
};

enum class Team
{
	Red,
	Blue,
	Netural,
};

class GameEntity
{
private:

	unsigned int m_ID;
	static unsigned int s_NextID;
	glm::mat4 m_Model;
	glm::vec3 m_Colour;


public:

	EntityType type;
	Team team;
	const float DELTA_T = 0.01; // time step (s)

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f); // velocity of the cylinder
	glm::vec3 accerleration = glm::vec3(0.0f, 0.0f, 0.0f); // acceleration of the cylinder

	glm::vec3 rotation; // rotation of the object around x, y and z axis
	float speed; // speed of the object

	const glm::vec3 GRAVITY_DIRECTION = glm::vec3(0.0f, -1.0f, 0.0f);
	const float GRAVITY_MAGNITUDE = 9.8f;

	float mass = 100; //grams

	float elasticity= 1.0f;

	glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f); // net force acting on the object

	float maxForce = 50;
	float maxvelocity = 100;

	enum class IntegrationMethod {
		RungeKutta,
		SimplifiedEuler,
		Euler
	};

	GameEntity() : m_Colour(1.0f, 1.0f, 1.0f), m_ID(s_NextID)
	{
		s_NextID++;
	}

	unsigned int ID() const { return m_ID; }

	glm::mat4 GetModel() const { return m_Model; }
	glm::mat4& GetModel() { return m_Model; }
	glm::vec3 GetColour() const { return m_Colour; }
	glm::vec3& GetColour() { return m_Colour; }

	void Scale(glm::vec3 pScale)
	{
		m_Model = glm::scale(m_Model, pScale);
	}

	void Translate(glm::vec3 pTranslation)
	{
		m_Model = glm::translate(m_Model, pTranslation);
		position = pTranslation;
	}

	void Rotate(float pRadians, glm::vec3 pAxis)
	{
		m_Model = glm::rotate(m_Model, pRadians, pAxis);
	}

	void moveForward() {
		position += speed * glm::normalize(glm::vec3(cos(rotation.x), sin(rotation.y), sin(rotation.z)));
	}

	void applyForce(glm::vec3 f) 
	{
		force += f;
		//accerleration += (f / mass) * direction;
	}

	void SetPosition(glm::vec3 pPosition)
	{
		glm::mat4& model = GetModel();
		model[3][0] = pPosition.x;
		model[3][1] = pPosition.y;
		model[3][2] = pPosition.z;
		position = pPosition;
	}

	void PhysicsUpdate(IntegrationMethod integrationMethod)
	{
		applyForce(GRAVITY_MAGNITUDE * mass * GRAVITY_DIRECTION);
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

		SetPosition(position);
	}
	virtual void Render(const IRenderHelpers&) const = 0;
	virtual void RenderGui(const IGuiHelpers&);
};
