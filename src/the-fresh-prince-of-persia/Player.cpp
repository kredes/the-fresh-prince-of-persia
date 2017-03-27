#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define Y_PLAYER_OFFSET -15
#define PLAYER_STEP 64

#define X_TILES 12
#define Y_TILES 18

#define PLAYER_RUN_SPEED 2
#define PLAYER_WALK_SPEED 1

#define PLAYER_SIZE_X 84
#define PLAYER_SIZE_Y 86

#define SHIFT_KEY 16

enum PlayerAnim
{
	STAND,

	START_RUN,
	RUN,
	END_RUN,

	WALK,

	TURN,

	RUN_TURN,

	START_CROUCH,
	CROUCH,
	END_CROUCH,
	WALK_CROUCH
};

enum InputKey
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

glm::vec2 getTexCoords(int xidx, int yidx) {
	return glm::vec2(
		1.0f/(float)X_TILES * (float)xidx, 
		1.0f/(float)Y_TILES * (float)yidx
	);
}

void addKeyframes(Sprite* sprite, int anim, int xidx, int yidx, int num) {
	
	for (int i = 1; i <= num; i++)
	{
		sprite->addKeyframe(anim, getTexCoords(xidx, yidx));
		++xidx;
		if (xidx >= X_TILES) {
			xidx = 0;
			++yidx;
		}
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
	sprite = Sprite::createSprite(
		// Sprite size
		glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y),
		// The ammount to add to the texture coords
		glm::vec2(1.0f/(float)X_TILES, 1.0f/(float)Y_TILES),
		// The spritesheet
		&spritesheet, 
		// The shader
		&shaderProgram);

	sprite->setNumberAnimations(20);
	
		sprite->setAnimationSpeed(STAND, 8);
		sprite->addKeyframe(STAND, getTexCoords(1,1));
		
		sprite->setAnimationSpeed(START_RUN, 10);
		addKeyframes(sprite, START_RUN, 0, 0, 4);

		sprite->setAnimationSpeed(RUN, 10);
		addKeyframes(sprite, RUN, 4, 0, 9);

		sprite->setAnimationSpeed(END_RUN, 8);
		addKeyframes(sprite, END_RUN, 3, 5, 4);

		sprite->setAnimationSpeed(WALK, 10);
		addKeyframes(sprite, WALK, 0, 11, 12);

		sprite->setAnimationSpeed(TURN, 10);
		addKeyframes(sprite, TURN, 7, 3, 8);

		sprite->setAnimationSpeed(RUN_TURN, 10);
		addKeyframes(sprite, RUN_TURN, 7, 5, 9);

		sprite->setAnimationSpeed(START_CROUCH, 10);
		addKeyframes(sprite, START_CROUCH, 8, 9, 3);

		sprite->setAnimationSpeed(CROUCH, 1);
		addKeyframes(sprite, CROUCH, 10, 9, 1);

		sprite->setAnimationSpeed(END_CROUCH, 10);
		addKeyframes(sprite, END_CROUCH, 11, 9, 10);

		//sprite->setAnimationSpeed(WALK_CROUCH, 10);
		//addKeyframes(sprite, WALK_CROUCH, 11, 9, 10);
		//START_CROUCH,
		//CROUCH,
		//END_CROUCH,
		//WALK_CROUCH

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	Player::changeState(STANDING_RIGHT);
}

