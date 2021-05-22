#include "SemiImplicitEulerSolver.h"

SemiImplicitEulerSolver::SemiImplicitEulerSolver()
{

}

void SemiImplicitEulerSolver::UpdateState(Box* box, glm::vec3 force, glm::vec3 torque, float dt)
{
	Box::State newState = {
		box->getState().com + dt * box->getState().linearMomentum / box->getMass(),
		(0.5f * box->getInertiaTensor()) * box->getRotationMatrix(),
		box->getState().linearMomentum + dt * force,
		box->getState().angularMomentum + dt * torque
	};
	box->setState(newState);

	/*x->getState().linearMomentum = box->getState().linearMomentum + dt * force;
	box->getState().angularMomentum = box->getState().angularMomentum + dt * torque;
	box->getState().com = box->getState().com + dt * box->getState().linearMomentum / box->getMass();
	box->getState().rotation  = (0.5f * box->getInertiaTensor()) *  box->getRotationMatrix();*/
}