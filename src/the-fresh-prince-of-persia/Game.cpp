#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "KeyListeners.cpp"

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//scene.init();

	scene = Scene();
	scene.initShaders();
	ShaderProgram &texProgram = scene.texProgram;

	TileMap *map = new TileMap();
	map->init("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	TextMap *text = new TextMap();
	text->init(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	text->addText(glm::vec2(0, 2), "A Piso Pedralbes game");

	UserInterface * ui = new UserInterface();
	ui->init(texProgram, SCREEN_WIDTH, SCREEN_HEIGHT, INIT_PLAYER_HEALTH);

	Player *player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, INIT_PLAYER_HEALTH, ui);
	// Set the player's initial position
	player->setPosition(
		glm::vec2(
			INIT_PLAYER_X_TILES * map->getTileSizeX(),
			INIT_PLAYER_Y_TILES * map->getTileSizeY()
		)
	);
	player->setTileMap(map);

	scene.init(map, ui, text, player);
	scene.setKeyListener(new PlayingSceneKeyListener());
	//scene.setKeyListener(...)
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
	scene.keyListener->onKeyPressed(&scene, key);
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	scene.keyListener->onKeyReleased(&scene, key);
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
	scene.keyListener->onSpecialKeyPressed(&scene, key);
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
	scene.keyListener->onSpecialKeyReleased(&scene, key);
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





