#include "..\Box.h"

namespace Cube {
	extern void updateCube(const glm::mat4& transform);
}

Box::Box(float width, float height, float depth, float mass)
	: RigidBody(mass), width(width), height(height), depth(depth)
{
	verticesLocalPos[0] = { -width * 0.5f, -height * 0.5f, -depth * 0.5f };  // Izquierda Abajo Detras
	verticesLocalPos[1] = { -width * 0.5f, height * 0.5f, -depth * 0.5f };	// Izquierda Arriba Detras
	verticesLocalPos[2] = { width * 0.5f, -height * 0.5f, -depth * 0.5f };	// Derecha Abajo Detras
	verticesLocalPos[3] = { width * 0.5f, height * 0.5f, -depth * 0.5f };	// Derecha Abajo Arriba
	verticesLocalPos[4] = { -width * 0.5f, -height * 0.5f, depth * 0.5f };	// Izquierda Abajo Delante 
	verticesLocalPos[5] = { -width * 0.5f, height * 0.5f, depth * 0.5f };	// Izquierda Arriba Delante
	verticesLocalPos[6] = { width * 0.5f, -height * 0.5f, depth * 0.5f };	// Derecha Abajo Delante
	verticesLocalPos[7] = { width * 0.5f, height * 0.5f, depth * 0.5f };	// Derecha Arriba Delante
};

void Box::draw() {
	RigidBody::State state = getState();
	glm::mat4 transform =
		glm::translate(glm::mat4(1.f), state.com) *
		glm::mat4_cast(state.rotation) *
		glm::scale(glm::mat4(1.f), glm::vec3(width, height, depth));
	Cube::updateCube(transform);
}

glm::vec3 Box::GetVertexWorldPos(int idx, glm::vec3 _com, glm::quat _rotation) // Obtiene el vertice en coordenadas de mundo
{
	return glm::mat3_cast(_rotation) * verticesLocalPos[idx] + _com;
}

glm::mat3 Box::getInitialInertiaTensor() // Obtiene el tensor de inercia inicial
{
	return glm::mat3(
		mass / 12.f * (pow(height, 2.f) + pow(depth, 2.f)), 0, 0,

		0, mass / 12.f * (pow(width, 2.f) + pow(depth, 2.f)), 0,

		0, 0, mass / 12.f * (pow(width, 2.f) + pow(height, 2.f))
	);
}