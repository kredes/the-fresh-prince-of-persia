#pragma once
#include "Scene.h"

class TitleSceneKeyListener : public Scene::KeyListener {
public:
	void onKeyPressed(Scene *scene, int key);
	void onKeyReleased(Scene *scene, int key);
	void onSpecialKeyPressed(Scene *scene, int key);
	void onSpecialKeyReleased(Scene *scene, int key);
};

class PlayingSceneKeyListener : public Scene::KeyListener {
public:
	void onKeyPressed(Scene *scene, int key);
	void onKeyReleased(Scene *scene, int key);
	void onSpecialKeyPressed(Scene *scene, int key);
	void onSpecialKeyReleased(Scene *scene, int key);
};

class InstructionsSceneKeyListener : public Scene::KeyListener {
public:
	void onKeyPressed(Scene *scene, int key);
	void onKeyReleased(Scene *scene, int key);
	void onSpecialKeyPressed(Scene *scene, int key);
	void onSpecialKeyReleased(Scene *scene, int key);
};

class CreditsSceneKeyListener : public Scene::KeyListener {
public:
	void onKeyPressed(Scene *scene, int key);
	void onKeyReleased(Scene *scene, int key);
	void onSpecialKeyPressed(Scene *scene, int key);
	void onSpecialKeyReleased(Scene *scene, int key);
};