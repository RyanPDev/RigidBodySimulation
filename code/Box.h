#pragma once
#include "RigidBody.h"

class Box : public RigidBody {
public:
	Box(float width, float height, float depth, float mass);
	virtual void draw() override;
	glm::vec3 GetVertexWorldPos(int);
protected:
	virtual glm::mat3 getInitialInertiaTensor() override;

private:
	float width, height, depth;
	glm::vec3 verticesLocalPos[8];
};

