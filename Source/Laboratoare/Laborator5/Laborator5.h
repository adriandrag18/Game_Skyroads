#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"

class Laborator5 : public SimpleScene
{
	public:
		Laborator5();
		~Laborator5();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
		float fov = 2 * M_PI / 3;
		float orthoLeft = -10.0f, orthoRight = 10.0f, orthoBottom = -5.0f, orthoTop = 5.0f, orthoNear = 0.1f, orthoFar = 200;
		int activeO = 0;
};
