#include "bullet.h"
#include <bullet.h>


void Bullet::render(gl2d::Renderer2D& renderer, gl2d::Texture bulletsTexture,
	gl2d::TextureAtlasPadding bulletsAtlas)
{
	float bulletDirection = atan2(fireDirection.y, -fireDirection.x);

	glm::vec4 textCoords = bulletsAtlas.get(1, 1);
	if (isEnemy)
	{
		textCoords = bulletsAtlas.get(0, 0);
	}
	renderer.renderRectangle({ position - glm::vec2(25,25), 50, 50 },
		bulletsTexture, Colors_White, {}, glm::degrees(bulletDirection)+90.f, textCoords);
}

void Bullet::update(float deltaTime)
{
	position += fireDirection * deltaTime * speed;
}

