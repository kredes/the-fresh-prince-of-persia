#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "TextMap.h"
#include "Player.h"
#include "UserInterface.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	class KeyListener {
		virtual void onKeyPressed(int key) = 0;
		virtual void onKeyReleased(int key) = 0;
		virtual void onSpecialKeyPressed(int key) = 0;
		virtual void onSpecialKeyReleased(int key) = 0;
	};

	Scene();
	Scene(TileMap *_map, UserInterface *_ui, TextMap *_text, Player *_player);
	~Scene();

	void init();
	// If there is an scene change, it returns the new Scene to show.
	// Otherwise, returns itself.
	Scene* update(int deltaTime);
	void render();
	void changeScene(Scene *newScene);

	void setKeyListener(KeyListener *listener);
	KeyListener *keyListener;

private:
	void initShaders();

private:
	TileMap *map;
	TextMap *text;
	Player *player;
	UserInterface *ui;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Scene *nextScene;

};


#endif // _SCENE_INCLUDE

