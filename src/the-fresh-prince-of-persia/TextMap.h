#ifndef _TEXT_MAP_INCLUDE
#define _TEXT_MAP_INCLUDE


#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"


// Similar to TileMap. This class renders text from a tileset of characters.
// Render it after rendering the TileMap or you won't see shit.


class TextMap
{

public:
	enum Alignment {
		CENTER, LEFT, RIGHT
	};

	// Tile maps can only be created inside an OpenGL context
	void init(const glm::vec2 screenSize, const glm::vec2 &minCoords, ShaderProgram &program);

	TextMap();
	~TextMap();

	void render() const;
	void free();

	int getTileSizeX() const { return tileSizeX; }
	int getTileSizeY() const { return tileSizeY; }

	void addText(glm::vec2 coords, string text);
	void addText(int y, string text, Alignment alignment);

private:
	void prepareMap(const glm::vec2 screenSize);
	void prepareArrays();
	void initCharMap();

private:
	GLuint vao, vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSizeX, tileSizeY, blockSizeX, blockSizeY;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	vector<int> m;
	map<char, int> CHARS;

	glm::vec2 minCoords;
	ShaderProgram program;
};


#endif // _TEXT_MAP_INCLUDE


