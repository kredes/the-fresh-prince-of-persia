#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define Y_PLAYER_OFFSET -20
#define PLAYER_STEP 64


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	isMoving = false;
	// No movement
	movementDir = -1;
	initMovementPos = posPlayer.x;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::move(int direction) {
	if (!isMoving)
		return;
	if (sprite->animation() != direction)
		sprite->changeAnimation(direction);
	// If we are moving to the left, we add to the player's position
	// 2 pixels to the left, if not, to the right.
	int stride = direction == MOVE_LEFT ? -2 : 2;
	posPlayer.x += stride;
	if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
	{
		posPlayer.x += stride;
		// We can do this because of how the enum is declared:
		// MOVE_RIGHT = 3
		// STAND_RIGHT = 1
		// So, if we want the player to stand in the direction of the
		// latest movement, we can always substract 2 from the original
		// animation
		sprite->changeAnimation(direction - 2);
	}
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (!isMoving) {
			isMoving = true;
			movementDir = MOVE_LEFT;
			initMovementPos = posPlayer.x;
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (!isMoving) {
			isMoving = true;
			movementDir = MOVE_RIGHT;
			initMovementPos = posPlayer.x;
		}
	}
	else
	{
		// If no key has been pressed, we wait for the movement to stop and
		// we execute the idle animation
		if (!isMoving) {
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
	}

	move(movementDir);

	if (posPlayer.x > initMovementPos + PLAYER_STEP) {
		posPlayer.x = initMovementPos + PLAYER_STEP;
		isMoving = false;
		movementDir = -1;
	}
	if (posPlayer.x < initMovementPos - PLAYER_STEP) {
		posPlayer.x = initMovementPos - PLAYER_STEP;
		isMoving = false;
		movementDir = -1;
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(
		float(tileMapDispl.x + posPlayer.x),
		float(tileMapDispl.y + posPlayer.y + Y_PLAYER_OFFSET)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




