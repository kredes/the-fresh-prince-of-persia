#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

// This changes the initial offset of the tiles
#define SCREEN_X 0
#define SCREEN_Y 0

#define CLOSE_ENOUGH_TO_SCREEN_TRANS 30

#define INIT_PLAYER_X_TILES 7
#define INIT_PLAYER_Y_TILES 1

#define INIT_PLAYER_HEALTH 3


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	ui = new UserInterface();
	// I am assuming the same shader can be used for the UI
	ui->init(texProgram, SCREEN_WIDTH, SCREEN_HEIGHT, INIT_PLAYER_HEALTH);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, INIT_PLAYER_HEALTH, ui);
	// Set the player's initial position
	player->setPosition(
		glm::vec2(
			INIT_PLAYER_X_TILES * map->getTileSizeX(),
			INIT_PLAYER_Y_TILES * map->getTileSizeY()
		)
	);
	playerOriginX = 0;
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void Scene::updateCamera(int x, int y) {
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	projection *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.f));
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	int playerX = player->getPosition().x;
	int playerY = player->getPosition().y;

	int screensPassedX = playerOriginX * SCREEN_WIDTH;

	if (playerX - screensPassedX > SCREEN_WIDTH - CLOSE_ENOUGH_TO_SCREEN_TRANS) {
		// This means we're moving right
		if (playerX / SCREEN_WIDTH > playerOriginX) {
			++playerOriginX;
			updateCamera(-SCREEN_WIDTH, 0);
			player->setPosition(glm::vec2(playerX + 20, playerY));
		}
	}
}

void Scene::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	//modelview *= glm::translate(glm::mat4(1.0f), glm::vec3(200, 0.f, 0.f));
	
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	ui->render();
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



