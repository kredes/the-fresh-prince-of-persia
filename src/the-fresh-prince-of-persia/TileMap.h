#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"

typedef vector<vector<int>> Map;


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	TileMap();
	~TileMap();

	void init(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	void render() const;
	void free();
	
	int getTileSizeX() const { return tileSizeX; }
	int getTileSizeY() const { return tileSizeY; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	vector<GLuint> vaos;
	vector<GLuint> vbos;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSizeX, tileSizeY, blockSizeX, blockSizeY, nbLayers;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	Map map;
};


#endif // _TILE_MAP_INCLUDE


