#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "UserInterface.h"
#include <windows.h>
#include <mmsystem.h>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 64
#define JUMP_RUN_HEIGHT 24
#define FALL_STEP 4
#define Y_PLAYER_OFFSET 0
#define PLAYER_STEP 64

#define X_TILES 12
#define Y_TILES 18

#define PLAYER_RUN_SPEED 4
#define PLAYER_WALK_SPEED 1

#define PLAYER_SIZE_X 100
#define PLAYER_SIZE_Y 104

// Bounding box, for collisions
#define PLAYER_BB_SIZE_X 84
#define PLAYER_BB_SIZE_Y 20

#define INIT_HEALTH_POINTS 3;

// Sounds (time in milliseconds)
#define SOUND_TIME_FOOTSTEP 300

#define FALL_ACCEL 8

#define SHIFT_KEY 112

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
	WALK_CROUCH,

	JUMP_STAND,
	JUMP_RUN,
	JUMP,
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
		1.0f / (float)X_TILES * (float)xidx,
		1.0f / (float)Y_TILES * (float)yidx
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

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, 
	int intialHealth, UserInterface* _ui)
{
	bJumping = false;
	isMoving = false;
	healthPoints = INIT_HEALTH_POINTS;
	ui = _ui;
	// No movement
	movementDir = -1;
	initMovementPos = posPlayer.x;
	spritesheet.loadFromFile("sprites/kid/tileset.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(
		// Sprite size
		glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y),
		// The ammount to add to the texture coords
		glm::vec2(1.0f / (float)X_TILES, 1.0f / (float)Y_TILES),
		// The spritesheet
		&spritesheet,
		// The shader
		&shaderProgram);

	sprite->setNumberAnimations(20);

	sprite->setAnimationSpeed(STAND, 8);
	sprite->addKeyframe(STAND, getTexCoords(1, 1));

	sprite->setAnimationSpeed(START_RUN, 10);
	addKeyframes(sprite, START_RUN, 0, 0, 4);

	sprite->setAnimationSpeed(RUN, 12);
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

	sprite->setAnimationSpeed(WALK_CROUCH, 8);
	addKeyframes(sprite, WALK_CROUCH, 11, 9, 4);

	sprite->setAnimationSpeed(JUMP_STAND, 8);
	addKeyframes(sprite, JUMP_STAND, 7, 6, 12);

	sprite->setAnimationSpeed(JUMP_RUN, 8);
	addKeyframes(sprite, JUMP_RUN, 8, 2, 12);

	sprite->setAnimationSpeed(JUMP, 8);
	addKeyframes(sprite, JUMP, 1, 1, 19);

	
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	Player::changeState(STATE_FALLING);
	timeSinceSoundPlayed = 0;
	timeFalling = 0;
}

void Player::changeState(PlayerState nextState) {
	switch (nextState)
	{
	case STATE_STANDING:
		sprite->changeAnimation(STAND);
		break;
	case STATE_WALKING:
		sprite->changeAnimation(WALK);
		break;
	case STATE_START_WALKING:
		changeState(STATE_WALKING);
		return;
	case STATE_RUNNING:
		sprite->changeAnimation(RUN);
		break;
	case STATE_START_RUNNING:
		sprite->changeAnimation(START_RUN);
		break;
	case STATE_END_RUNNING:
		sprite->changeAnimation(END_RUN);
		break;
	case STATE_TURNING:
		sprite->changeAnimation(TURN);
		break;
	case STATE_TURNING_RUNNING:		
		sprite->changeAnimation(RUN_TURN);
		break;
	case STATE_CROUCHING:
		sprite->changeAnimation(CROUCH);
		break;
	case STATE_WALK_CROUCHING:
		sprite->changeAnimation(WALK_CROUCH);
		break;
	case STATE_START_CROUCHING:
		sprite->changeAnimation(START_CROUCH);
		break;
	case STATE_END_CROUCHING:
		sprite->changeAnimation(END_CROUCH);
		break;
	case STATE_JUMPING_STANDING:
		sprite->changeAnimation(JUMP_STAND, 11, 11);
		break;
	case STATE_FALLING:
		sprite->changeAnimation(JUMP_STAND, 11, 11);
		break;
	case STATE_START_JUMPING_STANDING:
		sprite->changeAnimation(JUMP_STAND, 0, 10);
		break;
	case STATE_JUMPING_RUNNING:
		startY = posPlayer.y;
		jumpAngle = 0;
		sprite->changeAnimation(JUMP_RUN, 6);
		break;
	case STATE_START_JUMPING_RUNNING:
		sprite->changeAnimation(JUMP_RUN, 0, 5);
		break;
	case STATE_JUMPING:
		sprite->changeAnimation(JUMP, 8, 12);
		break;
	case STATE_START_JUMPING:
		sprite->changeAnimation(JUMP, 0, 7);
		break;
	case STATE_END_JUMPING:
		sprite->changeAnimation(JUMP, 13, 18);
		break;
	default:
		cout << "changeState: State not recognized" << endl;
		break;
	}
	currentState = nextState;
	cout << "Player state changed to: " << getStateName(currentState) << endl;
}

