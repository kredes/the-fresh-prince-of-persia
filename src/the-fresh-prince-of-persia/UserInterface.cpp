#include <GL/glew.h>
#include <GL/glut.h>
#include "UserInterface.h"

#define SIZE_UI_X 200
#define SIZE_UI_Y 20

#define X_TILES 3
#define Y_TILES 4

void UserInterface::init(ShaderProgram &shaderProgram, int initialHealth)
{
	playerHealth = initialHealth;

	spritesheet.loadFromFile("sprites/kid/tileset.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(
		// Sprite size
		glm::ivec2(SIZE_UI_X, SIZE_UI_Y),
		// The ammount to add to the texture coords
		glm::vec2(1.0f / (float)X_TILES, 1.0f / (float)Y_TILES),
		// The spritesheet
		&spritesheet,
		// The shader
		&shaderProgram);

}

bool UserInterface::update(int deltaTime)
{
	return false;
}

void UserInterface::render()
{
}

void UserInterface::updateHealthPoints(int ammount)
{

}
