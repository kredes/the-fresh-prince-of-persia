#include "Scene.h"
#include "Game.h"
#include "KeyListeners.h"
#include <iostream>


/* PlayingSceneKeyListener */
void PlayingSceneKeyListener::onKeyPressed(Scene *scene, int key) {
	if (key == 'r') {
		// Set te player to the initial position
		scene->player->setPosition(
			glm::vec2(
				INIT_PLAYER_X_TILES * scene->map->getTileSizeX(),
				INIT_PLAYER_Y_TILES * scene->map->getTileSizeY()
			)
		);
	}
}
void PlayingSceneKeyListener::onKeyReleased(Scene *scene, int key) {}
void PlayingSceneKeyListener::onSpecialKeyPressed(Scene *scene, int key) {}
void PlayingSceneKeyListener::onSpecialKeyReleased(Scene *scene, int key) {}



/* InstructionsSceneKeyListener */
void InstructionsSceneKeyListener::onKeyPressed(Scene *scene, int key) {
	if (key == 'b') {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		Scene *newScene = new Scene();
		newScene->initShaders();

		TileMap *map = new TileMap();
		map->init("levels/title.txt", glm::vec2(0, 0), newScene->texProgram);

		TextMap *text = new TextMap();
		text->init(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_X, SCREEN_Y), newScene->texProgram);
		text->addText(glm::vec2(9, 7), "A Piso Pedralbes game");
		text->addText(glm::vec2(10, 15), "Press space to start");
		text->addText(glm::vec2(0, 21), "i: Instructions");
		text->addText(glm::vec2(30, 21), "c: Credits");

		newScene->init(map, NULL, text, NULL);
		newScene->setKeyListener(new TitleSceneKeyListener());
		
		delete Game::instance().scene;
		Game::instance().scene = newScene;
	}
}
void InstructionsSceneKeyListener::onKeyReleased(Scene *scene, int key) {}
void InstructionsSceneKeyListener::onSpecialKeyPressed(Scene *scene, int key) {}
void InstructionsSceneKeyListener::onSpecialKeyReleased(Scene *scene, int key) {}


/* TitleSceneKeyListener */
void TitleSceneKeyListener::onKeyPressed(Scene *scene, int key) {
	if (key == 32) { // Space
		std::cout << "Enter key pressed" << endl;
			
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		Scene *newScene = new Scene();
		newScene->initShaders();

		TileMap *map = new TileMap();
		map->init("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), newScene->texProgram);

		UserInterface * ui = new UserInterface();
		ui->init(newScene->texProgram, SCREEN_WIDTH, SCREEN_HEIGHT, INIT_PLAYER_HEALTH);

		Player *player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), newScene->texProgram, INIT_PLAYER_HEALTH, ui);
		// Set the player's initial position
		player->setPosition(
			glm::vec2(
				INIT_PLAYER_X_TILES * map->getTileSizeX(),
				INIT_PLAYER_Y_TILES * map->getTileSizeY()
			)
		);
		player->setTileMap(map);

		newScene->init(map, ui, NULL, player);
		newScene->setKeyListener(new PlayingSceneKeyListener());

		delete Game::instance().scene;
		Game::instance().scene = newScene;
	} 
	else if (key == 'i') {
		Scene *newScene = new Scene();
		newScene->initShaders();

		TileMap *map = new TileMap();
		map->init("levels/instructions.txt", glm::vec2(0, 0), newScene->texProgram);

		TextMap *text = new TextMap();
		text->init(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_X, SCREEN_Y), scene->texProgram);

		text->addText(1, "INSTRUCTIONS", TextMap::Alignment::CENTER);
		text->addText(2, "----------------", TextMap::Alignment::CENTER);
		text->addText(4, "Left arrow: Move left", TextMap::Alignment::CENTER);
		text->addText(5, "Right arrow: Move right", TextMap::Alignment::CENTER);
		text->addText(6, "Up arrow: Jump", TextMap::Alignment::CENTER);
		text->addText(7, "Down arrow: Crouch", TextMap::Alignment::CENTER);
		text->addText(8, "F1 + Left: Walk left", TextMap::Alignment::CENTER);
		text->addText(9, "F1 + Right: Walk right", TextMap::Alignment::CENTER);
		text->addText(10, "r: Restart", TextMap::Alignment::CENTER);
		text->addText(21, "b: Back", TextMap::Alignment::LEFT);

		newScene->init(map, NULL, text, NULL);
		newScene->setKeyListener(new InstructionsSceneKeyListener());

		delete Game::instance().scene;
		Game::instance().scene = newScene;
	} 
	else if (key == 'c') {
		Scene *newScene = new Scene();
		newScene->initShaders();

		TileMap *map = new TileMap();
		map->init("levels/credits.txt", glm::vec2(0, 0), newScene->texProgram);

		TextMap *text = new TextMap();
		text->init(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_X, SCREEN_Y), scene->texProgram);
		
		text->addText(1, "CREDITS", TextMap::Alignment::CENTER);
		text->addText(2, "-----------", TextMap::Alignment::CENTER);
		text->addText(4, "Victor Alc�zar", TextMap::Alignment::CENTER);
		text->addText(5, "&", TextMap::Alignment::CENTER);
		text->addText(6, "Andr�s Insaurralde", TextMap::Alignment::CENTER);
		text->addText(8, "Videogames. FIB.", TextMap::Alignment::CENTER);
		text->addText(9, "Q2 2017", TextMap::Alignment::CENTER);
		text->addText(21, "b: Back", TextMap::Alignment::LEFT);

		newScene->init(map, NULL, text, NULL);
		newScene->setKeyListener(new CreditsSceneKeyListener());

		delete Game::instance().scene;
		Game::instance().scene = newScene;
	}
}
void TitleSceneKeyListener::onKeyReleased(Scene *scene, int key) {}
void TitleSceneKeyListener::onSpecialKeyPressed(Scene *scene, int key) {}
void TitleSceneKeyListener::onSpecialKeyReleased(Scene *scene, int key) {}


/* CreditsSceneKeyListener */
void CreditsSceneKeyListener::onKeyPressed(Scene *scene, int key) {
	if (key == 'b') {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		Scene *newScene = new Scene();
		newScene->initShaders();

		TileMap *map = new TileMap();
		map->init("levels/title.txt", glm::vec2(0, 0), newScene->texProgram);

		TextMap *text = new TextMap();
		text->init(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_X, SCREEN_Y), newScene->texProgram);
		
		text->addText(7, "A Piso Pedralbes game", TextMap::Alignment::CENTER);
		text->addText(15, "Press space to start", TextMap::Alignment::CENTER);
		text->addText(21, "i: Instructions", TextMap::Alignment::LEFT);
		text->addText(21, "c: Credits", TextMap::Alignment::RIGHT);

		newScene->init(map, NULL, text, NULL);
		newScene->setKeyListener(new TitleSceneKeyListener());

		delete Game::instance().scene;
		Game::instance().scene = newScene;
	}
}
void CreditsSceneKeyListener::onKeyReleased(Scene *scene, int key) {}
void CreditsSceneKeyListener::onSpecialKeyPressed(Scene *scene, int key) {}
void CreditsSceneKeyListener::onSpecialKeyReleased(Scene *scene, int key) {}