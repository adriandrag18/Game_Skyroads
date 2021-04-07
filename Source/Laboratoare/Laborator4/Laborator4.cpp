#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	
	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	jumpX = 0;
	jumpY = 0;
	flip = 0;
	up = true;
	start = false;

	sunMovementX = 0;
	sunMovementY = 0;
	orbit = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// bonus
	if (start) {
		if (jumpY > height)
			up = false;
		else if (jumpY < 0)
			up = true;

		if (up)
			jumpY += deltaTimeSeconds * jumpSpeed;
		else
			jumpY -= deltaTimeSeconds * jumpSpeed;

		jumpX += deltaTimeSeconds;
		flip -= deltaTimeSeconds;
	}
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, 0, 0);
	modelMatrix *= Transform3D::Translate(jumpX, jumpY, 0);
	modelMatrix *= Transform3D::RotateOZ(flip);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// bonus 2

	if (start) {
		sunMovementY += deltaTimeSeconds;
		sunMovementX += deltaTimeSeconds;
		orbit += deltaTimeSeconds;
	}
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0, -3.5f);
	modelMatrix *= Transform3D::Translate(sunMovementX, sunMovementY, 0);
	modelMatrix *= Transform3D::Scale(2, 2, 2);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(4.0f, 0, -3.5f);
	modelMatrix *= Transform3D::Translate(sunMovementX, sunMovementY, 0);
	modelMatrix *= Transform3D::Translate(-4, 0, 0);
	modelMatrix *= Transform3D::RotateOY(orbit);
	modelMatrix *= Transform3D::Translate(4, 0, 0);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(5.0f, 0, -3.5f);
	modelMatrix *= Transform3D::Translate(sunMovementX, sunMovementY, 0);
	modelMatrix *= Transform3D::Translate(-5, 0, 0);
	modelMatrix *= Transform3D::RotateOY(orbit);
	modelMatrix *= Transform3D::Translate(5, 0, 0);
	modelMatrix *= Transform3D::Translate(-1, 0, 0);
	modelMatrix *= Transform3D::RotateOY(2 * orbit);
	modelMatrix *= Transform3D::Translate(1, 0, 0);
	modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W))
		translateY += deltaTime * translationSpeed;
	
	if (window->KeyHold(GLFW_KEY_S))
		translateY -= deltaTime * translationSpeed;

	if (window->KeyHold(GLFW_KEY_D))
		translateX += deltaTime * translationSpeed;
	
	if (window->KeyHold(GLFW_KEY_A))
		translateX -= deltaTime * translationSpeed;

	if (window->KeyHold(GLFW_KEY_R))
		translateZ += deltaTime * translationSpeed;

	if (window->KeyHold(GLFW_KEY_F))
		translateZ -= deltaTime * translationSpeed;


	if (window->KeyHold(GLFW_KEY_1)) {
		scaleX += deltaTime * scaleSpeed;
		scaleY += deltaTime * scaleSpeed;
		scaleZ += deltaTime * scaleSpeed;
	}

	if (window->KeyHold(GLFW_KEY_2)) {
		scaleX -= deltaTime * scaleSpeed;
		scaleY -= deltaTime * scaleSpeed;
		scaleZ -= deltaTime * scaleSpeed;
	}

	if (window->KeyHold(GLFW_KEY_3))
		angularStepOX += deltaTime * rotationSpeed;

	if (window->KeyHold(GLFW_KEY_4))
		angularStepOX -= deltaTime * rotationSpeed;

	if (window->KeyHold(GLFW_KEY_5))
		angularStepOY += deltaTime * rotationSpeed;

	if (window->KeyHold(GLFW_KEY_6))
		angularStepOY -= deltaTime * rotationSpeed;

	if (window->KeyHold(GLFW_KEY_7))
		angularStepOZ += deltaTime * rotationSpeed;

	if (window->KeyHold(GLFW_KEY_8))
		angularStepOZ -= deltaTime * rotationSpeed;
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
	
	if (key == GLFW_KEY_ENTER)
		start = !start;
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
