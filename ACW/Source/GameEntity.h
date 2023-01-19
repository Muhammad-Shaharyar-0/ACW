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
	Floor,
	All,
};

enum class Team
{
	Red=0,
	Blue=1,
	Netural=2,
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
	float dragCoefficient=0.3;

	float elasticity= 0.1f;

	glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f); // net force acting on the object

	float maxForce = 50;
	const float maxvelocity = 50;

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


	void SetPosition(glm::vec3 pPosition)
	{
		glm::mat4& model = GetModel();
		model[3][0] = pPosition.x;
		model[3][1] = pPosition.y;
		model[3][2] = pPosition.z;
		position = pPosition;
	}
	virtual bool checkCollision(GameEntity& other) =0;

	virtual void resolveCollision(GameEntity& other) = 0;

	virtual void Render(const IRenderHelpers&) const = 0;
	virtual void RenderGui(const IGuiHelpers&);
	void moveForward();
	void applyForce(glm::vec3 f);
	void PhysicsUpdate(IntegrationMethod integrationMethod, float pDeltaTime);

	virtual void Update(float pDeltaTime)=0;
};
