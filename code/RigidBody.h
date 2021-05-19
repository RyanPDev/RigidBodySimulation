#pragma once
#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>
#pragma once
class RigidBody {
public:
	struct State {
		glm::vec3 com;  // Position of the Cenrer Of Mass
		glm::quat rotation;  // Quaternion that represents the current rotation q(t)
		glm::vec3 linearMomentum;  // P(t)
		glm::vec3 angularMomentum;  // L(t)
	};

	RigidBody(float mass);
	void initializeState(glm::vec3 initialPosition, glm::quat initialRotation, glm::vec3 linearSpeed, glm::vec3 angularSpeed);

	State getState();
	void setState(State state);
	State rollbackState();
	void commitState();

	float getMass();
	glm::mat3 getInertiaTensor();

	virtual void draw() = 0;
protected:
	glm::mat3 getRotationMatrix();
	virtual glm::mat3 getInitialInertiaTensor() = 0;
private:
	float mass;
	glm::mat3 initialInertiaTensor;
	State stableState;
	State state;
};