void Player::changeState(PlayerState nextState) {
	switch (nextState)
	{
	case STANDING_LEFT:
		sprite->isFacingLeft = true;
		sprite->changeAnimation(STAND);
		break;
	case STANDING_RIGHT:
		sprite->isFacingLeft = false;
		sprite->changeAnimation(STAND);
		break;
	case WALKING_LEFT:
		sprite->changeAnimation(WALK);
		break;
	case WALKING_RIGHT:
		sprite->changeAnimation(WALK);
		break;
	case START_WALKING_RIGHT:
		sprite->isFacingLeft = false;
		changeState(WALKING_RIGHT);
		return;
	case START_WALKING_LEFT:
		sprite->isFacingLeft = true;
		changeState(WALKING_LEFT);
		return;
	case RUNNING_LEFT:
		sprite->isFacingLeft = true;
		sprite->changeAnimation(RUN);
		break;
	case RUNNING_RIGHT:
		sprite->isFacingLeft = false;
		sprite->changeAnimation(RUN);
		break;
	case START_RUNNING_RIGHT:
		sprite->isFacingLeft = false;
		sprite->changeAnimation(START_RUN);
		break;
	case START_RUNNING_LEFT:
		sprite->isFacingLeft = true;
		sprite->changeAnimation(START_RUN);
		break;
	case END_RUNNING_RIGHT:
		sprite->isFacingLeft = false;
		sprite->changeAnimation(END_RUN);
		break;
	case END_RUNNING_LEFT:
		sprite->isFacingLeft = true;
		sprite->changeAnimation(END_RUN);
		break;
	case TURNING_LEFT:
		sprite->isFacingLeft = true;
		sprite->changeAnimation(TURN);
		break;
	case TURNING_RIGHT:
		sprite->isFacingLeft = false;
		sprite->changeAnimation(TURN);
		break;
	case TURNING_RUNNING_LEFT:
		sprite->changeAnimation(RUN_TURN);
		break;
	case TURNING_RUNNING_RIGHT:
		sprite->changeAnimation(RUN_TURN);
		break;
	case CROUCHING_LEFT:
		sprite->changeAnimation(CROUCH);
		break;
	case CROUCHING_RIGHT:
		sprite->changeAnimation(CROUCH);
		break;
	case START_CROUCHING_LEFT:
		sprite->changeAnimation(START_CROUCH);
		break;
	case START_CROUCHING_RIGHT:
		sprite->changeAnimation(START_CROUCH);
		break;
	case END_CROUCHING_LEFT:
		sprite->changeAnimation(END_CROUCH);
		break;
	case END_CROUCHING_RIGHT:
		sprite->changeAnimation(END_CROUCH);
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
			if (Game::instance().getSpecialKey(GLUT_KEY_F1)) {
				changeState(START_WALKING_LEFT);
				break;
			}
			changeState(START_RUNNING_LEFT);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			changeState(TURNING_RIGHT);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			changeState(START_CROUCHING_LEFT);
		}
		break;
	case STANDING_RIGHT:
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			changeState(TURNING_LEFT);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_F1)) {
				changeState(START_WALKING_RIGHT);
				break;
			}
			changeState(START_RUNNING_RIGHT);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			changeState(START_CROUCHING_RIGHT);
		}
		break;
	case WALKING_LEFT:
		if (sprite->isAtEndingKeyframe()) {
			changeState(STANDING_LEFT);
		}
		move(true, PLAYER_WALK_SPEED);
		break;
	case WALKING_RIGHT:
		if (sprite->isAtEndingKeyframe()) {
			changeState(STANDING_RIGHT);
		}
		move(false, PLAYER_WALK_SPEED);
		break;
	case START_WALKING_RIGHT:
		break;
	case START_WALKING_LEFT:
		break;
	case RUNNING_LEFT:
		if (sprite->isAtEndingKeyframe()) {
			if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				changeState(END_RUNNING_LEFT);
				break;
			}
		}
		move(true, PLAYER_RUN_SPEED);
		break;
	case RUNNING_RIGHT:
		if (sprite->isAtEndingKeyframe()) {
			if (!Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				changeState(END_RUNNING_RIGHT);
				break;
			}
		}
		move(false, PLAYER_RUN_SPEED);
		break;
	case START_RUNNING_RIGHT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			if (!Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				changeState(END_RUNNING_RIGHT);
				break;
			}
			changeState(RUNNING_RIGHT);
		}
		move(false, PLAYER_WALK_SPEED);
		break;
	case START_RUNNING_LEFT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			// If we are not pressing the left key, there is no point on keeping
			// running
			if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				changeState(END_RUNNING_LEFT);
				break;
			}
			changeState(RUNNING_LEFT);
		}
		move(true, PLAYER_WALK_SPEED);
		break;
	case END_RUNNING_RIGHT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				changeState(TURNING_RUNNING_LEFT);
				break;
			}
			changeState(STANDING_RIGHT);
		}
		move(false, PLAYER_WALK_SPEED);
		break;
	case END_RUNNING_LEFT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				changeState(TURNING_RUNNING_RIGHT);
				break;
			}
			changeState(STANDING_LEFT);
		}
		move(true, PLAYER_WALK_SPEED);
		break;
	case TURNING_LEFT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STANDING_LEFT);
		}
		break;
	case TURNING_RIGHT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STANDING_RIGHT);
		}
		break;
	case TURNING_RUNNING_LEFT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				changeState(RUNNING_LEFT);
				break;
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				changeState(TURNING_RUNNING_RIGHT);
				break;
			}
			changeState(END_RUNNING_LEFT);
		}
		break;
	case TURNING_RUNNING_RIGHT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				changeState(TURNING_RUNNING_LEFT);
				break;
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				changeState(RUNNING_RIGHT);
				break;
			}
			changeState(END_RUNNING_RIGHT);
		}
		break;
	case CROUCHING_LEFT:
		if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			changeState(END_CROUCHING_LEFT);
		}
		break;
	case CROUCHING_RIGHT:
		if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			changeState(END_CROUCHING_RIGHT);
		}
		break;
	case START_CROUCHING_LEFT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(CROUCHING_LEFT);
		}
		break;
	case START_CROUCHING_RIGHT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(CROUCHING_RIGHT);
		}
		break;
	case END_CROUCHING_LEFT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STANDING_LEFT);
		}
		break;
	case END_CROUCHING_RIGHT:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STANDING_RIGHT);
		}
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
	
	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, 
					glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y), &posPlayer.y))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}

	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(
		float(tileMapDispl.x + posPlayer.x),
		float(tileMapDispl.y + posPlayer.y + Y_PLAYER_OFFSET)));
}

void Player::move(bool isMovingLeft, int speed) {

	int stride = isMovingLeft ? -speed : speed;
	posPlayer.x += stride;
	if (map->collisionMoveLeft(posPlayer, glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y)))
	{
		posPlayer.x -= stride;
		changeState(
			isMovingLeft ? STANDING_LEFT : STANDING_RIGHT
		);
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



