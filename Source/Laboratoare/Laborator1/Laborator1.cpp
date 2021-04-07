#include "Laborator1.h"

#include <iostream>
#include <cmath>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		changeInPosition = glm::vec3(0, 0, 0);
		
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
		
		mesh = new Mesh("teapot");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;

		mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	if (backgroundSecondColor)
		glClearColor(0.5, 0, 0, 1);
	else
		glClearColor(0, 0, 0, 1);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
	
	if (change) {
		framesToChange = std::ceil(1 / deltaTimeSeconds);
		change = false;
	}

	framesToChange--;
	
	if (framesToChange == 0)
	{
		switch (id)
		{
		case 1: id = 2;
			change = true;
			break;
		case 2: id = 3;
			change = true;
			break;
		case 3: id = 1;
			break;
			
		default: break;
		}
	}

	Mesh* mesh;
	switch (id)
	{
	case 1: mesh = meshes["box"];
		break;
	case 2: mesh = meshes["sphere"];
		break;
	case 3: mesh = meshes["teapot"];
		break;
	default: mesh = meshes["box"];
		break;
	}

	rotation = 2.0f * glm::vec3(std::cos(angle) - 1, std::sin(angle), 0);
	
	// render the object
	RenderMesh(mesh, glm::vec3(1, 0.5f, 0) + changeInPosition + rotation, glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(mesh, glm::vec3(-1, 0.5f, 0) + changeInPosition);

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W))
		changeInPosition += glm::vec3(0, 1 * deltaTime, 0);
	
	if (window->KeyHold(GLFW_KEY_S))
		changeInPosition += glm::vec3(0, -1 * deltaTime, 0);
	
	if (window->KeyHold(GLFW_KEY_D))
		changeInPosition += glm::vec3( 1 * deltaTime, 0, 0);
	
	if (window->KeyHold(GLFW_KEY_A))
		changeInPosition += glm::vec3( -1 * deltaTime, 0, 0);
	
	if (window->KeyHold(GLFW_KEY_Q))
		changeInPosition += glm::vec3(0, 0, 1 * deltaTime);
	
	if (window->KeyHold(GLFW_KEY_E))
		changeInPosition += glm::vec3(0, 0, -1 * deltaTime);


	if (window->KeyHold(GLFW_KEY_R))
		angle += deltaTime;

	if (window->KeyHold(GLFW_KEY_T))
		angle -= deltaTime;
};

void Laborator1::OnKeyPress(const int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		backgroundSecondColor = !backgroundSecondColor;
	}

	if (key == GLFW_KEY_K) {
		change = true;
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
