#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "UserInterface.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Player
{
public:
	
	void init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, 
		int intialHealth, UserInterface* ui);
	void update(int deltaTime);
	void move(bool isMovingfLeft, int speed);
	
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();

	void addDamage(int ammount);
	
private:
	int healthPoints;
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	UserInterface *ui;
	TileMap *map;
	// For the tile-based movement
	bool isMoving;
	int movementDir;
	int initMovementPos;

	float timeFalling;
	float timeSinceSoundPlayed;

	enum PlayerState
	{
		// STANDING
		STATE_STANDING,

		// WALKING
		STATE_WALKING,
		STATE_START_WALKING,

		// RUNNING
		// This is the default movement action
		STATE_RUNNING,
		STATE_START_RUNNING,
		STATE_END_RUNNING,

		// TURNING
		STATE_TURNING,
		STATE_TURNING_RUNNING,

		// CROUCHING
		STATE_CROUCHING,
		STATE_WALK_CROUCHING,
		STATE_START_CROUCHING,
		STATE_END_CROUCHING,

		// JUMPING (STANDING)
		STATE_JUMPING_STANDING,
		STATE_START_JUMPING_STANDING,
		
		// FALLING
		STATE_FALLING,

		// JUMPING RUNNING
		STATE_JUMPING_RUNNING,
		STATE_START_JUMPING_RUNNING,
		STATE_END_JUMPING_RUNNING,

		// JUMPING STANDING
		STATE_JUMPING,
		STATE_START_JUMPING,
		STATE_END_JUMPING,
	};

	string getStateName(Player::PlayerState state);
	void Player::changeState(Player::PlayerState nextState);
	Player::PlayerState currentState;
};


#endif // _PLAYER_INCLUDE


