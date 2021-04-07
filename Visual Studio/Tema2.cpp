#include "Tema2.h"

#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include "Core/GPU/GPUBuffers.h"
#include "Core/GPU/Mesh.h"
#include "Core/GPU/Shader.h"
#include "Core/Managers/ResourcePath.h"
#include <Core/Engine.h>

glm::vec3 red(3, 0, 0);
glm::vec3 green(0, 3, 0);
glm::vec3 blue(0, 0, 3);
glm::vec3 yellow(5, 5, 0);
glm::vec3 orange(3, 1, 0);
glm::vec3 black(0, 0, 0);
glm::vec3 white(3, 3, 3);
glm::vec3 purple(147.0 / 256, 112.0 / 256, 219.0 / 256);

glm::vec3 chooseColor(int bad, int good)
{
	int c = rand() % 100;
	if (c < bad) {
		switch (c % 3) {
			case 0: return red;
			case 1: return orange;
			case 2: return yellow;
			default: return black;
		}
	}
	
	if (c < bad + good)
		return green;

	return blue;
}

bool intersect(glm::vec3 sphere, float radius, glm::vec3 box, glm::vec3 scale, glm::vec3 & point) {

	point.x = glm::max(box.x - scale.x / 2, glm::min(sphere.x, box.x + scale.x / 2));
	point.y = glm::max(box.y - scale.y / 2, glm::min(sphere.y, box.y + scale.y / 2));
	point.z = glm::max(box.z - scale.z / 2, glm::min(sphere.z, box.z + scale.z / 2));
	
	const auto distance = sqrt((point.x - sphere.x) * (point.x - sphere.x) +
		(point.y - sphere.y) * (point.y - sphere.y) + (point.z - sphere.z) * (point.z - sphere.z));

	return distance < radius;
}

Tema2::Tema2() = default;

Tema2::~Tema2() = default;

void Tema2::Init()
{
	LoadData();
	StartGame();
}

