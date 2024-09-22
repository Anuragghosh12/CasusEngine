#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <tiledRenderer.h>
#include <bullet.h>
#include <vector>
#include <enemy.h>
#include <glui/glui.h>
#include <raudio.h>


constexpr int BACKGROUNDS = 3;
const float shipSize = 250.f;
float TimeElapsed = 0.0f;

//Textures for assets
gl2d::Renderer2D renderer;
gl2d::Texture spaceShipsTexture;
gl2d::Texture backgroundTexture[BACKGROUNDS];
gl2d::TextureAtlasPadding spaceShipAtlas;
gl2d::Texture bulletsTexture;
gl2d::TextureAtlasPadding bulletAtlas;
gl2d::Texture healthBar;
gl2d::Texture health;
Sound shootSound;
Sound shootSoundP;

bool intersectBullet(glm::vec2 bulletPos, glm::vec2 shipPos, float shipSize)
{
	return glm::distance(bulletPos, shipPos) <= shipSize;
}


//gameplay struct

struct GameplayData
{
	glm::vec2 playerPos = { 100, 100 };

	std::vector<Bullet> bullets;

	std::vector<Enemy> enemies;

	float health = 1.f;

	float spawnEnemyTimerSeconds = 5.f;
};




//Instances
GameplayData data;
TiledRenderer tiledRenderer[BACKGROUNDS];



void restartGame()
{
	data = {};
	TimeElapsed = 0.f;
	renderer.currentCamera.follow(data.playerPos, 550, 0, 0, renderer.windowW, renderer.windowH);
}


bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();


	spaceShipsTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png",128, true);
	spaceShipAtlas = gl2d::TextureAtlasPadding(5, 2, spaceShipsTexture.GetSize().x, spaceShipsTexture.GetSize().y);

	bulletsTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/projectiles.png", 500, true);
	bulletAtlas = gl2d::TextureAtlasPadding(3, 2, bulletsTexture.GetSize().x, bulletsTexture.GetSize().y);


	backgroundTexture[0].loadFromFile(RESOURCES_PATH "background1.png", true);
	backgroundTexture[1].loadFromFile(RESOURCES_PATH "background2.png", true);
	backgroundTexture[2].loadFromFile(RESOURCES_PATH "background3.png", true);
	
	tiledRenderer[0].texture = backgroundTexture[0];
	tiledRenderer[1].texture = backgroundTexture[1];
	tiledRenderer[2].texture = backgroundTexture[2];

	tiledRenderer[0].paralaxStrength = 0;
	tiledRenderer[1].paralaxStrength = 0.5;
	tiledRenderer[2].paralaxStrength = 0.7;

	healthBar.loadFromFile(RESOURCES_PATH "healthBar.png", true);
	health.loadFromFile(RESOURCES_PATH "health.png", true);

	shootSound = LoadSound(RESOURCES_PATH "shoot.flac");
	SetSoundVolume(shootSound, 0.5f);
	/*shootSoundP = shootSound;
	SetSoundPitch(shootSoundP, 0.2);*/

	restartGame();
	return true;
}
void spawnEnemies()
{
	glm::uvec2 shipTypes[] = { {0,0},{0,1},{2,0},{3,1} };
	Enemy e;
	e.position = data.playerPos;

	glm::vec2 offset(2000, 0);
	offset = glm::vec2(glm::vec4(offset, 0, 1) * glm::rotate(glm::mat4(1.f), glm::radians((float)(rand() % 360)), glm::vec3(0, 0, 1)));

	e.position += offset;
	e.speed = 300 + rand() % 700;
	e.turnSpeed = 2.f + (rand() % 1000) / 500.f;
	e.type = shipTypes[rand() % 4];
	e.fireRange = 1.5 + (rand() / 1000) / 2000.f;
	e.fireTimeReset = 0.1 + (rand() % 1000) / 500;
	e.bulletSpeed = rand() % 3000 + 1000;
	data.enemies.push_back(e);
}





bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	

	renderer.updateWindowMetrics(w, h);
#pragma endregion

#pragma region movement

	glm::vec2 move = {};

	if (platform::isButtonHeld(platform::Button::W))
	{
		move.y = -1;
	}
	if (platform::isButtonHeld(platform::Button::S))
	{
		move.y = 1;
	}
	if (platform::isButtonHeld(platform::Button::A))
	{
		move.x = -1;
	}
	if (platform::isButtonHeld(platform::Button::D))
	{
		move.x = 1;
	}
	/*if (platform::isButtonHeld(platform::Button::W))
	{
		move.y = -1;
	}*/

	if (move.x != 0 || move.y != 0)
	{
		move = glm::normalize(move);

		move *= deltaTime * 2000; //200 pixel speed

		data.playerPos += move;
	}


#pragma endregion

#pragma region follow
	renderer.currentCamera.follow(data.playerPos, deltaTime * 150, 10, 200, w, h);
#pragma endregion

#pragma region render brackground
	renderer.currentCamera.zoom = 0.5;

	for (int i = 0; i < BACKGROUNDS; i++)
	{
		tiledRenderer[i].render(renderer);
	}
	
	
#pragma endregion


