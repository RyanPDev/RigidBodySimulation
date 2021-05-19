#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <..\RigidBody.h>
#include <..\Box.h>
#include <..\Ball.h>
#include <iostream>

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

Box* box;
Ball* ball;
void PhysicsInit() {
	renderCube = true;
	renderSphere = true;
	checkPotentialCollisions();
	printBoxes();
	glm::vec3 rotationAxis = glm::vec3(0, 1, 0);
	float angle = 3.14f / 4.f; // 45 graus

	/*
	box = new Box(
		glm::vec3(0.f, 5.f, 0.f),
		GetQuaternion(rotationAxis,angle),
		1.f,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		1.f, 2.f, 1.f);

	ball = new Ball(
		glm::vec3(3.f, 5.f, 0.f),
		GetQuaternion(rotationAxis, angle),
		1.f,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		1.f);
	*/
	box = new Box(1.f, 2.f, 1.f,1.f);
	ball = new Ball(1.f, 2.f);
	
}

void PhysicsUpdate(float dt) {
	box->draw();
	ball->draw();
}

void PhysicsCleanup() {
}