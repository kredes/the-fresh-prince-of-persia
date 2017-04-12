#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "KeyListeners.h"
#include <Windows.h>
#include <mmsystem.h>

void Game::init()
{
	bPlay = true;
	
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	scene = new Scene();
	scene->initShaders();

	TileMap *map = new TileMap();
	map->init("levels/title.txt", glm::vec2(0, 0), scene->texProgram);

	TextMap *text = new TextMap();
	text->init(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_X, SCREEN_Y), scene->texProgram);
	text->addText(glm::vec2(9, 7), "A Piso Pedralbes game");
	text->addText(glm::vec2(10, 15), "Press space to start");
	text->addText(glm::vec2(0, 21), "i: Instructions");
	text->addText(glm::vec2(30, 21), "c: Credits");

	scene->init(map, NULL, text, NULL, false);
	scene->setKeyListener(new TitleSceneKeyListener());

	PlaySound(TEXT("songs\\main-theme.wav"), NULL, SND_FILENAME | SND_ASYNC);

}

bool Game::update(int deltaTime)
{
	scene->update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->render();
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
	scene->keyListener->onKeyPressed(scene, key);
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	scene->keyListener->onKeyReleased(scene, key);
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
	scene->keyListener->onSpecialKeyPressed(scene, key);
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
	scene->keyListener->onSpecialKeyReleased(scene, key);
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





