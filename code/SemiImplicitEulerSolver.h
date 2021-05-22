#pragma once
#include <..\Box.h>
 

class SemiImplicitEulerSolver
{
private:
	//glm::vec3 position, velocity;
	//glm::vec3 linearMomentum, angularMomentum;

public:
	SemiImplicitEulerSolver();
	void UpdateState(Box*, glm::vec3, glm::vec3,float);
};