bool inputToCurrentDirection(bool facingLeft) {
	return (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && facingLeft) ||
		(Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !facingLeft);
}

bool inputToOppositeDirection(bool facingLeft) {
	return (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !facingLeft) ||
		(Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && facingLeft);
}

void Player::update(int deltaTime)
{
	timeSinceSoundPlayed += deltaTime;
	bool endJump = false;
	float vel = 0;
	switch (currentState)
	{
	case STATE_STANDING:
		if (inputToCurrentDirection(sprite->isFacingLeft)) {
			if (Game::instance().getSpecialKey(GLUT_KEY_F1)) {
				changeState(STATE_WALKING);
				break;
			}
			changeState(STATE_START_RUNNING);
		}
		else if(inputToOppositeDirection(sprite->isFacingLeft)){
			sprite->isFacingLeft = !sprite->isFacingLeft;
			changeState(STATE_TURNING);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			changeState(STATE_START_CROUCHING);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			changeState(STATE_START_JUMPING_STANDING);
		}
		break;
	case STATE_WALKING:
		if (sprite->isAtEndingKeyframe()) {
			changeState(STATE_STANDING);
		}
		move(sprite->isFacingLeft, PLAYER_WALK_SPEED);
		break;
	case STATE_START_WALKING:
		break;
	case STATE_RUNNING:
		if (timeSinceSoundPlayed > SOUND_TIME_FOOTSTEP) {
			PlaySound(TEXT("sounds\\footstep.wav"), NULL, SND_FILENAME | SND_ASYNC);
			timeSinceSoundPlayed = 0;
		}
		if (sprite->isAtEndingKeyframe()) {
			if (!inputToCurrentDirection(sprite->isFacingLeft))
			{
				changeState(STATE_END_RUNNING);
				break;
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				changeState(STATE_START_JUMPING_RUNNING);
				break;
			}
		}
		move(sprite->isFacingLeft, PLAYER_RUN_SPEED);
		break;
	case STATE_START_RUNNING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			if (!inputToCurrentDirection(sprite->isFacingLeft))
			{
				changeState(STATE_END_RUNNING);
				break;
			}
			changeState(STATE_RUNNING);
		}
		move(sprite->isFacingLeft, PLAYER_WALK_SPEED);
		break;
	case STATE_END_RUNNING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			if (inputToOppositeDirection(sprite->isFacingLeft)) {
				changeState(STATE_TURNING_RUNNING);
				break;
			}
			changeState(STATE_STANDING);
		}
		move(sprite->isFacingLeft, PLAYER_WALK_SPEED);
		break;
	case STATE_TURNING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STATE_STANDING);
		}
		break;
	case STATE_TURNING_RUNNING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			sprite->isFacingLeft = !sprite->isFacingLeft;
			if (inputToCurrentDirection(sprite->isFacingLeft))
			{
				changeState(STATE_RUNNING);
				break;
			}
			if (inputToOppositeDirection(sprite->isFacingLeft))
			{
				sprite->isFacingLeft = !sprite->isFacingLeft;
				changeState(STATE_TURNING_RUNNING);
				break;
			}
			changeState(STATE_END_RUNNING);
		}
		break;
	case STATE_CROUCHING:
		if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			changeState(STATE_END_CROUCHING);
		}
		if (inputToCurrentDirection(sprite->isFacingLeft)) {
			changeState(STATE_WALK_CROUCHING);
		}
		break;
	case STATE_WALK_CROUCHING:
		if (sprite->isAtEndingKeyframe()) {
			if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				changeState(STATE_END_CROUCHING);
				break;
			}
			if (!inputToCurrentDirection(sprite->isFacingLeft)) {
				changeState(STATE_CROUCHING);
				break;
			}
		}
		move(sprite->isFacingLeft, PLAYER_WALK_SPEED);
		break;
	case STATE_START_CROUCHING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				changeState(STATE_END_CROUCHING);
				break;
			}
			changeState(STATE_CROUCHING);
		}
		break;
	case STATE_END_CROUCHING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STATE_STANDING);
		}
		break;
	case STATE_JUMPING_STANDING:
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			posPlayer.y = startY;
			changeState(STATE_FALLING);
		}
		else
		{
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posPlayer,
					glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y), &posPlayer.y))
				{
					changeState(STATE_START_CROUCHING);
				}
			}

		}
		break;
	case STATE_FALLING:
		vel = FALL_ACCEL * timeFalling / 1000.0f;
		posPlayer.y += vel * timeFalling / 1000.0f + FALL_STEP;
		timeFalling += deltaTime;
		if (map->collisionMoveDown(posPlayer,
			glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y),
			&posPlayer.y))
		{
			// If we have been falling for more than half a second
			if (timeFalling > 10) {
				PlaySound(TEXT("sounds\\land-harm.wav"), 
					NULL, 
					SND_FILENAME | SND_ASYNC);
				healthPoints = 0;
				ui->updateHealthPoints(healthPoints);
				
				if (healthPoints <= 0) {
					//glm::vec2 cameraPos = 
					//	Game::instance().scene->getCameraPos();
					// cameraPos.y += 120;
					glm::vec2 pos = glm::vec2(1, 4);
					
					//Game::instance().scene->text->addText(
					//	pos, "HAHAHAHAHHAHAHA"
					//
					//);
					//cout << "u died" << endl;
				}
			}
			else {
				PlaySound(TEXT("sounds\\land-soft.wav"), 
					NULL, 
					SND_FILENAME | SND_ASYNC);
			}
			timeFalling = 0;
			changeState(STATE_START_CROUCHING);
		}
		break;
	case STATE_START_JUMPING_STANDING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			jumpAngle = 0;
			startY = posPlayer.y;
			changeState(STATE_JUMPING_STANDING);
		}
		break;
	case STATE_JUMPING_RUNNING:
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			endJump = true;
		}
		else
		{
			posPlayer.y = int(startY - JUMP_RUN_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posPlayer,
					glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y), &posPlayer.y))
				{
					endJump = true;
				}
			}
		}
		if (sprite->timesLoopedCurrentAnimation > 0 || endJump) {
			// For posterior jump checks
			endJump = false;
			posPlayer.y = startY;
			if (inputToCurrentDirection(sprite->isFacingLeft))
			{
				changeState(STATE_RUNNING);
				break;
			}
			changeState(STATE_END_RUNNING);
			break;
		}
		move(sprite->isFacingLeft, PLAYER_RUN_SPEED);
		break;
	case STATE_START_JUMPING_RUNNING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STATE_JUMPING_RUNNING);
		}
		move(sprite->isFacingLeft, PLAYER_RUN_SPEED);
		break;
	case STATE_END_JUMPING_RUNNING:
		break;
	case STATE_JUMPING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STATE_END_JUMPING);
		}
		break;
	case STATE_START_JUMPING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STATE_JUMPING);
		}
		break;
	case STATE_END_JUMPING:
		if (sprite->timesLoopedCurrentAnimation > 0) {
			changeState(STATE_STANDING);
		}
		break;
	default:
		break;
	}
	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(
		float(tileMapDispl.x + posPlayer.x),
		float(tileMapDispl.y + posPlayer.y + Y_PLAYER_OFFSET)));
}

