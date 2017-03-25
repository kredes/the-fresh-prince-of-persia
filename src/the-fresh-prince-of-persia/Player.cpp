#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define Y_PLAYER_OFFSET -40
#define PLAYER_STEP 64


enum PlayerAnim
{
	STAND_LEFT,
	STAND_RIGHT,
	
	// RUN
	RUN_LEFT,
	RUN_RIGHT,

	// WALK
	WALK_LEFT,
	WALK_RIGHT

};

enum InputKey
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

glm::vec2 getTexCoords(int idx) {
	return glm::vec2(0.00462962962 * idx, 0);
}

void addKeyframes(Sprite* sprite, int anim, int idx1, int idx2) {
	for (int i = idx1; i <= idx2; i++)
	{
		sprite->addKeyframe(anim, getTexCoords(i));
	}
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	isMoving = false;
	// No movement
	movementDir = -1;
	initMovementPos = posPlayer.x;
	spritesheet.loadFromFile("sprites/kid/tileset.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(56, 57), glm::vec2(0.00462962962, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, getTexCoords(14));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, getTexCoords(14));
		
		sprite->setAnimationSpeed(RUN_LEFT, 16);
		addKeyframes(sprite, RUN_LEFT, 1, 13);
		
		sprite->setAnimationSpeed(RUN_RIGHT, 16);
		addKeyframes(sprite, RUN_RIGHT, 1, 13);

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	Player::changeState(STANDING_RIGHT);
}

void Player::changeState(PlayerState nextState) {
	switch (nextState)
	{
	case STANDING_LEFT:
		sprite->changeAnimation(STAND_LEFT);
		break;
	case STANDING_RIGHT:
		sprite->changeAnimation(STAND_RIGHT);
		break;
	case WALKING_LEFT:
		break;
	case WALKING_RIGHT:
		break;
	case START_WALKING_RIGHT:
		break;
	case START_WALKING_LEFT:
		break;
	case RUNNING_LEFT:
		sprite->changeAnimation(RUN_LEFT, 5);
		break;
	case RUNNING_RIGHT:
		sprite->changeAnimation(RUN_RIGHT, 5);
		break;
	case START_RUNNING_RIGHT:
		sprite->changeAnimation(RUN_RIGHT, 0, 4);
		break;
	case START_RUNNING_LEFT:
		sprite->changeAnimation(RUN_LEFT, 0, 4);
		break;
	case END_RUNNING_RIGHT:
		break;
	case END_RUNNING_LEFT:
		break;
	case TURNING_LEFT:
		break;
	case TURNING_RIGHT:
		break;
	case TURNING_RUNNING_LEFT:
		break;
	case TURNING_RUNNING_RIGHT:
		break;
	case CROUCHING_LEFT:
		break;
	case CROUCHING_RIGHT:
		break;
	case START_CROUCHING_LEFT:
		break;
	case START_CROUCHING_RIGHT:
		break;
	case END_CROUCHING_LEFT:
		break;
	case END_CROUCHING_RIGHT:
		break;
	case JUMPING_LEFT:
		break;
	case JUMPING_RIGHT:
		break;
	case START_JUMPING_LEFT:
		break;
	case START_JUMPING_RIGHT:
		break;
	case END_JUMPING_LEFT:
		break;
	case END_JUMPING_RIGHT:
		break;
	case JUMPING_RUNNING_LEFT:
		break;
	case JUMPING_RUNNING_RIGHT:
		break;
	case START_JUMPING_RUNNING_LEFT:
		break;
	case START_JUMPING_RUNNING_RIGHT:
		break;
	case END_JUMPING_RUNNING_LEFT:
		break;
	case END_JUMPING_RUNNING_RIGHT:
		break;
	default:
		break;
	}
	currentState = nextState;
	cout << "Player state changed to: " << getStateName(currentState) << endl;
}

void Player::update(int deltaTime)
{
	switch (currentState)
	{
	case STANDING_LEFT:
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			changeState(START_RUNNING_LEFT);
		}
		break;
	case STANDING_RIGHT:
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			changeState(START_RUNNING_LEFT);
		}
		break;
	case WALKING_LEFT:
		break;
	case WALKING_RIGHT:
		break;
	case START_WALKING_RIGHT:
		break;
	case START_WALKING_LEFT:
		break;
	case RUNNING_LEFT:
		if (sprite->isAtEndingKeyframe()) {
			if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				changeState(STANDING_LEFT);
				break;
			}
		}
		break;
	case RUNNING_RIGHT:
		break;
	case START_RUNNING_RIGHT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(RUNNING_RIGHT);
		}
		break;
	case START_RUNNING_LEFT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			// If we are not pressing the left key, there is no point on keeping
			// running
			if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				changeState(STANDING_LEFT);
				break;
			}
		}
		break;
	case END_RUNNING_RIGHT:
		break;
	case END_RUNNING_LEFT:
		break;
	case TURNING_LEFT:
		break;
	case TURNING_RIGHT:
		break;
	case TURNING_RUNNING_LEFT:
		break;
	case TURNING_RUNNING_RIGHT:
		break;
	case CROUCHING_LEFT:
		break;
	case CROUCHING_RIGHT:
		break;
	case START_CROUCHING_LEFT:
		break;
	case START_CROUCHING_RIGHT:
		break;
	case END_CROUCHING_LEFT:
		break;
	case END_CROUCHING_RIGHT:
		break;
	case JUMPING_LEFT:
		break;
	case JUMPING_RIGHT:
		break;
	case START_JUMPING_LEFT:
		break;
	case START_JUMPING_RIGHT:
		break;
	case END_JUMPING_LEFT:
		break;
	case END_JUMPING_RIGHT:
		break;
	case JUMPING_RUNNING_LEFT:
		break;
	case JUMPING_RUNNING_RIGHT:
		break;
	case START_JUMPING_RUNNING_LEFT:
		break;
	case START_JUMPING_RUNNING_RIGHT:
		break;
	case END_JUMPING_RUNNING_LEFT:
		break;
	case END_JUMPING_RUNNING_RIGHT:
		break;
	default:
		break;
	}
	sprite->update(deltaTime);
	return;
	

	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (!isMoving) {
			isMoving = true;
			movementDir = WALK_LEFT;
			initMovementPos = posPlayer.x;
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (!isMoving) {
			isMoving = true;
			movementDir = WALK_RIGHT;
			initMovementPos = posPlayer.x;
		}
	}
	else
	{
		// If no key has been pressed, we wait for the movement to stop and
		// we execute the idle animation
		if (!isMoving) {
			if (sprite->animation() == WALK_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == WALK_RIGHT)
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

void Player::move(int direction) {
	if (!isMoving)
		return;
	if (sprite->animation() != direction)
		sprite->changeAnimation(direction);
	// If we are moving to the left, we add to the player's position
	// 2 pixels to the left, if not, to the right.
	int stride = direction == WALK_LEFT ? -2 : 2;
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

string Player::getStateName(PlayerState state) {
	switch (state)
	{
	case STANDING_LEFT:
		return "STANDING_LEFT";
		break;
	case STANDING_RIGHT:
		return "STANDING_RIGHT";
		break;
	case WALKING_LEFT:
		return "WALKING_LEFT";
		break;
	case WALKING_RIGHT:
		return "WALKING_RIGHT";
		break;
	case START_WALKING_RIGHT:
		return "START_WALKING_RIGHT";
		break;
	case START_WALKING_LEFT:
		return "START_WALKING_LEFT";
		break;
	case RUNNING_LEFT:
		return "RUNNING_LEFT";
		break;
	case RUNNING_RIGHT:
		return "RUNNING_RIGHT";
		break;
	case START_RUNNING_RIGHT:
		return "START_RUNNING_RIGHT";
		break;
	case START_RUNNING_LEFT:
		return "START_RUNNING_LEFT";
		break;
	case END_RUNNING_RIGHT:
		return "END_RUNNING_RIGHT";
		break;
	case END_RUNNING_LEFT:
		return "END_RUNNING_LEFT";
		break;
	case TURNING_LEFT:
		return "TURNING_LEFT";
		break;
	case TURNING_RIGHT:
		return "TURNING_RIGHT";
		break;
	case TURNING_RUNNING_LEFT:
		return "TURNING_RUNNING_LEFT";
		break;
	case TURNING_RUNNING_RIGHT:
		return "TURNING_RUNNING_RIGHT";
		break;
	case CROUCHING_LEFT:
		return "CROUCHING_LEFT";
		break;
	case CROUCHING_RIGHT:
		return "CROUCHING_RIGHT";
		break;
	case START_CROUCHING_LEFT:
		return "START_CROUCHING_LEFT";
		break;
	case START_CROUCHING_RIGHT:
		return "START_CROUCHING_RIGHT";
		break;
	case END_CROUCHING_LEFT:
		return "END_CROUCHING_LEFT";
		break;
	case END_CROUCHING_RIGHT:
		return "END_CROUCHING_RIGHT";
		break;
	case JUMPING_LEFT:
		return "JUMPING_LEFT";
		break;
	case JUMPING_RIGHT:
		return "JUMPING_RIGHT";
		break;
	case START_JUMPING_LEFT:
		return "START_JUMPING_LEFT";
		break;
	case START_JUMPING_RIGHT:
		return "START_JUMPING_RIGHT";
		break;
	case END_JUMPING_LEFT:
		return "END_JUMPING_LEFT";
		break;
	case END_JUMPING_RIGHT:
		return "END_JUMPING_RIGHT";
		break;
	case JUMPING_RUNNING_LEFT:
		return "JUMPING_RUNNING_LEFT";
		break;
	case JUMPING_RUNNING_RIGHT:
		return "JUMPING_RUNNING_RIGHT";
		break;
	case START_JUMPING_RUNNING_LEFT:
		return "START_JUMPING_RUNNING_LEFT";
		break;
	case START_JUMPING_RUNNING_RIGHT:
		return "START_JUMPING_RUNNING_RIGHT";
		break;
	case END_JUMPING_RUNNING_LEFT:
		return "END_JUMPING_RUNNING_LEFT";
		break;
	case END_JUMPING_RUNNING_RIGHT:
		return "END_JUMPING_RUNNING_RIGHT";
		break;
	default:
		break;
	}
}



