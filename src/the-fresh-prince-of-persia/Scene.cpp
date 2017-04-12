#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

// This changes the initial offset of the tiles
#define SCREEN_X 0
#define SCREEN_Y 0

#define CLOSE_ENOUGH_TO_SCREEN_TRANS 50

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
	
	/*player->setPosition(
		glm::vec2(
			INIT_PLAYER_X_TILES * map->getTileSizeX(),
			INIT_PLAYER_Y_TILES * map->getTileSizeY()
		)
	);*/
	screensPassedX = 0;
	screensPassedY = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentCameraPos = glm::vec2(0, 0);
	currentTime = 0.0f;
}

void Scene::updateCamera(int x, int y) {
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	projection *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.f));
	currentCameraPos = glm::vec2(x, y);
}

// direction -> direction to where we want to go
// 0 -> left
// 1 -> right
// 2 -> up
// 3 -> down
void Scene::changeScreen(int direction) {

	int playerX = player->getPosition().x;
	int playerY = player->getPosition().y;
	
	switch (direction)
	{
	case 0:
		--screensPassedX;
		playerX -= CLOSE_ENOUGH_TO_SCREEN_TRANS * 2;
		break;
	case 1:
		++screensPassedX;
		playerX += CLOSE_ENOUGH_TO_SCREEN_TRANS * 2;
		break;
	case 2:
		++screensPassedY;
		playerY += CLOSE_ENOUGH_TO_SCREEN_TRANS * 2;
		break;
	case 3:
		--screensPassedY;
		playerY -= CLOSE_ENOUGH_TO_SCREEN_TRANS * 2;
		break;
	default:
		break;
	}

	player->setPosition(
		glm::vec2(playerX, playerY)
	);

	// I am changing the sign of this because the code does not work very well
	// with negative screenPassedX's
	updateCamera(
		-SCREEN_WIDTH * screensPassedX, 
		-SCREEN_HEIGHT * screensPassedY
	);
	ui->updatePosition(
		-SCREEN_WIDTH * screensPassedX,
		-SCREEN_HEIGHT * screensPassedY
	);
	cout << "Changing scene to: " << screensPassedX << ", " << screensPassedY << endl;
	cout << "With direction: " << direction << endl;
}

Scene* Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (!player)
	{
		return this;
	}
	player->update(deltaTime);

	int playerX = player->getPosition().x;
	int playerY = player->getPosition().y;
	// Conceptually this is how much screen space the player has
	// covered if we only take into account the "screens"
	// that the player has gone through
	int screensPassedXWidth = screensPassedX * SCREEN_WIDTH;
	int screensPassedYHeight = screensPassedY * SCREEN_HEIGHT;

	if (playerX - screensPassedXWidth < CLOSE_ENOUGH_TO_SCREEN_TRANS)
	{
		// This means we're moving left
		changeScreen(0);
	}
	else if (playerX - screensPassedXWidth > 
		SCREEN_WIDTH - CLOSE_ENOUGH_TO_SCREEN_TRANS) 
	{
		// This means we're moving right
		changeScreen(1);
	}
	else if (playerY - screensPassedYHeight > 
		SCREEN_HEIGHT - CLOSE_ENOUGH_TO_SCREEN_TRANS)
	{
		// This means we're moving up
		changeScreen(2);
	}
	else if (playerY - screensPassedYHeight < CLOSE_ENOUGH_TO_SCREEN_TRANS)
	{
		// This means we're moving down
		changeScreen(3);
	}
	return this;
}

glm::vec2 Scene::getCameraPos()
{
	return currentCameraPos;
}

void Scene::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);

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


