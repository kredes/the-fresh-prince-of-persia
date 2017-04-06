#include "Scene.h"
#include "Game.h"
#include <iostream>


class PlayingSceneKeyListener : public Scene::KeyListener {
	void onKeyPressed(Scene *scene, int key) {
		if (key == 'r') {
			// Set te player to the initial position
			scene->player->setPosition(
				glm::vec2(
					INIT_PLAYER_X_TILES * scene->map->getTileSizeX(),
					INIT_PLAYER_Y_TILES * scene->map->getTileSizeY()
				)
			);
		}
	}
	void onKeyReleased(Scene *scene, int key) {}
	void onSpecialKeyPressed(Scene *scene, int key) {}
	void onSpecialKeyReleased(Scene *scene, int key) {}
};

class TitleSceneKeyListener : public Scene::KeyListener {
	void onKeyPressed(Scene *scene, int key) {
		if (key == 32) { // Space
			std::cout << "Enter key pressed" << endl;
			
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

			Scene *newScene = new Scene();
			newScene->initShaders();

			TileMap *map = new TileMap();
			map->init("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), newScene->texProgram);

			UserInterface * ui = new UserInterface();
			ui->init(newScene->texProgram, SCREEN_WIDTH, SCREEN_HEIGHT, INIT_PLAYER_HEALTH);

			Player *player = new Player();
			player->init(glm::ivec2(SCREEN_X, SCREEN_Y), newScene->texProgram, INIT_PLAYER_HEALTH, ui);
			// Set the player's initial position
			player->setPosition(
				glm::vec2(
					INIT_PLAYER_X_TILES * map->getTileSizeX(),
					INIT_PLAYER_Y_TILES * map->getTileSizeY()
				)
			);
			player->setTileMap(map);

			newScene->init(map, ui, NULL, player);
			newScene->setKeyListener(new PlayingSceneKeyListener());

			delete Game::instance().scene;
			Game::instance().scene = newScene;
		}
	}
	void onKeyReleased(Scene *scene, int key) {}
	void onSpecialKeyPressed(Scene *scene, int key) {}
	void onSpecialKeyReleased(Scene *scene, int key) {}
};