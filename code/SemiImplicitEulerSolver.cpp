#include "SemiImplicitEulerSolver.h"

SemiImplicitEulerSolver::SemiImplicitEulerSolver()
{

}

void SemiImplicitEulerSolver::UpdateState(Box* box, glm::vec3 force, glm::vec3 torque, float dt)
{
	RigidBody::State newState = {
		box->getState().com + dt * box->getState().linearMomentum / box->getMass(),
		(0.5f * (glm::inverse(box->getInertiaTensor())) * box->getState().angularMomentum) * glm::normalize(box->getState().rotation),
		box->getState().linearMomentum + dt * force,
		box->getState().angularMomentum + dt * torque
	};
	box->setState(newState);
}