#pragma region mouse pos

	glm::vec2 mousePos = platform::getRelMousePosition();
	glm::vec2 screenCenter(w / 2.f, h / 2.f);

	glm::vec2 mouseDirection = mousePos - screenCenter;
	if (glm::length(mouseDirection) == 0.f)
	{
		mouseDirection = { 1,0 };
	}
	else
	{
		mouseDirection = normalize(mouseDirection);
	}

	float spaceShipAngle = atan2(mouseDirection.y,- mouseDirection.x);

#pragma endregion

#pragma region bullets

	if (platform::isLMousePressed())
	{
		Bullet b;
		b.position = data.playerPos;
		b.fireDirection = mouseDirection;
		b.speed = 2000.f;
		data.bullets.push_back(b);
		PlaySound(shootSound);
	}

	for (int i = 0; i < data.bullets.size(); i++)
	{
		if (glm::distance(data.bullets[i].position, data.playerPos) > 5'000)
		{
			data.bullets.erase(data.bullets.begin() + i);
			i--;
			continue;
		}

		if (!data.bullets[i].isEnemy)
		{
			bool breakBothLoops = false;
			for (int e = 0; e < data.enemies.size(); e++)
			{
				if (intersectBullet(data.bullets[i].position, data.enemies[e].position, enemyShipSize))
				{
					data.enemies[e].life -= 0.1;
					if(data.enemies[e].life<=0)
					{
						//Kill enemy
						data.enemies.erase(data.enemies.begin() + e);
					}
					data.bullets.erase(data.bullets.begin() + i);
					i--;
					breakBothLoops = true;
					continue;
				}
			}
			if (breakBothLoops)
				continue;
		}
		else
		{
			if (intersectBullet(data.bullets[i].position, data.playerPos, shipSize))
			{
				data.health -= 0.1;
				data.bullets.erase(data.bullets.begin() + i);
				i--;
				continue;
			}
		}

		data.bullets[i].update(deltaTime);
	}
	if (data.health <= 0)
	{
		//Kill player
		restartGame();
	}
	else
	{
		data.health += deltaTime * 0.1;
		data.health = glm::clamp(data.health, 0.f, 1.f);
	}
	/*for (auto& b : data.bullets)
	{
		b.update(deltaTime);
	}*/

#pragma endregion

#pragma region render bullets
	for (auto& b : data.bullets)
	{
		b.render(renderer, bulletsTexture, bulletAtlas);
	}
#pragma endregion

#pragma region handle enemy

	if (data.enemies.size() < 15)
	{
		data.spawnEnemyTimerSeconds -= deltaTime;
		if (data.spawnEnemyTimerSeconds < 0)
		{
			data.spawnEnemyTimerSeconds = rand() % 5 + 1;

			spawnEnemies();
			if (rand() % 3 == 0)
			{
				spawnEnemies();
			}
		}
	}



	for (int i = 0; i < data.enemies.size(); i++)
	{
		if (glm::distance(data.playerPos, data.enemies[i].position) > 4000.f)
		{
			//dispawn the enemies
			data.enemies.erase(data.enemies.begin() + i);
			i--;
			continue;
		}
		if (data.enemies[i].update(deltaTime, data.playerPos))
		{
			Bullet b;
			b.isEnemy = 1;
			b.position = data.enemies[i].position;
			b.fireDirection = data.enemies[i].viewDirection;
			b.speed = data.enemies[i].bulletSpeed;
			data.bullets.push_back(b);
			if (!IsSoundPlaying(shootSound))  PlaySound(shootSound);
		}
		data.enemies[i].update(deltaTime, data.playerPos);
	}
#pragma endregion

#pragma region render enemy

	for (int i = 0; i < data.enemies.size(); i++)
	{
		data.enemies[i].render(renderer, spaceShipsTexture, spaceShipAtlas);
	}

#pragma endregion


#pragma region render ship
	

	renderSpaceShip(renderer, data.playerPos, shipSize, spaceShipsTexture, spaceShipAtlas.get(3, 0), mouseDirection);
#pragma endregion

#pragma region render ui

	renderer.pushCamera();
	{
		glui::Frame f({ 0,0,w,h });
		glui::Box healthBox = glui::Box().xLeftPerc(0.65).yTopPerc(0.1).xDimensionPercentage(0.3).yAspectRatio(1.f / 8.f);

		renderer.renderRectangle(healthBox, healthBar);


		glm::vec4 newRect = healthBox();
		newRect.z *= data.health;

		glm::vec4 textCoords = { 0,1,1,0 };
		textCoords.z *= data.health;
		renderer.renderRectangle(newRect, health,Colors_White, {}, {}, textCoords);

	}
	renderer.popCamera();

#pragma endregion

	renderer.flush();

	TimeElapsed += deltaTime;
	ImGui::Begin("debug");

	ImGui::Text("Bullets count: %d", (int)data.bullets.size());
	ImGui::Text("Enemy count: %d", (int)data.enemies.size());
	ImGui::Text("Time Elapsed: %f", TimeElapsed);
	if (ImGui::Button("Spawn Enemy"))
	{
		spawnEnemies();
	}

	if (ImGui::Button("Restart Game"))
	{
		restartGame();
	}

	ImGui::SliderFloat("Player Health", &data.health, 0, 1);

	ImGui::End();

	//ImGui::ShowDemoWindow();


	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{



}
