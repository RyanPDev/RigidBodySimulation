#pragma once
#include "RigidBody.h"
class Ball : public RigidBody {
public:
	Ball(float radius, float mass);
	virtual void draw() override;
protected:
	virtual glm::mat3 getInitialInertiaTensor() override;
private:
	float radius;
};
