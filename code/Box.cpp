#include "..\Box.h"
namespace Cube {
	extern void updateCube(const glm::mat4& transform);
}
Box::Box(float width, float height, float depth, float mass)
	: RigidBody(mass), width(width), height(height), depth(depth) {};

void Box::draw() {
	RigidBody::State state = getState();
	glm::mat4 transform = glm::translate(glm::mat4(1.f), state.com) *
		glm::mat4_cast(state.rotation) *
		glm::scale(glm::mat4(1.f), glm::vec3(width, height, depth));
	Cube::updateCube(transform);
}

glm::mat3 Box::getInitialInertiaTensor() {
	// TODO implement
	return glm::mat3(
		mass / 12.f * (pow(height, 2.f) + pow(depth, 2.f)), 0, 0,

		0, mass / 12.f * (pow(width, 2.f) + pow(depth, 2.f)), 0,

		0, 0, mass / 12.f * (pow(width, 2.f) + pow(height, 2.f))
	);
}