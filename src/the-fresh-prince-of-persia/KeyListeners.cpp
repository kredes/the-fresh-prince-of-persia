#include "Scene.h"
#include <iostream>

class TitleSceneKeyListener : public Scene::KeyListener {
	void onKeyPressed(Scene *scene, int key) {
		if (key == 32) {// Enter

		}
	}
	void onKeyReleased(Scene *scene, int key) {}
	void onSpecialKeyPressed(Scene *scene, int key) {}
	void onSpecialKeyReleased(Scene *scene, int key) {}
};

class PlayingSceneKeyListener : public Scene::KeyListener {
	void onKeyPressed(Scene *scene, int key) {
		if (key == 'r') {// Enter
			std::cout << "r key pressed" << endl;
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