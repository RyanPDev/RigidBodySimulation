#include "SemiImplicitEulerSolver.h"

SemiImplicitEulerSolver::SemiImplicitEulerSolver()
{

}

void SemiImplicitEulerSolver::UpdateState(Box* box, glm::vec3 force, glm::vec3 torque, float dt)
{
	// Linear Momentum P(t+dt)
	glm::vec3 newLinearMomentum = box->getState().linearMomentum + dt * force;
	// Angular Momentum L(t+dt)
	glm::vec3 newAngularMomentum = box->getState().angularMomentum + dt * torque;
	// velocity v(t+dt)
	glm::vec3 velocity = newLinearMomentum / box->getMass();
	// Position x(t+dt)
	glm::vec3 newPosition = box->getState().com + dt * velocity;
	// new Inertia tensor I(t)-1 
	//	GetInertiaTensor()
	// Angular velocity w(t)
	glm::vec3 angularVelocity = glm::inverse(box->getInertiaTensor()) * newAngularMomentum;
	// Rotation R(t+dt)
	glm::quat newRotation = box->getState().rotation + dt * (0.5f * glm::quat(0.f,angularVelocity) * box->getState().rotation);

	RigidBody::State newState = { 
			newPosition,
			glm::normalize(newRotation),
			newLinearMomentum,
			newAngularMomentum
	};
	box->setState(newState);
}
/*
RigidBody::State newState = {
		box->getState().com + dt * box->getState().linearMomentum / box->getMass(),
		(0.5f * (glm::inverse(box->getInertiaTensor())) * box->getState().angularMomentum) * glm::normalize(box->getState().rotation),
		box->getState().linearMomentum + dt * force,
		box->getState().angularMomentum + dt * torque
	};
*/