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
	keyListener = NULL;
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
	if (ui != NULL)
		delete ui;
	if (keyListener != NULL)
		delete keyListener;
}


void Scene::init(TileMap *_map, UserInterface *_ui, TextMap *_text, Player *_player)
{
	map = _map;
	ui = _ui;
	text = _text;
	player = _player;
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

Scene* Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (player) player->update(deltaTime);

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


