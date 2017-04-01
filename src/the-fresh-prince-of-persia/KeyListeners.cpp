#include "Scene.h"

class TitleSceneKeyListener : public Scene::KeyListener {
	void onKeyPressed(Scene *scene, int key) {
		if (key == 32) // Enter
			scene->changeScene(NULL);
	}
	void onKeyReleased(Scene *scene, int key) {}
	void onSpecialKeyPressed(Scene *scene, int key) {}
	void onSpecialKeyReleased(Scene *scene, int key) {}
};