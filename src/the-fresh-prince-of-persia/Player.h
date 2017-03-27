#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void move(bool isMovingfLeft, int speed);
	void render();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	// For the tile-based movement
	bool isMoving;
	int movementDir;
	int initMovementPos;

	enum PlayerState
	{
		// STANDING
		STANDING_LEFT,
		STANDING_RIGHT,

		// WALKING
		WALKING_LEFT,
		WALKING_RIGHT,

		START_WALKING_RIGHT,
		START_WALKING_LEFT,

		// RUNNING
		// This is the default movement action
		RUNNING_LEFT,
		RUNNING_RIGHT,

		START_RUNNING_RIGHT,
		START_RUNNING_LEFT,

		END_RUNNING_RIGHT,
		END_RUNNING_LEFT,

		// TURNING
		TURNING_LEFT,
		TURNING_RIGHT,

		TURNING_RUNNING_LEFT,
		TURNING_RUNNING_RIGHT,

		// CROUCHING
		CROUCHING_LEFT,
		CROUCHING_RIGHT,

		WALK_CROUCHING_LEFT,
		WALK_CROUCHING_RIGHT,

		START_CROUCHING_LEFT,
		START_CROUCHING_RIGHT,

		END_CROUCHING_LEFT,
		END_CROUCHING_RIGHT,

		// JUMPING
		JUMPING_LEFT,
		JUMPING_RIGHT,

		START_JUMPING_LEFT,
		START_JUMPING_RIGHT,

		END_JUMPING_LEFT,
		END_JUMPING_RIGHT,

		// JUMPING RUNNING
		JUMPING_RUNNING_LEFT,
		JUMPING_RUNNING_RIGHT,

		START_JUMPING_RUNNING_LEFT,
		START_JUMPING_RUNNING_RIGHT,

		END_JUMPING_RUNNING_LEFT,
		END_JUMPING_RUNNING_RIGHT
	};

	string getStateName(Player::PlayerState state);
	void Player::changeState(Player::PlayerState nextState);
	Player::PlayerState currentState;
};


#endif // _PLAYER_INCLUDE


