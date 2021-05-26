#pragma once
#include <..\Box.h>
#include <iostream>

class SemiImplicitEulerSolver
{
public:
	float E = 0.5f;
	bool useCollisions = true;

private:
	bool isValidState = true;
	bool hasCollided = false;
	float threshold = 0.1f;

public:
	SemiImplicitEulerSolver();
	void UpdateState(Box*, glm::vec3, glm::vec3,float);
	float GetDistanceFromPlane(int plane, glm::vec3 pos);
	glm::vec3 CalculateImpulse(glm::vec3, glm::vec3, glm::vec3, glm::vec3, Box*, glm::vec3, float);
protected:
	struct WorldBounds
	{
		glm::vec3 pos[2] = { glm::vec3(-5.f, 0.f, -5.f), glm::vec3{5.f, 10.f, 5.f} };
		glm::vec3 norms[6] = {
			glm::vec3(0.f,  1.f,  0.f),
			glm::vec3(1.f,  0.f,  0.f),
			glm::vec3(0.f,  0.f,  1.f),
			glm::vec3(0.f, -1.f,  0.f),
			glm::vec3(-1.f,  0.f,  0.f),
			glm::vec3(0.f,  0.f, -1.f)
		};
		float d[6] = { 0.f, 5.f, 5.f, 10.f, 5.f, 5.f };
	}boxPlanes;
};