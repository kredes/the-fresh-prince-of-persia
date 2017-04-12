#ifndef _UI_INCLUDE
#define _UI_INCLUDE

#include "Sprite.h"

class UserInterface
{

public:

	void init(ShaderProgram &shaderProgram, int screen_width,
		int screen_height, int initialHealth);
	bool update(int deltaTime);
	void render();
	void updatePosition(int x, int y);
	void updateHealthPoints(int ammount);
private:
	int initialHealth;
	int playerHealth;
	Texture health_points_spritesheet;
	Texture ui_back_spritesheet;
	vector <Sprite*> health_point_sprites;
	vector<glm::vec2> sprites_positions;
	Sprite* ui_back_sprite;
};

#endif //_UI_INCLUDE