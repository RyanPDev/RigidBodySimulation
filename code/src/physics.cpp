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

#pragma region UI variables

bool playSimulation = false;
bool useCollisions = true;
float TIMER{ 15 };
float resetTimer = 0;

#pragma endregion

namespace Simulation {
	SemiImplicitEulerSolver solver;
	Box* box;

	glm::vec3 force = glm::vec3(0, 0, 0);
	glm::vec3 torques = glm::vec3(0, 0, 0);
	float forceMagnitud = 100;
	const glm::vec3 gravity = glm::vec3(0, -9.81f, 0);

	glm::quat getRotationQuaternion(glm::vec3 axis, float angle) {
		float w = cos(angle / 2);
		glm::vec3 v = sin(angle / 2) * axis;
		return glm::normalize(glm::quat(w, v));
	}

	glm::vec3 getTorqueAtPoint(glm::vec3 force, glm::vec3 point)
	{
		return glm::cross(point, force);
	}

	void AddRandomForceAndTorque()
	{
		force = glm::normalize(glm::vec3((2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))) - 1.f, 1.f, (2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))) - 1.f));
		force *= forceMagnitud;
		glm::vec3 point = glm::vec3(0.5f, 0.f, 0.5f);
		torques = getTorqueAtPoint(force, point);
	}
	void ResetSimulation()
	{
		resetTimer = 0;

		glm::vec3 boxCom = glm::vec3(-4 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4 - (-4)))), 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (8 - 1))), -4 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4 - (-4)))));
		glm::vec3 boxLinearSpeed = glm::vec3((2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))) - 1.f, 8.f, (2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))) - 1.f);
		glm::vec3 boxAngularSpeed = glm::vec3(0.f, 0.f, 0.f);

		box->initializeState(
			boxCom,
			getRotationQuaternion(glm::vec3((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),(static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))), ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)))* 3.14f / 2.f),
			boxLinearSpeed,
			boxAngularSpeed // angular velocity
		);

		AddRandomForceAndTorque();
	}
	void init() {
		srand(static_cast<unsigned>(time(nullptr)));
		box = new Box(1.f, 1.f, 1.f, 1.f);

		ResetSimulation();

		renderCube = true;
	}

	void update(float dt) {
		if (playSimulation)
		{
			resetTimer += dt;
			solver.UpdateState(box, gravity, torques, dt);
			torques = glm::vec3(0, 0, 0);
			if (resetTimer >= TIMER) ResetSimulation();
		}

		box->draw();
	}

	void cleanup() {
		delete box;
	}
}

void PhysicsInit() {

	renderCube = true;
	Simulation::init();
}

void PhysicsUpdate(float dt) {
	Simulation::update(dt);
}

void PhysicsCleanup() {
	Simulation::cleanup();
}

void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate	

	ImGui::Checkbox("Play simulation", &playSimulation);

	if (ImGui::Button("Reset simulation")) Simulation::ResetSimulation();

	ImGui::DragFloat("Reset time", &TIMER, 0.05f, 1.f, 15.f);

	ImGui::DragFloat3("Gravity", (float*)&Simulation::gravity, 0.05f, -9.8f, 9.8f);

	if (ImGui::CollapsingHeader("Collision"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Use collisions", &useCollisions);

		ImGui::DragFloat("Elastic coefficient", &Simulation::solver.E, 0.01f, 0.f, 1.f);

		ImGui::Unindent();
	}

	ImGui::End();

	bool show_test_window = false;
	if (show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}