void Tema2::LoadData()
{
	const std::string textureLoc = "Source/Laboratoare/Laborator9/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass_bilboard.png").c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		mapTextures["crate"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "earth.png").c_str(), GL_REPEAT);
		mapTextures["earth"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/Models/Vegetation/Bamboo/bamboo.png", GL_REPEAT);
		mapTextures["bamboo"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/game_over.png", GL_REPEAT);
		mapTextures["game_over"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/front_trees.png", GL_REPEAT);
		mapTextures["foreground"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/back_sky.jpg", GL_REPEAT);
		mapTextures["sky"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/0.png", GL_REPEAT);
		mapTextures["0"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/1.png", GL_REPEAT);
		mapTextures["1"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/2.png", GL_REPEAT);
		mapTextures["2"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/3.png", GL_REPEAT);
		mapTextures["3"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/4.png", GL_REPEAT);
		mapTextures["4"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/5.png", GL_REPEAT);
		mapTextures["5"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/6.png", GL_REPEAT);
		mapTextures["6"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/7.png", GL_REPEAT);
		mapTextures["7"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/8.png", GL_REPEAT);
		mapTextures["8"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/9.png", GL_REPEAT);
		mapTextures["9"] = texture;
	}
	
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/Textures/black.jpg", GL_REPEAT);
		mapTextures["black"] = texture;
	}

	
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh * mesh = new Mesh("quad");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "screen_quad.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		std::vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		std::vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		std::vector<glm::vec2> textureCoords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f)
		};

		std::vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textureCoords;
		int x = 300;
		float step = 0.1f;
		unsigned short points = 0;
		std::vector<unsigned short> indices;
		for (float h = 0; h < 2; h++)
		{
			for (float angle = 0; angle < 2 * M_PI + 0.1; angle += step)
			{
				vertices.push_back(glm::vec3(cos(angle), (x+50) * h, sin(angle)));
				normals.push_back(glm::vec3(cos(angle), 0, sin(angle)));
				textureCoords.push_back(glm::vec2(h, 0.01 + angle / (30 * M_PI)));
				if (!h) points++;
			}
		}
		vertices.push_back(glm::vec3(0, 0, 0));
		normals.push_back(glm::vec3(0, -1, 0));
		textureCoords.push_back(glm::vec2(0.5, 0));
		vertices.push_back(glm::vec3(0, 50, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2(0.5, 0));
		for (unsigned short i = 0; i < points; i++)
		{
			unsigned short next = (i + 1) % points;
			indices.push_back(i);
			indices.push_back(next);
			indices.push_back(i + points);

			indices.push_back(i + points);
			indices.push_back(next + points);
			indices.push_back(i + 1);

			indices.push_back(i);
			indices.push_back(next);
			indices.push_back(2 * points);

			indices.push_back(i + points);
			indices.push_back(next + points);
			indices.push_back(2 * points + 1);
		}

		points = vertices.size();
		vertices.push_back(glm::vec3(0, x + 30, 15));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2(1, 1));

		vertices.push_back(glm::vec3(0, x + 30, -15));
		normals.push_back(glm::vec3(0, 0, 0));
		textureCoords.push_back(glm::vec2(0, 1));

		vertices.push_back(glm::vec3(30, x + 40, -15));
		normals.push_back(glm::vec3(0, 0, 0));
		textureCoords.push_back(glm::vec2(0, 0.2));

		vertices.push_back(glm::vec3(30, x + 40, 15));
		normals.push_back(glm::vec3(0, 0, 0));
		textureCoords.push_back(glm::vec2(1, 0.2));
		indices.push_back(points);
		indices.push_back(points + 1);
		indices.push_back(points + 2);

		indices.push_back(points + 2);
		indices.push_back(points + 3);
		indices.push_back(points);

		points = vertices.size();
		vertices.push_back(glm::vec3(0, x + 50, 15));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2(1, 1));

		vertices.push_back(glm::vec3(0, x + 50, -15));
		normals.push_back(glm::vec3(0, 0, 0));
		textureCoords.push_back(glm::vec2(0, 1));

		vertices.push_back(glm::vec3(30, x + 60, -15));
		normals.push_back(glm::vec3(0, 0, 0));
		textureCoords.push_back(glm::vec2(0, 0.2));

		vertices.push_back(glm::vec3(30, x + 60, 15));
		normals.push_back(glm::vec3(0, 0, 0));
		textureCoords.push_back(glm::vec2(1, 0.2));
		indices.push_back(points);
		indices.push_back(points + 1);
		indices.push_back(points + 2);

		indices.push_back(points + 2);
		indices.push_back(points + 3);
		indices.push_back(points);


		Mesh* mesh = new Mesh("bamboo");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Visual Studio/Tema2/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Visual Studio/Tema2/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Tema2::StartGame()
{
	playerPosition = glm::vec3(0, 1, 0);
	camera = new Laborator::Camera();
	camera->Set(playerPosition + def_camera, playerPosition, glm::vec3(0, 1, 0));

	lightPosition1 = glm::vec3(-5, 2, -1);
	lightPosition2 = glm::vec3(6, 2, -1);
	lightDirection = glm::normalize(glm::vec3(-1, -0.5, 0));
	materialShininess = 30;
	materialKd = 0.5;
	materialKs = 0.5;
	cutOffAngle = 40;
	lightIntensity1 = 20;
	lightIntensity2 = 2;

	lightColor1 = blue;
	lightColor2 = red;
	
	playerSpeed = glm::vec3(0, 0, 0.5f);
	max_speed = 3;
	min_speed = 0.5f;

	chance_platform = initial_chance_platform;
	chance_good_platform = initial_chance_good_platform;
	chance_bad_platform = initial_chance_bad_platform;
	chance_obstacle = initial_chance_obstacle;
	chance_power_ups = initial_chance_power_ups;

	num_columns = 3;
	next_row = -30;
	
	firstPerson = false;
	pause = false;
	jumped = false;
	can_jump = true;
	land = false;
	game_over = false;
	invincibility = false;
	fall = false;
	collided = false;
	magnet = false;
	deform = false;
	penalty_speed = false;
	
	fuel = max_fuel;
	lives = max_lives;

	level = 1;
	score = 0;
	next_level_score = 100;
	
	AddRow(0, 100);

	for (float z = -step_platforms; z > next_row + 1; z -= step_platforms)
		AddRow(z, chance_platform);

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
	frame++;
	if (!pause && frame % 120 == 0)
		std::cout << playerPosition << " " << fuel << std::endl;
}

void Tema2::Update(float deltaTimeSeconds)
{
	score += last_z - playerPosition.z;
	last_z = playerPosition.z;
	const auto time = Engine::GetElapsedTime() - paused_time;

	if (!pause)
		Movement(deltaTimeSeconds);
	else
		paused_time += deltaTimeSeconds;

	if (!fall && playerPosition.y < -5)
	{
		fall = true;
		time_of_fall = time;
		std::cout << "Falling\n";
	}

	CollisionObstacle();

	if (!jumped && Collision()) {
		MovePlayer(0, 0.3f - playerPosition.y, 0);
		playerSpeed.y = 0;
		if (!can_jump && !deform) {
			can_jump = true;
			land = true;
			time_of_land = time;
			std::cout << "Land" << std::endl;
		}
	}

	CheckFuel();
	AnimationTiming(deltaTimeSeconds);
	
	if (next_row > playerPosition.z - 30)
		NextRow();
	if (deform)
		is_deform = 1;
	glm::mat4 modelMatrix = translate(glm::mat4(1), playerPosition);
	modelMatrix = scale(modelMatrix, glm::vec3(player_radius));
	modelMatrix = scale(modelMatrix, scale_player);
	RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, red, mapTextures["earth"]);
	is_deform = 0;
	
	RenderPlatforms();
	RenderDecoration();
	RenderLives();
	RenderFuel();
	if (game_over)
		RenderGameOver();
	RenderScore();
	RenderBackground();
}

void Tema2::MovePlayer(glm::vec3 delta)
{
	MovePlayer(delta.x, delta.y, delta.z);
}

void Tema2::MovePlayer(float delta_x, float delta_y, float delta_z)
{
	playerPosition += glm::vec3(delta_x, delta_y, -delta_z);
	camera->TranslateForward(delta_z);
	camera->TranslateUpword(delta_y);
	camera->TranslateRight(delta_x);
	lightPosition1.z -= delta_z;
	lightPosition2.z -= delta_z;
}


void Tema2::Movement(float deltaTimeSeconds)
{
	MovePlayer(0, playerSpeed.y * deltaTimeSeconds, factor_speed * playerSpeed.z * deltaTimeSeconds);
	
	fuel -= 2 * factor_speed * playerSpeed.z * deltaTimeSeconds;
	playerSpeed.y -= g * deltaTimeSeconds;
}

bool Tema2::Collision()
{
	for (auto& platform : platforms)
	{
		glm::vec3 point;
		if (intersect(playerPosition, player_radius, platform.position, scale_platforms, point)) {
			if (platform.color == yellow)
				fuel -= fuel_penalty;
			if (platform.color == green)
				fuel += fuel_penalty;

			if (platform.color == red)
				LoseLive();

			if (platform.color == orange)
			{
				factor_speed = penalty_factor_speed;
				penalty_speed = true;
				time_of_penalty = Engine::GetElapsedTime();
			}

			if (platform.color != blue && platform.color != purple)
			{
				deform = true;
				time_of_deform = Engine::GetElapsedTime() - paused_time;
				can_jump = true;
			}

			platform.color = purple;

			return true;
		}
	}
	return false;
}

bool Tema2::CollisionObstacle()
{
	for (auto& platform : platforms)
	{
		if (!platform.obstacle)
			continue;
		glm::vec3 point;
		glm::vec3 position = platform.position + glm::vec3(0, 0.35f, 0);
		if (intersect(playerPosition, player_radius, position, scale_obstacle, point)) {
			// std::cout << "\t" << point << "  " << position << " " << playerPosition << std::endl;
			if (point.z == position.z + scale_obstacle.z/2  && point.y != position.y + scale_obstacle.y/2) {
				if (invincibility) {
					platform.obstacle = false;
				}
				else {
					if (!collided) {
						collided = true;
						std::cout << "Collided\n";
						time_of_collision = Engine::GetElapsedTime() - paused_time;
						platform.collided_obstacle = true;
					}
				}
			}
			else if((point.x == position.x - scale_obstacle.x / 2 || point.x == position.x + scale_obstacle.x / 2)
				&& point.y != position.y + scale_obstacle.y / 2) {
				std::cout << "Lateral\n";
				const float factor = (point.x == position.x + scale_obstacle.x / 2) ? 1.0f : -1.0f;
				MovePlayer(position.x - factor * (scale_obstacle.x  + scale_player.x) / 2  - playerPosition.x, 0, 0);
			}
			else {
				if (!jumped) {
					playerSpeed.y = 0;
					MovePlayer(0, 0.65f + player_radius / 2 - playerPosition.y, 0);
					if (!can_jump && !deform) {
						can_jump = true;
						land = true;
						time_of_land = Engine::GetElapsedTime() - paused_time;
						std::cout << "Land" << std::endl;
					}
				}
			}

			return true;
		}
	}
	return false;
}

void Tema2::CollectPowerUps()
{
}

void Tema2::AddRow(float z, int chance_platform)
{
	bool one_on_row = false;
	decorations.emplace_back(glm::vec3(-6, 0, z), rand() % 2, chooseColor(60, 20), false);
	decorations.emplace_back(glm::vec3(6, 0, z), rand() % 2, chooseColor(60, 20), true);
	for (int j = 0; j < num_columns; j++)
	{
		if (rand() % 100 > chance_platform && (j < num_columns - 1 || one_on_row))
			continue;
		one_on_row = true;
		float x = platform_position_x[j + (num_columns == 3 ? 1 : 0)];
		glm::vec3 position(x, 0, z);
		glm::vec3 color = chooseColor(chance_bad_platform, chance_good_platform);

		int chance = rand() % 100;
		bool power_up = false, obstacle = false;
		if (chance < chance_obstacle)
			obstacle = true;
		else if (chance < chance_obstacle + chance_power_ups)
			power_up = true;

		platforms.emplace_back(position, color, obstacle, power_up);
	}
}

void Tema2::NextRow()
{
	AddRow(next_row, chance_platform);

	next_row -= step_platforms;

	const float last_row_z = platforms[0].position.z;

	if (last_row_z - next_row > 40) {
		for (int i = 0; i < 5; i++)
		{
			if (last_row_z == platforms[i].position.z)
			{
				platforms.erase(platforms.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < 2; i++)
		{
			if (last_row_z == decorations[i].position.z)
			{
				decorations.erase(decorations.begin() + i);
				i--;
			}
		}
	}
}

void Tema2::RenderPlatforms()
{
	for (auto& platform : platforms)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, platform.position);
		modelMatrix = glm::scale(modelMatrix, scale_platforms);
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, platform.color, mapTextures["crate"]);

		if (platform.obstacle)
		{
			modelMatrix = glm::mat4(1);
			modelMatrix = translate(modelMatrix, glm::vec3(platform.position.x, platform.position.y + 0.35f, platform.position.z));
			modelMatrix = scale(modelMatrix, scale_obstacle);
			RenderSimpleMesh(meshes[platform.type ? "box" : "box"], shaders["ShaderTema2"], modelMatrix, black, mapTextures["crate"]);
		}

		if (platform.power_up)
		{
			if (platform.power_collected)
			{
				platform.power_radius *= 1.08;
			}
			else {
				float  dist_x = (playerPosition.x - platform.position.x);
				float  dist_y = (playerPosition.y - platform.position.y - 1);
				float  dist_z = (playerPosition.z - platform.position.z);
				float dist = sqrt(dist_x * dist_x + dist_y * dist_y + dist_z + dist_z);
				if (dist < player_radius + platform.power_radius) {
					platform.power_collected = true;
					score += 100;
				}
			}
			modelMatrix = translate(glm::mat4(1), glm::vec3(platform.position.x, platform.position.y + 1, platform.position.z));
			modelMatrix = scale(modelMatrix, glm::vec3(platform.power_radius));
			modelMatrix = glm::rotate(modelMatrix, (float)M_PI, glm::vec3(0, 1, 0));
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, black, mapTextures["bamboo"]);
		}
	}
}

void Tema2::RenderDecoration()
{
	for (auto& decoration : decorations)
	{
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1), decoration.position - glm::vec3(0, 45, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15));
		if(decoration.right) modelMatrix = glm::rotate(modelMatrix,	(float) M_PI, glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes["bamboo"], shaders["ShaderTema2"], modelMatrix, black, mapTextures["bamboo"]);

		if (decoration.has_light)
		{
			modelMatrix = translate(glm::mat4(1), glm::vec3(decoration.position.x, decoration.position.y + 1, decoration.position.z-0.5));
			modelMatrix = scale(modelMatrix, glm::vec3(0.2f));
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, decoration.color, mapTextures["black"]);
		}
	}
}

void Tema2::RenderLives()
{
	glm::vec3 position = glm::vec3(camera->position.x - 3.5f, camera->position.y + 0.8f, camera->position.z - 4);
	for (int i = 0; i < lives; i++)
	{
		glm::mat4 modelMatrix = translate(glm::mat4(1), glm::vec3(position.x + 0.4f * i, position.y, position.z));
		modelMatrix = scale(modelMatrix, glm::vec3(0.2f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, black, mapTextures["earth"]);
	}
}

void Tema2::RenderFuel()
{
	float f = static_cast<float>(fuel) / max_fuel;
	glm::vec3 position = glm::vec3(camera->position.x - 3.5f, camera->position.y + 0.5f, camera->position.z - 4);
	glm::mat4 modelMatrix =  translate(glm::mat4(1), position);
	modelMatrix = scale(modelMatrix, glm::vec3(0.5f * f, 0.1f, 0));
	RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::mix(red, green, f), mapTextures["black"]);
}

void Tema2::RenderScore()
{
	glm::vec3 position = glm::vec3(camera->position.x + 2.5f, camera->position.y + 0.8f, camera->position.z - 4);

	std::vector<int> digits;
	for (auto copy = score; copy; copy /= 10)
		digits.insert(digits.begin(), copy % 10);
	if (digits.empty())
		digits.push_back(0);

	glEnable(GL_CULL_FACE);
	for (int i = 0; i < digits.size(); i++)
	{
		glm::mat4 modelMatrix = translate(glm::mat4(1), glm::vec3(position.x + 0.4f * i, position.y, position.z));
		modelMatrix = scale(modelMatrix, glm::vec3(0.4f, 0.4f, 0));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, black, mapTextures[std::string(1, '0' + digits[i])]);
	}
	glDisable(GL_CULL_FACE);
}

void Tema2::RenderGameOver()
{
	const glm::vec3 position = glm::vec3(camera->position.x, camera->position.y - 0.5f, camera->position.z - 4);
	glm::mat4 modelMatrix = translate(glm::mat4(1), position);
	modelMatrix = scale(modelMatrix, glm::vec3(2, 2, 0.01f));
	RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, black, mapTextures["game_over"]);
}

void Tema2::RenderBackground()
{
	const auto position = glm::vec3(camera->position.x, camera->position.y - 30, camera->position.z);
	glm::mat4 modelMatrix = translate(glm::mat4(1), position);
	modelMatrix = scale(modelMatrix, glm::vec3(120));
	RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, black, mapTextures["sky"]);
}

void Tema2::LoseLive()
{
	lives--;
	fuel = max_fuel;

	scale_player = glm::vec3(1);
	playerPosition.y = 5;
	playerPosition.x = glm::clamp(playerPosition.x, 
		platform_position_x[num_columns == 3 ? 1 : 0], platform_position_x[num_columns == 5 ? 4 : 3]);
	playerSpeed = glm::vec3(0, 0 , min_speed);

	firstPerson = false;
	camera->Set(playerPosition + def_camera, playerPosition, glm::vec3(0, 1, 0));

	factor_speed = 1;
	penalty_speed = false;

	can_jump = true;

	std::cout << "Lost live: " << lives << " remaining\n";
	
	if (lives == 0)
		GameOver();
}

void Tema2::GameOver()
{
	pause = true;
	game_over = true;
	std::cout << "Game Over\n";
}

void Tema2::CheckFuel()
{
	if (fuel > 100)
		fuel = 100;
	if (fuel < 0)
		LoseLive();
}

void Tema2::AnimationTiming(float deltaTimeSeconds)
{
	const auto time = Engine::GetElapsedTime() - paused_time;
	
	if (jumped && time - time_of_jump < jump_deformation_duration)
		scale_player.y = 1 + 0.5 * sin(M_PI * (time - time_of_jump) / jump_deformation_duration);

	if (jumped && time - time_of_jump > jump_deformation_duration)
		jumped = false;

	if (land && time - time_of_land < land_deformation_duration)
		scale_player.y = 1 - 0.5 * sin(M_PI * (time - time_of_land) / land_deformation_duration);

	if (land && time - time_of_land > land_deformation_duration)
		land = false;

	if (fall && time - time_of_fall < fall_animation_duration) {
		scale_player = glm::vec3(1 - (time - time_of_fall) / fall_animation_duration);
		camera->RotateThirdPerson_OX(-0.15f * deltaTimeSeconds);
	}
	if (fall && time - time_of_fall > fall_animation_duration) {
		fall = false;
		scale_player = glm::vec3(1);
		LoseLive();
	}

	if (penalty_speed && time - time_of_penalty > penalty_duration)
	{
		factor_speed = 1;
		penalty_speed = false;
	}

	if (deform && time - time_of_deform > deform_duration)
		deform = false;

	if (collided)
	{
		// std::cout << "col\n";
		scale_player.z -= deltaTimeSeconds * playerSpeed.z;
		// std::cout << scale_player.z << std::endl;
		if (scale_player.z < 0) {
			scale_player.z = 0;
			time_of_collision = time;
			playerSpeed.z = 0;
		}
	}
	
	if (collided && time - time_of_collision > 1)
	{
		collided = false;
		LoseLive();
	}
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position1 = glGetUniformLocation(shader->program, "light_position1");
	glUniform3f(light_position1, lightPosition1.x, lightPosition1.y, lightPosition1.z);
	
	int light_position2 = glGetUniformLocation(shader->program, "light_position2");
	glUniform3f(light_position2, lightPosition2.x, lightPosition2.y, lightPosition2.z);

	int light_color1 = glGetUniformLocation(shader->program, "light_color1");
	glUniform3f(light_color1, lightColor1.x, lightColor1.y, lightColor1.z);

	int light_color2 = glGetUniformLocation(shader->program, "light_color2");
	glUniform3f(light_color2, lightColor2.x, lightColor2.y, lightColor2.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = camera->position;
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	int cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
	glUniform1f(cut_off_angle, cutOffAngle);

	int light_intensity = glGetUniformLocation(shader->program, "light_intensity1");
	glUniform1f(light_intensity, lightIntensity1);
	
	int light_intensity2 = glGetUniformLocation(shader->program, "light_intensity2");
	glUniform1f(light_intensity2, lightIntensity2);

	int time = glGetUniformLocation(shader->program, "time");
	glUniform1f(time, Engine::GetElapsedTime() - paused_time);
	
	int deform = glGetUniformLocation(shader->program, "deform");
	glUniform1i(deform, is_deform);
	
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
	}
	
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			camera->TranslateForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}
	} else
	{
		playerSpeed.x = 5;
		float speed = 2;
		if (window->KeyHold(GLFW_KEY_W)) {
			playerSpeed.z += speed * deltaTime;
			if (playerSpeed.z > max_speed)
				playerSpeed.z = max_speed;
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			playerPosition.x -= playerSpeed.x * deltaTime;
			camera->TranslateRight(-playerSpeed.x * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			playerSpeed.z -= speed * deltaTime;
			if (playerSpeed.z < min_speed)
				playerSpeed.z = min_speed;
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			playerPosition.x += playerSpeed.x * deltaTime;
			camera->TranslateRight(playerSpeed.x * deltaTime);
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C) {
		if (firstPerson)
		{
			firstPerson = false;
			camera->Set(playerPosition + def_camera, playerPosition, glm::vec3(0, 1, 0));
		} else
		{
			firstPerson = true;
			camera->Set(playerPosition + glm::vec3(0, 0.2f, -player_radius), playerPosition, glm::vec3(0, 1, 0));
			camera->RotateFirstPerson_OY(M_PI);
			camera->RotateFirstPerson_OX(-M_PI / 5);
		}
	}
	if (key == GLFW_KEY_P)
		pause = !pause;

	if (key == GLFW_KEY_L)
		LoseLive();

	if (key == GLFW_KEY_R)
		StartGame();
	
	if (key == GLFW_KEY_SPACE && can_jump) {
		playerSpeed.y += 5;
		jumped = true;
		can_jump = false;
		time_of_jump = Engine::GetElapsedTime();
	}

	if (key == GLFW_KEY_T)
	{
		for (auto& decoration : decorations)
			std::cout << decoration.position << "\n";
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(sensivityOX * deltaY);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(sensivityOY * deltaX);
		}

	}
}
