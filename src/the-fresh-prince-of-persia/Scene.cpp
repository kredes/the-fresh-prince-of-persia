#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

// This changes the initial offset of the tiles
//#define SCREEN_X -64
//#define SCREEN_Y -24


Scene::Scene() {
	map = NULL;
	ui = NULL;
	text = NULL;
	player = NULL;
}

Scene::Scene(TileMap *_map, UserInterface *_ui, TextMap *_text, Player *_player)
{
	map = _map;
	ui = _ui;
	text = _text;
	player = _player;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (text != NULL)
		delete text;
}


void Scene::init(TileMap *_map, UserInterface *_ui, TextMap *_text, Player *_player)
{
	map = _map;
	ui = _ui;
	text = _text;
	player = _player;
	/*
	initShaders();
	if (map) map->init("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	if (text) {
		text->init(glm::vec2(1280, 720), glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		text->addText(glm::vec2(0, 2), "A Piso Pedralbes game");
	}
	
	// I am assuming the same shader can be used for the UI
	if (ui) ui->init(texProgram, SCREEN_WIDTH, SCREEN_HEIGHT, INIT_PLAYER_HEALTH);
	if (player) {
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, INIT_PLAYER_HEALTH, ui);
		// Set the player's initial position
		player->setPosition(
			glm::vec2(
				INIT_PLAYER_X_TILES * map->getTileSizeX(),
				INIT_PLAYER_Y_TILES * map->getTileSizeY()
			)
		);
		player->setTileMap(map);
	}
	*/
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

Scene* Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	if (nextScene != NULL) return nextScene;

	return this;
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (map != NULL) map->render();
	if (text != NULL) text->render();
	if (player != NULL) player->render();
	if (ui != NULL) ui->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::setKeyListener(KeyListener *listener) {
	keyListener = listener;
}

void Scene::changeScene(Scene *newScene) {
	nextScene = newScene;
}



