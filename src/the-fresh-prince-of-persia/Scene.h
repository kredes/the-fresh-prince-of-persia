#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "TextMap.h"
#include "Player.h"
#include "UserInterface.h"


#define SCREEN_X 0
#define SCREEN_Y -120

#define INIT_PLAYER_X_TILES 10
#define INIT_PLAYER_Y_TILES 4

#define INIT_PLAYER_HEALTH 3


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	class KeyListener {
	public:
		virtual void onKeyPressed(Scene *scene, int key) = 0;
		virtual void onKeyReleased(Scene *scene, int key) = 0;
		virtual void onSpecialKeyPressed(Scene *scene, int key) = 0;
		virtual void onSpecialKeyReleased(Scene *scene, int key) = 0;
	};

	Scene();
	Scene(TileMap *_map, UserInterface *_ui, TextMap *_text, Player *_player);
	~Scene();

	void init(TileMap * _map, UserInterface * _ui, TextMap * _text, Player * _player, bool isMainScene);
	// If there is an scene change, it returns the new Scene to show.
	// Otherwise, returns itself.
	Scene* update(int deltaTime);
	glm::vec2 getCameraPos();
	glm::vec2 currentCameraPos;
	void render();
	void initShaders();
	void resetCamera();

	void setKeyListener(KeyListener *listener);
	KeyListener *keyListener;
	ShaderProgram texProgram;

	TileMap *map;
	TextMap *text;
	Player *player;
	UserInterface *ui;

private:
	float currentTime;
	glm::mat4 projection;
	void updateCamera(int x, int y);
	void setText(int sx, int sy);
	void changeScreen(int direction);
	int screensPassedX;
	int screensPassedY;
};


#endif // _SCENE_INCLUDE

