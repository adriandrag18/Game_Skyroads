#include "Tema1.h"
#include <iostream>
#include <Core/Engine.h>

using namespace std;

const glm::vec3 red = glm::vec3(1, 0, 0);
const glm::vec3 green = glm::vec3(0, 1, 0);
const glm::vec3 blue = glm::vec3(0, 0, 1);
const glm::vec3 yellow = glm::vec3(1, 1, 0);
const glm::vec3 black = glm::vec3(0, 0, 0);
const glm::vec3 white = glm::vec3(1, 1, 1);
const std::unordered_map<string, glm::vec3> colors ({ {"red", red}, {"green", green}, {"blue", blue}, {"yellow", yellow}, {"black", black}, {"white", white} });

string to_str(glm::vec3 color)
{
	for (const auto& item : colors)
		if (color == item.second)
			return item.first;
	return "";
}

Tema1::Tema1() = default;

Tema1::~Tema1() = default;

void Tema1::Init()
{
	const auto resolution = window->GetResolution();
	auto* camera = GetSceneCamera();
	camera->SetOrthographic(0, static_cast<float>(resolution.x), 0, static_cast<float>(resolution.y), 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	arrowColor = red;
	player = new Player(red, gameSpeed);
	player->Init(meshes);

	for (auto i = 0; i < numberBalloons; i++)
		balloons.push_back(new Balloon(rand() % 2 ? red : yellow, gameSpeed));
	balloons.back()->Init(meshes);

	arrows.push_back(new Arrow(arrowColor, player, gameSpeed));
	arrows[0]->Init(meshes);
	
	for (auto i = 0; i < numberShuriken; i++)
		shurikens.push_back(new Shuriken(red, gameSpeed));
	shurikens[0]->Init(meshes);
		
	powerBar = new PowerBar(red, player);
	powerBar->Init(meshes);

}

void Tema1::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const auto resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	time += deltaTimeSeconds;
	powerUp -= deltaTimeSeconds;
	glLineWidth(3);

	CheckArrows();
	CheckBalloons();
	VerifyCollisionArrow();
	VerifyCollisionShuriken();
	
	if (time > nextShotTime && (numberArrows == 0 || powerUp > 0))
		NextArrow();

	if (mustRemoveShuriken)
		RemoveShuriken();
	
	glm::mat3 matrix = player->Update(deltaTimeSeconds);
	auto id = "bow_" + to_str(player->color);
	RenderMesh2D(meshes[id], shaders["VertexColor"], matrix);
	
	for (auto& arrow : arrows) {
		matrix = arrow->Update(deltaTimeSeconds);
		for (const auto& component : arrow_components) {
			id = component + to_str(arrow->color);
			RenderMesh2D(meshes[id], shaders["VertexColor"], matrix);
		}
	}
	for (auto& balloon : balloons) {
		matrix = balloon->Update(deltaTimeSeconds);
		for (const auto& component : balloon_components) {
			id = component + to_str(balloon->color);
			RenderMesh2D(meshes[id], shaders["VertexColor"], matrix);
		}
	}
	for (auto& shuriken : shurikens) {
		matrix = shuriken->Update(deltaTimeSeconds);
		id = "shuriken_" + to_str(shuriken->color);
		RenderMesh2D(meshes[id], shaders["VertexColor"], matrix);
	}
	if (powerBar->show) {
		matrix = powerBar->Update(deltaTimeSeconds);
		id = "power_bar_" + to_str(powerBar->color);
		RenderMesh2D(meshes[id], shaders["VertexColor"], matrix);
	}
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W) && !(player->transform.position_y > 690))
		player->transform.position_y += player->speed * deltaTime;

	if (window->KeyHold(GLFW_KEY_S) && !(player->transform.position_y < 30))
		player->transform.position_y -= player->speed * deltaTime;
}

