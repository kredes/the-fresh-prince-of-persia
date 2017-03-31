#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "UserInterface.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void updateCamera(int x, int y);
	void initShaders();

private:
	// From where the player came to the current screen
	// 0 -> left
	// 1 -> right
	// 2 -> up
	// 3 -> down
	int playerOriginX;
	TileMap *map;
	Player *player;
	UserInterface *ui;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

