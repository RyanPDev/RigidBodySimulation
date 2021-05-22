#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include "..\RigidBody.h"
#include "..\Ball.h"
#include <iostream>
#include "..\SemiImplicitEulerSolver.h"

#pragma region forwardDeclarations

extern bool renderCube;
extern bool renderSphere;
#pragma endregion

#pragma region utils
glm::quat GetQuaternion(glm::vec3 axis, float angle)
{
	glm::quat q = glm::quat(0.f, 0.f, 0.f, 0.f);
	return glm::normalize(q);
}
#pragma endregion

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate

	}
	
	ImGui::End();
}

struct BoundingBox {
	int id;
	float x1; // x1 < x2
	float x2;
};

enum IntervalType {
	BEGIN, END
};

struct IntervalLimit {
	IntervalType type;
	int box;
	float* value; // <-- pointer so when we change the box we have the value updated
};
BoundingBox boxes[] = {
	 BoundingBox{0, 0.0f, 1.0f},
	 BoundingBox{1, 0.8f, 1.3f},
	 BoundingBox{2, 1.2f, 2.0f},
	 BoundingBox{3, 3.0f, 3.7f},
	 BoundingBox{4, 3.5f, 4.0f}
};
void printBoxes() {
	for (int i = 0; i < 5; i++) {
		std::cout << "B" << i << ": ";
		for (int j = 0; j < 50; j++) {
			float pos = j / 10.f;
			if (pos < boxes[i].x1 || pos > boxes[i].x2) {
				std::cout << " ";
			}
			else {
				std::cout << "#";
			}
		}
		std::cout << std::endl;
	}
}

void initIntervals(IntervalLimit intervalsLimits[]) {
	for (int i = 0; i < 5; i++)
	{
		intervalsLimits[2*i] = IntervalLimit{ BEGIN, i, &boxes[i].x1 };
		intervalsLimits[2 * i + 1] = IntervalLimit{ END, i, &boxes[i].x2 };
	}
}

void sort(IntervalLimit list[], int n)
{
	//insertion sort
}

void sweep(IntervalLimit list[], int n)
{
	bool active[5] = { false, false, false, false, false };
	// activar caixa 3 --> active[3] = true
	// implementar sweep
	// print collisions
}

void checkPotentialCollisions() {
	IntervalLimit intervalsLimits[2 *5];
	initIntervals(intervalsLimits);
	// sort
	sort(intervalsLimits, 10);
	// sweep
	sweep(intervalsLimits, 10);
}



namespace Gravity {

	SemiImplicitEulerSolver solver;
	Box* box;
	
	const float G = 0.001f;

	glm::quat getRotationQuaternion(glm::vec3 axis, float angle) {
		float w = cos(angle / 2);
		glm::vec3 v = sin(angle / 2) * axis;
		return glm::normalize(glm::quat(w, v));
	}

	glm::vec3 getGravityForce(RigidBody* r1) {
		glm::vec3 direction = glm::vec3(0,-9.81,0);
		float distance = glm::length(direction);
		float magnitude = G * r1->getMass() / distance;
		return glm::normalize(direction) * magnitude;
	}

	void init() {
		box = new Box(1.f, 1.f, 1.f, 1.f);

		glm::vec3 boxCom = glm::vec3(0.0f, 5.0f, 0.0f);
		glm::vec3 boxLinearVelocity = glm::vec3(0.f, 5.f, 0.f);

		box->initializeState(
			boxCom,
			getRotationQuaternion(glm::vec3(1.f, 1.f, 0.f), 3.14f / 2.f),
			boxLinearVelocity,
			glm::vec3(1.0f, 0.0f, 0.0f) // angular velocity
		);

		renderCube = true;
	}

	void update(float dt) {
	//glm::vec3 force = getGravityForce(box);
		glm::vec3 force = glm::vec3(0,-1.f,0);
		glm::vec3 torques = glm::vec3(20.f);

		solver.UpdateState(box, force, torques, dt);
		box->commitState();
		//solver.updateState(ball, -force, torques, dt);

		box->draw();
		//ball->draw();
	}

	void cleanup() {
		delete box;
	}
}

void PhysicsInit() {
	renderCube = true;
	
	checkPotentialCollisions();
	printBoxes();
	glm::vec3 rotationAxis = glm::vec3(0, 1, 0);
	float angle = glm::pi<float>() / 4.f; // 45 graus
	Gravity::init();
	//box = new Box(1.f, 2.f, 1.f,1.f);
}

void PhysicsUpdate(float dt) {
	Gravity::update(dt);
}

void PhysicsCleanup() {
}