void Player::move(bool isMovingLeft, int speed) {

	int stride = isMovingLeft ? -speed : speed;
	posPlayer.x += stride;
	
	if (
		isMovingLeft && map->collisionMoveLeft(posPlayer, glm::ivec2(PLAYER_BB_SIZE_X, PLAYER_BB_SIZE_Y)) ||
		!isMovingLeft && map->collisionMoveRight(posPlayer, glm::ivec2(PLAYER_BB_SIZE_X, PLAYER_BB_SIZE_Y)))
	{
		posPlayer.x -= stride;
	}
	else if (!map->collisionMoveDown(posPlayer,
		glm::ivec2(PLAYER_BB_SIZE_X, PLAYER_BB_SIZE_Y), &posPlayer.y))
	{
		changeState(STATE_FALLING);
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

glm::vec2 Player::getPosition()
{
	return posPlayer;
}

void Player::addDamage(int ammount)
{
	healthPoints -= ammount;
	ui->updateHealthPoints(healthPoints);
	cout << "Player recieved " << ammount << " points of damage " <<
		healthPoints << " health points left." << endl;
}

string Player::getStateName(PlayerState state) {
	switch (state)
	{
	case STATE_STANDING:
		return "STATE_STANDING";
	case STATE_WALKING:
		return "STATE_WALKING";
	case STATE_START_WALKING:
		return "STATE_START_WALKING_RIGHT";
	case STATE_RUNNING:
		return "STATE_RUNNING";
	case STATE_START_RUNNING:
		return "STATE_START_RUNNING_RIGHT";
	case STATE_END_RUNNING:
		return "STATE_END_RUNNING_RIGHT";
	case STATE_TURNING:
		return "STATE_TURNING";
	case STATE_TURNING_RUNNING:
		return "STATE_TURNING_RUNNING";
	case STATE_CROUCHING:
		return "STATE_CROUCHING";
	case STATE_START_CROUCHING:
		return "STATE_START_CROUCHING";
	case STATE_END_CROUCHING:
		return "STATE_END_CROUCHING";
	case STATE_JUMPING_STANDING:
		return "STATE_JUMPING";
	case STATE_START_JUMPING_STANDING:
		return "STATE_START_JUMPING";
	case STATE_START_JUMPING:
		return "STATE_END_JUMPING";
	case STATE_JUMPING_RUNNING:
		return "STATE_JUMPING_RUNNING";
	case STATE_START_JUMPING_RUNNING:
		return "STATE_START_JUMPING_RUNNING";
	case STATE_END_JUMPING_RUNNING:
		return "STATE_END_JUMPING_RUNNING";
	case STATE_WALK_CROUCHING:
		return "STATE_WALK_CROUCHING";
	case STATE_FALLING:
		return "STATE_FALLING";
	default:
		return "STATE_UNKNOWN";
	}
}



