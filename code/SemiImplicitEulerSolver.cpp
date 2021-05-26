#include "SemiImplicitEulerSolver.h"

SemiImplicitEulerSolver::SemiImplicitEulerSolver()
{

}

void SemiImplicitEulerSolver::UpdateState(Box* box, glm::vec3 force, glm::vec3 torque, float dt)
{
	isValidState = true;
	RigidBody::State newState;
	float lastdt = 0;
	do
	{
		hasCollided = false;
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
		glm::quat newRotation = box->getState().rotation + dt * (0.5f * glm::quat(0.f, angularVelocity) * box->getState().rotation);

		newState = RigidBody::State{
				newPosition,
				glm::normalize(newRotation),
				newLinearMomentum,
				newAngularMomentum
		};

		for (int p = 0; p < 6; p++)
		{
			for (int v = 0; v < 8; v++)
			{
				// Hay vertices dentro del plano?
				if ((glm::dot(boxPlanes.norms[p], box->GetVertexWorldPos(v, newState.com, newState.rotation)) + boxPlanes.d[p]) *
					(glm::dot(boxPlanes.norms[p], box->GetVertexWorldPos(v, box->getState().com, box->getState().rotation)) + boxPlanes.d[p]) <= 0)
				{
					hasCollided = true;
				}
			}
			if (!hasCollided)
			{
				for (int v = 0; v < 8; v++)
				{
					glm::vec3 auxWpos = box->GetVertexWorldPos(v, newState.com, newState.rotation);
					if (GetDistanceFromPlane(p, auxWpos) <= threshold)
					{
						std::cout << "Vertex: " << auxWpos.x << "," << auxWpos.y << "," << auxWpos.z << " With plane :" << p << ", " << dt << std::endl;
						isValidState = true;
					}
				}
			}
		}
		if (hasCollided)
		{
			dt = (lastdt + dt) * 0.5f;
			isValidState = false;
		}
		else if (!isValidState)
		{
			float aux = dt;
			(dt -= lastdt) * 0.5f;
			lastdt = aux;
		}
	} while (!isValidState);

	box->setState(newState);
}

float SemiImplicitEulerSolver::GetDistanceFromPlane(int plane, glm::vec3 pos)
{
	return (glm::abs((boxPlanes.norms[plane].x * pos.x) + (boxPlanes.norms[plane].y * pos.y) + (boxPlanes.norms[plane].z * pos.z) + boxPlanes.d[plane])) /
		(glm::sqrt(glm::pow(boxPlanes.norms[plane].x, 2) + glm::pow(boxPlanes.norms[plane].y, 2) + glm::pow(boxPlanes.norms[plane].z, 2)));
}
float SemiImplicitEulerSolver::GetDirectionFromPlane(int plane, glm::vec3 pos)
{
	return 0;
}