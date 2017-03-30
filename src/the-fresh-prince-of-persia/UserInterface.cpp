#include <GL/glew.h>
#include <GL/glut.h>
#include "UserInterface.h"

#define SIZE_UI_X 200
#define SIZE_UI_Y 20

#define SIZE_POINT_X 20
#define SIZE_POINT_Y 30

#define DISTANCE_BETWEEN_POINTS 30
#define DISTANCE_TO_BOT 10

#define X_TILES 14
#define Y_TILES 1

// Fuck c++
glm::vec2 getTexCoordsForTheUI(int xidx, int yidx) {
	return glm::vec2(
		(1.0f / (float)X_TILES) * (float)xidx,
		(1.0f / (float)Y_TILES) * (float)yidx
	);
}

Sprite* initHealthPointSprite(ShaderProgram & shaderProgram, 
	Texture& spritesheet, int screen_width, int screen_height, int index) {
	Sprite* sprite = Sprite::createSprite(
		// Sprite size
		glm::ivec2(SIZE_POINT_X, SIZE_POINT_Y),
		// The ammount to add to the texture coords
		glm::vec2(1.0f / (float)X_TILES, 1.0f / (float)Y_TILES),
		// The spritesheet
		&spritesheet,
		// The shader
		&shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, getTexCoordsForTheUI(10,0));

	sprite->changeAnimation(0);
	sprite->setPosition(
		glm::vec2(DISTANCE_BETWEEN_POINTS * (index + 1),
			screen_height - SIZE_POINT_Y - DISTANCE_TO_BOT));
	return sprite;
}

Sprite* initBlackSprite(ShaderProgram & shaderProgram,
	Texture& spritesheet, int screen_width, int screen_height) {
	Sprite* sprite = Sprite::createSprite(
		glm::ivec2(screen_width, SIZE_POINT_Y + DISTANCE_TO_BOT*2),
		// The ammount to add to the texture coords
		glm::vec2(.1f, .1f),
		// The spritesheet
		&spritesheet,
		// The shader
		&shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(.05,.05));

	sprite->changeAnimation(0);
	sprite->setPosition(
		glm::vec2(0,
			screen_height - SIZE_POINT_Y - DISTANCE_TO_BOT * 2));
	return sprite;
}

void UserInterface::init(ShaderProgram & shaderProgram, int screen_width,
	int screen_height, int _initialHealth)
{
	initialHealth = _initialHealth;
	playerHealth = _initialHealth;
	
	health_points_spritesheet.loadFromFile("sprites/general/health-points.png", 
		TEXTURE_PIXEL_FORMAT_RGBA);

	ui_back_spritesheet.loadFromFile("sprites/general/mi-negro.png",
		TEXTURE_PIXEL_FORMAT_RGB);

	// The background sprite
	ui_back_sprite = initBlackSprite(shaderProgram, ui_back_spritesheet, screen_width,
		screen_height);


	for (int i = 0; i < _initialHealth; i++)
	{
		health_point_sprites.push_back(initHealthPointSprite(shaderProgram, health_points_spritesheet, screen_width,
			screen_height, i));
	}
}

bool UserInterface::update(int deltaTime)
{
	return false;
}

void UserInterface::render()
{
	ui_back_sprite->render();
	for (int i = 0; i < playerHealth; i++)
	{
		health_point_sprites[i]->render();
	}
}

void UserInterface::updateHealthPoints(int ammount)
{
	playerHealth = ammount;
}