void Tema1::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_A)
		cout << arrows.size() << endl;

	if (key == GLFW_KEY_B) {
		cout << balloons.size() << endl;
		for (auto* balloon : balloons)
		{
			cout << balloon->transform.position_x << " ";
			cout << balloon->transform.position_y << " " << balloon->color  << " " << balloon << endl;
		}
		cout << endl;
	}
	
	if (key == GLFW_KEY_P)
		PauseGame();

	if (key == GLFW_KEY_L)
		ModifyScore(50);

	if (key == GLFW_KEY_T)
		AddBalloon();
	
	if (key == GLFW_KEY_E)
		AddShuriken();

	if (key == GLFW_KEY_1)
		BlackArrowLevel();
	
	if (key == GLFW_KEY_2)
		PowerUpLevel();
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	player->OnMouseMove(mouseX, mouseY);
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && time > nextShotTime) {
		for (auto& arrow : arrows) {
			if (!arrow->fired) {
				arrow->OnMouseBtnPress();
				powerBar->OnMouseBtnPress();
			}
		}
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && time > nextShotTime) {
		for (auto& arrow : arrows) {
			if (!arrow->fired) {
				arrow->OnMouseBtnRelease();
				powerBar->OnMouseBtnRelease();
				nextShotTime = time + 1;
				numberArrows = 0;
			}
		}
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

void Tema1::NextArrow()
{
	arrows.push_back(new Arrow(arrowColor, player, gameSpeed));
	numberArrows = 1;
}

void Tema1::CheckArrows()
{
	for (auto i = 0; i < arrows.size(); i++)
	{
		if (!arrows[i]->fired)
		if (arrows[i]->transform.position_x > 1280 || arrows[i]->transform.position_x < 0
			|| arrows[i]->transform.position_y < 0 || arrows[i]->transform.position_y > 720)
		{
			delete arrows[i];
			arrows.erase(arrows.begin() + i);
		}
	}
}

void Tema1::CheckBalloons()
{
	for (int i = 0; i < balloons.size();i++)
	{
		if (balloons[i]->transform.position_y > 820 || balloons[i]->transform.position_y < -200)
		{
			delete balloons[i];
			balloons[i] = new Balloon(rand() % 2 ? red : yellow, gameSpeed);
			// if (mustRemoveBalloon) {
			// 	const auto it = find(balloons.begin(), balloons.end(), balloon);
			// 	balloons.erase(it);
			// 	mustRemoveBalloon = false;
			// }
			// else

		}
		float a = balloons[i]->burst;
	}
}

void Tema1::VerifyCollisionArrow()
{
	for (int i = 0; i < balloons.size(); i++)
	{
		if (balloons[i]->burst)
			continue;
		for (auto & arrow : arrows)
		{
			if (!arrow->fired)
				continue;
			const auto tip_arrow_x = arrow->transform.position_x + 120 * cos(arrow->transform.angle);
			const auto tip_arrow_y = arrow->transform.position_y + 120 * sin(arrow->transform.angle);
			const auto center_balloon_x = balloons[i]->transform.position_x + 15 * cos(balloons[i]->transform.angle);
			const auto center_balloon_y = balloons[i]->transform.position_y + 25 + 25 * cos(balloons[i]->transform.angle);
			const auto delta_x = center_balloon_x - tip_arrow_x;
			const auto delta_y = center_balloon_y - tip_arrow_y;
			if (delta_x * delta_x / 900 + delta_y * delta_y / 2500 < 1) {
				balloons[i]->Burst();
				ModifyScore(balloons[i]->color == red ? 10 : -10);
			}
		}
	}
}

void Tema1::VerifyCollisionShuriken()
{
	for (auto* shuriken : shurikens) {
		if (shuriken->hit_player)
			continue;
		const auto delta_x = shuriken->transform.position_x - player->transform.position_x;
		const auto delta_y = shuriken->transform.position_y - player->transform.position_y;
		if (delta_y * delta_y + delta_x * delta_x < 6400)
		{
			shuriken->hit_player = true;
			lives--;
			cout << "Lives: " << lives << endl;
			ModifyScore(-100);
		}
	}
}

void Tema1::ModifyScore(int points)
{
	score += points;
	cout << "Score: " << score << endl;
	if (score / 50 != level)
		LevelUp();
}

void Tema1::LevelUp()
{
	if (level < score / 50)
	{
		arrowColor = red;
		powerUp = 0;
		// if (rand() % 10 == 0)
		// 	PowerUpLevel();
		if (rand() % 10 == 1)
			BlackArrowLevel();
	}
	
	level = max(0, score / 50);
	gameSpeed = 100 + level * 20;
	if (numberBalloons < max(level / 3 + 2, 2))
		AddBalloon();
	
	if (numberShuriken < max(level / 4 + 1, 1))
		AddShuriken();

	player->SetSpeed(gameSpeed);

	for (auto& arrow : arrows)
		arrow->SetSpeed(gameSpeed);

	for (auto& balloon : balloons)
		if (!balloon->burst)
		balloon->SetSpeed(gameSpeed);

	for (auto& shuriken : shurikens)
		shuriken->SetSpeed(gameSpeed);
	
	if (numberBalloons > max(level / 3 + 2, 2))
		mustRemoveBalloon = true;
	
	if (numberShuriken > max(level / 4 + 1, 1))
		mustRemoveShuriken = true;
}

void Tema1::AddBalloon()
{
	balloons.push_back(new Balloon(rand() % 2 ? red : yellow, gameSpeed));
	numberBalloons++;
}

void Tema1::RemoveBalloon()
{
	delete balloons.back();
	balloons.erase(balloons.end() - 1);
	numberBalloons--;
	mustRemoveBalloon = false;
}

void Tema1::AddShuriken()
{
	shurikens.push_back(new Shuriken(blue, gameSpeed));
	numberShuriken++;
}

void Tema1::RemoveShuriken()
{
	delete shurikens.back();
	shurikens.erase(shurikens.end() - 1);
	numberShuriken--;
	mustRemoveShuriken = false;
}

void Tema1::PowerUpLevel()
{
	powerUp = 30;
	cout << "Special level: Multiple arrows\n";
}

void Tema1::BlackArrowLevel()
{
	arrowColor = black;
	cout << "Expert level: Invisible arrow\n";
}

void Tema1::PauseGame()
{
	if (pause)
	{
		pause = false;
		gameSpeed *= 100000;
	}
	else {
		pause = true;
		gameSpeed /= 100000;
		player->SetSpeed(gameSpeed);
	}
	for (auto& arrow : arrows)
		arrow->SetSpeed(gameSpeed);

	for (auto& balloon : balloons)
		if (!balloon->burst)
			balloon->SetSpeed(gameSpeed);

	for (auto& shuriken : shurikens)
		shuriken->SetSpeed(gameSpeed);
}

