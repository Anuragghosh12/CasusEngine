#include "enemy.h"
#include "tiledRenderer.h"

void Enemy::render(gl2d::Renderer2D& renderer, gl2d::Texture& sprites,
	gl2d::TextureAtlasPadding& atlas)
{
	renderSpaceShip(renderer, position, enemyShipSize,
		sprites, atlas.get(type.x, type.y), viewDirection);
}

bool Enemy::update(float deltaTime, glm::vec2 playerPos)
{
	glm::vec2 directionPlayer = playerPos - position;
	if (glm::length(directionPlayer) == 0)
	{
		directionPlayer = {1,0};
	}
	else
	{
		directionPlayer = glm::normalize(directionPlayer);
	}
	glm::vec2 newDirection = {};

	bool shoot = glm::length(directionPlayer + viewDirection) >= fireRange;
	if (shoot)
	{
		if (firedTime <= 0.f)
		{
			//we can shoot
			firedTime = fireTimeReset;
		}
		else
		{
			shoot = 0;
		}
	}
	firedTime -= deltaTime;
	if (firedTime < 0) 
	{
		firedTime - 0.f;
	}

	if (glm::length(newDirection+viewDirection) <= 0.2)
	{
		if(rand()%2)
		{
			newDirection = glm::vec2(directionPlayer.y, -directionPlayer.x);
		}
		else
		{
			newDirection = glm::vec2(-directionPlayer.y, directionPlayer.x);
		}
	}
	else
	{
		newDirection = deltaTime * turnSpeed * directionPlayer + viewDirection;
	}
	float length = glm::length(newDirection);
	viewDirection = glm::normalize(newDirection);
	length = glm::clamp(length, 0.1f, 3.f);
	position += viewDirection * deltaTime * speed * length;

	return shoot;
}
