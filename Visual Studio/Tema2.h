#pragma once

#include "Decoration.h"
#include "Platform.h"
#include "Component/SimpleScene.h"
#include "Core/GPU/Texture2D.h"

#include "Laboratoare/Laborator5/LabCamera.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void LoadData();
	void StartGame();
	
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;

	void MovePlayer(glm::vec3 delta);
	void MovePlayer(float delta_x, float delta_y, float delta_z);
	void Movement(float deltaTimeSeconds);
	bool Collision();
	bool CollisionObstacle();
	void CollectPowerUps();
	void NextRow();
	void AddRow(float z, int chance_platform);
	void RenderPlatforms();
	void RenderDecoration();
	void RenderLives();
	void RenderFuel();
	void RenderScore();
	void RenderGameOver();
	void RenderBackground();
	void LoseLive();
	void GameOver();
	void CheckFuel();
	void AnimationTiming(float deltaTimeSeconds);

	// void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1), Texture2D* texture1 = nullptr);
	
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	
	glm::vec3 lightPosition1;
	glm::vec3 lightPosition2;
	glm::vec3 lightColor1;
	glm::vec3 lightColor2;
	glm::vec3 lightDirection;
	unsigned int materialShininess;
	float materialKd;
	float materialKs;
	float cutOffAngle;
	float lightIntensity1;
	float lightIntensity2;
	int is_deform;
	
	bool firstPerson = false;

	Laborator::Camera* camera;
	glm::vec3 def_camera = glm::vec3(0, 1, 3.5);
	glm::mat4 projectionMatrix;
	std::unordered_map<std::string, Texture2D*> mapTextures;
	
	glm::vec3 playerSpeed;
	glm::vec3 playerPosition;
	int last_z = 0;
	int frame = 0;
	int num_columns;
	int lives;
	const int max_lives = 3;

	int level;
	int score;
	int next_level_score;

	bool pause;
	bool invincibility;
	bool fall;
	bool collided;
	bool magnet;
	bool deform;
	bool jumped;
	bool can_jump;
	bool land;
	bool penalty_speed;
	bool game_over;

	const double jump_deformation_duration = 1;
	const double land_deformation_duration = 0.3;
	const double fall_animation_duration = 1;
	const double deform_duration = 1;
	const double penalty_duration = 10;

	double time_of_invincibility;
	double time_of_jump;
	double time_of_land;
	double time_of_penalty;
	double paused_time;
	double time_of_fall;
	double time_of_deform;
	double time_of_collision;
	
	glm::vec3 scale_player = glm::vec3(1);
	
	const float platform_position_x[5] = { -5, -2.5, 0, 2.5, 5 };
	float next_row;

	const float g = 4.9f;

	float fuel;
	const float max_fuel = 100;
	const float fuel_penalty = 20;

	float factor_speed = 1;
	const float penalty_factor_speed = 3;

	float max_speed;
	float min_speed;

	int chance_platform;
	int chance_obstacle;
	int chance_power_ups;
	int chance_good_platform;
	int chance_bad_platform;
	const int initial_chance_power_ups = 40;
	const int initial_chance_platform = 80;
	const int initial_chance_obstacle = 40;
	const int initial_chance_good_platform = 30;
	const int initial_chance_bad_platform = 30;

	const float power_up_radius = 0.3f;
	const float player_radius = 0.5f;
	const float step_platforms = 5;
	const glm::vec3 scale_obstacle = glm::vec3(0.6f, 0.6f, 2);
	const glm::vec3 scale_platforms = glm::vec3(1, 0.1f, 4);

	std::vector<Platform> platforms;
	std::vector<Decoration> decorations;
};

