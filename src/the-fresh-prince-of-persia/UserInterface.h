#ifndef _UI_INCLUDE
#define _UI_INCLUDE

#include "Sprite.h"

class UserInterface
{

public:

	void init(ShaderProgram &shaderProgram, int initialHealth);
	bool update(int deltaTime);
	void render();

	void updateHealthPoints(int ammount);

private:
	int playerHealth;
	Texture spritesheet;
	Sprite* sprite;
};

#endif //_UI_INCLUDE