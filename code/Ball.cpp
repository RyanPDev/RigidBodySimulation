#include "Ball.h"
namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
}
Ball::Ball(float radius, float mass) : RigidBody(mass), radius(radius) {};

void Ball::draw() {
	Sphere::updateSphere(getState().com, radius);
}

glm::mat3 Ball::getInitialInertiaTensor() {
	// TODO implement
	return glm::mat3(1.f);
}
