#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

// This changes the initial offset of the tiles
#define SCREEN_X 0
#define SCREEN_Y 0

#define CLOSE_ENOUGH_TO_SCREEN_TRANS 10

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

void Scene::resetCamera() {
	updateCamera(0, 0);
	setText(0, 0);
	currentCameraPos = glm::vec2(0, 0);
	currentTime = 0.0f;
	player->reset();
}


void Scene::init(TileMap *_map, UserInterface *_ui, TextMap *_text, Player *_player, 
	bool isMainScene)
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
	if(isMainScene)
		setText(screensPassedX, screensPassedY);
}

void Scene::updateCamera(int x, int y) {
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	projection *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.f));
	currentCameraPos = glm::vec2(x, y);
	
	ui->updatePosition(
		x,
		y
	);

	text->updatePosition(
		x,
		y
	);
}

// Set the text for the scene (sx, sy)
void Scene::setText(int sx, int sy) {
	text->clearText();
	if (sx == 0 && sy == 0) {

		text->addText(
			1, "WELCOME TO THE TRIVIAL OF PERSIA", TextMap::Alignment::CENTER
		);
		text->addText(
			2, "YOUR SOUL WILL BE TESTED", TextMap::Alignment::CENTER
		);
		text->addText(
			3, "AND YOUR KNOWLEDGE ABOUT VIDEOGAMES", TextMap::Alignment::CENTER
		);
		text->addText(
			4, "PUT TO QUESTION!", TextMap::Alignment::CENTER
		);

		text->addText(
			6, "FIRST OF ALL", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "YOU HAVE TO JUMP THIS SMALL CHASM", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "I AM SURE IT'LL BE NO CHALLENGE TO YOU", TextMap::Alignment::CENTER
		);
		text->addText(
			9, "BRAVE ADVENTURER! ------------->", TextMap::Alignment::CENTER
		);
	}

	if (sx == 0 && sy == 1) {
		text->addText(
			1, "   NO NO NO NO        DID YOU REALLY", TextMap::Alignment::LEFT
		);
		text->addText(
			2, "   YOU ARE NOT        NOT MADE", TextMap::Alignment::LEFT
		);
		text->addText(
			3, "   SUPPOSED           THIS TINY", TextMap::Alignment::LEFT
		);
		text->addText(
			4, "   TO GO LEFT         JUMP?", TextMap::Alignment::LEFT
		);
		text->addText(
			5, "   OH MY GOD          OH MY GOD", TextMap::Alignment::LEFT
		);
		text->addText(
			14, "  YOU CAN RESTART BY PRESSING R", TextMap::Alignment::CENTER
		);
		text->addText(
			15, "  BY THE WAY", TextMap::Alignment::CENTER
		);
	}
	if (sx == 1 && sy == 0) {
		text->addText(
			1, "Okay, here comes the fist question:", TextMap::Alignment::CENTER
		);
		text->addText(
			3, "WHICH YEAR WAS", TextMap::Alignment::CENTER
		);
		text->addText(
			4, "PRINCE OF PERSIA RELEASED ON?", TextMap::Alignment::CENTER
		);
		text->addText(
			6, "            1989                  2003", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "             |                    |  ", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "             V                    V  ", TextMap::Alignment::CENTER
		);
	}
	if (sx == 1 && sy == 2) {
		text->addText(
			6, "HOLY S**T YOU ARE RIGHT!", TextMap::Alignment::CENTER
		);
		text->addText(
			10, "          NOT IF YOU LANDED", TextMap::Alignment::LEFT
		);
		text->addText(
			11, "       <--- HERE THOUGH --->", TextMap::Alignment::LEFT
		);
		text->addText(
			12, "               (LMAO)", TextMap::Alignment::LEFT
		);
	}
	if (sx == 1 && sy == 3) {
		text->addText(
			4, "OKAY SO", TextMap::Alignment::CENTER
		);
		text->addText(
			5, "THIS IS EMBARASSING", TextMap::Alignment::CENTER
		);
		text->addText(
			6, "YOU TECHNICALLY WON  ", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "THIS \"GAME\"", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "ALREADY", TextMap::Alignment::CENTER
		);
		text->addText(
			9, "SO YOU CAN", TextMap::Alignment::CENTER
		);
		text->addText(
			9, "PRESS R", TextMap::Alignment::CENTER
		);
		text->addText(
			10, "AND RESTART IT", TextMap::Alignment::CENTER
		);
		text->addText(
			11, "OR IDK YOU COULD GO PLAY OVERWATCH", TextMap::Alignment::CENTER
		);
		text->addText(
			12, "OR HEARTSTONE", TextMap::Alignment::CENTER
		);
		text->addText(
			13, "I'VE HEARD LEAGUE IS STILL GOOD", TextMap::Alignment::CENTER
		);
		text->addText(
			15, "A GAME BY", TextMap::Alignment::CENTER
		);
		text->addText(
			16, "VICTOR ALCAZAR", TextMap::Alignment::CENTER
		);
		text->addText(
			17, "ANDRES INSAURRALDE", TextMap::Alignment::CENTER
		);
		text->addText(
			18, "HIDEO KOJIMA (NOT REALLY)", TextMap::Alignment::CENTER
		);
	}
	if (sx == 2 && sy == 0) {
		text->addText(
			6, "HEY KID", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "GET BACK THERE", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "AND ANSWER MY QUESTION!", TextMap::Alignment::CENTER
		);
	}
	if (sx == 2 && sy == 1) {
		text->addText(
			6, "WHAT DID I TOLD YOU?", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "NOW YOU'RE HERE", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "FOREVER!", TextMap::Alignment::CENTER
		);
		text->addText(
			10, "(OR UNTILL YOU HIT R)", TextMap::Alignment::CENTER
		);
	}
	if (sx == 1 && sy == 1) {
		text->addText(
			6, "THAT WAS NOT QUITE", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "RIGHT", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "BUT STILL", TextMap::Alignment::CENTER
		);
		text->addText(
			9, "YOU CAN PLAY AROUND IN MY DUNGEON", TextMap::Alignment::CENTER
		);
		text->addText(
			10, "AS A PARTICIPATION TROPHY", TextMap::Alignment::CENTER
		);
		text->addText(
			11, "OR HIT R AND TRY AGAIN", TextMap::Alignment::CENTER
		);
	}
	if (sx == 2 && sy == 2) {
		text->addText(
			6, "DID YOU HAD FUN?", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "ARE THE SPIKES ENTERTAINING YOU?", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "YEAH I KNOW THEY ARE PRETTY", TextMap::Alignment::CENTER
		);
		text->addText(
			9, "COOL HUH", TextMap::Alignment::CENTER
		);
		text->addText(
			11, "REMEMBER TO HIT R TO PLAY AGAIN", TextMap::Alignment::CENTER
		);
	}
	if (sx == 0 && sy == 2) {
		text->addText(
			6, "KID!", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "WHAT THE HELL ARE YOU DOING HERE", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "YOU ARE NOT SUPPOSED TO BE HERE", TextMap::Alignment::CENTER
		);
		text->addText(
			9, "THIS IS WHERE I COME TO CRY :(", TextMap::Alignment::CENTER
		);
		text->addText(
			11, "PRESS R AND LEAVE ME ALONE!", TextMap::Alignment::CENTER
		);
	}
	if (sx == 0 && sy == 3) {
		text->addText(
			6, "THIS IS MY COOL ZONE", TextMap::Alignment::CENTER
		);
		text->addText(
			7, "ITS PRETTY COOL HUH", TextMap::Alignment::CENTER
		);
		text->addText(
			8, "YEAH I KNOW", TextMap::Alignment::CENTER
		);
		text->addText(
			9, "THIS GAME IS NOT VERY WELL MADE", TextMap::Alignment::CENTER
		);
		text->addText(
			10, "BUT WHAT ARE YOU GONNA DO", TextMap::Alignment::CENTER
		);
		text->addText(
			11, "ABOUT IT? SUE ME? HAH!", TextMap::Alignment::CENTER
		);
		text->addText(
			13, "(please don't sue me)", TextMap::Alignment::CENTER
		);
		text->addText(
			14, "(press R to start over...)", TextMap::Alignment::CENTER
		);
	}


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

	// We have to make this call here to trigger the prepareArrays() 
	// function.
	// DO NOT MOVE
	setText(screensPassedX, screensPassedY);

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
	if (text != NULL) {
		text->render();
	}
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


