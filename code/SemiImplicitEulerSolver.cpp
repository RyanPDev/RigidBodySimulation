#include "SemiImplicitEulerSolver.h"

SemiImplicitEulerSolver::SemiImplicitEulerSolver()
{

}

void SemiImplicitEulerSolver::UpdateState(Box* box, glm::vec3 force, glm::vec3 torque, float dt)
{
	isValidState = true;
	RigidBody::State newState;
	float lastdt = 0;
	int counter = 0;
	glm::vec3 impulse = glm::vec3(0, 0, 0);
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
						float relativeSpeed = glm::dot(boxPlanes.norms[p], (velocity + glm::cross(angularVelocity, (auxWpos - newState.com))));

						if (relativeSpeed < 0)
						{
							isValidState = true;
							impulse = CalculateImpulse(boxPlanes.norms[p], velocity, angularVelocity, newState.com, box, auxWpos, relativeSpeed);
							newState.linearMomentum += impulse;
							newState.angularMomentum += glm::cross((auxWpos - newState.com), impulse);
							velocity = newState.linearMomentum / box->getMass();
							angularVelocity = glm::inverse(box->getInertiaTensor()) * newState.angularMomentum;
						}

						newState.com += 0.001f * boxPlanes.norms[p];
					}
				}
				impulse = glm::vec3(0, 0, 0);
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
			dt = (dt - lastdt) * 0.5f;
			lastdt = aux;
		}
		counter++;
	} while (!isValidState && counter <= 16);

	if (counter > 16)
	{
		newState.com += 0.01f * glm::normalize(glm::vec3(0, 5, 0) - newState.com);
	}

	box->setState(newState);
}

float SemiImplicitEulerSolver::GetDistanceFromPlane(int plane, glm::vec3 pos)
{
	return (glm::abs((boxPlanes.norms[plane].x * pos.x) + (boxPlanes.norms[plane].y * pos.y) + (boxPlanes.norms[plane].z * pos.z) + boxPlanes.d[plane])) /
		(glm::sqrt(glm::pow(boxPlanes.norms[plane].x, 2) + glm::pow(boxPlanes.norms[plane].y, 2) + glm::pow(boxPlanes.norms[plane].z, 2)));
}

glm::vec3 SemiImplicitEulerSolver::CalculateImpulse(glm::vec3 normal, glm::vec3 velocity, glm::vec3 angularVelocity, glm::vec3 _com, Box* box, glm::vec3 vertexPos, float relativeVel)
{
	glm::vec3 ra = (vertexPos - _com);

	return (-(1 + E) * relativeVel) / ((1 / box->getMass()) + glm::dot(glm::cross(glm::inverse(box->getInertiaTensor()) * glm::cross(ra, normal), ra), normal)) * normal;
}