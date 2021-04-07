#pragma once

#include <Component/SimpleScene.h>

#include "Balloon.h"
#include "Arrow.h"
#include "Player.h"
#include "Shuriken.h"
#include "PowerBar.h"

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void NextArrow();
	void CheckArrows();
	void CheckBalloons();
	void VerifyCollisionArrow();
	void VerifyCollisionShuriken();
	void ModifyScore(int points);
	void LevelUp();
	void AddBalloon();
	void RemoveBalloon();
	void AddShuriken();
	void RemoveShuriken();
	void PowerUpLevel();
	void BlackArrowLevel();
	void PauseGame();

protected:
	int numberBalloons = 2;
	int numberShuriken = 1;
	int numberArrows = 1;
	int score = 0;
	int lives = 3;
	int level = 0;
	float time = 0;
	float powerUp = 0;
	float nextShotTime;
	float gameSpeed = 100;
	bool mustRemoveBalloon = false;
	bool mustRemoveShuriken = false;
	bool pause = false;
	glm::vec3 arrowColor;
	Player* player;
	std::vector<Arrow*> arrows = std::vector<Arrow*>();
	std::vector<Balloon*> balloons = std::vector<Balloon*>();
	std::vector<Shuriken*> shurikens = std::vector<Shuriken*>();
	PowerBar* powerBar;
	const std::vector<std::string> balloon_components = {
		"balloon_body_", "balloon_triangle_", "balloon_string_"
	};
	const std::vector<std::string> arrow_components = {"arrow_tip_", "arrow_body_"};
};
