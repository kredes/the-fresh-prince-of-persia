#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <GL/glut.h>

using namespace std;

#define CLOSE_ENOUGH_Y 4

#define LAYER_BACKGROUND 0
#define LAYER_WALL_DEPTH 1
#define LAYER_FLOOR 2
#define LAYER_WALL 3
#define LAYER_OVER_FLOOR 4
#define LAYER_WALK_BEHIND 5

string layerName(int k) {
	switch (k)
	{
	case 0: return "LAYER_BACKOUND";
	case 1: return "LAYER_WALL_DEPTH";
	case 2: return "LAYER_FLOOR";
	case 3: return "LAYER_WALL";
	case 4: return "LAYER_OVER_FLOOR";
	case 5: return "LAYER_WALK_BEHIND";
	default:
		return "UNKNOWN";
	}
}

void TileMap::init(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	vaos = vector<GLuint>(nbLayers);
	vbos = vector<GLuint>(nbLayers);
	prepareArrays(minCoords, program);
}


TileMap::TileMap()
{
}

TileMap::~TileMap()
{
	/*if (map != NULL)
		delete map;*/
}


void TileMap::render() const
{

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	
	for (int k = 0; k < map.size(); ++k) {

		if (vaos[k] <= 0) continue;

		glBindVertexArray(vaos[k]);
		glEnableVertexAttribArray(posLocation);
		glEnableVertexAttribArray(texCoordLocation);
		glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
		glDisable(GL_TEXTURE_2D);
	}
}

void TileMap::free()
{
	for (int k = 0; k < map.size(); ++k) glDeleteBuffers(1, &vbos[k]);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;
	// We open the file containing the level, in plain text
	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSizeX >> tileSizeY >> blockSizeX >> blockSizeY;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	getline(fin, line);
	sstream.str(line);
	sstream >> nbLayers;
	
	map = Map(nbLayers, vector<int>(mapSize.x * mapSize.y));
	for (int k = 0; k < map.size(); ++k) {
		vector<int> &layer = map[k];
		for (int j = 0; j < mapSize.y; j++) {
			getline(fin, line);
			sstream = stringstream(line);	// Autismo. Arriba funciona sin instanciar de nuevo, pero aquí no.

			for (int i = 0; i < mapSize.x; i++) {
				sstream >> tile;
				layer[j*mapSize.x + i] = tile + 1;	// Tiled comienza en 0
				sstream.ignore();	// Comma
			}
			/* No sé que coño es esto, ya estaba aquí antes
			fin.get(tile);
	#ifndef _WIN32
			fin.get(tile);
	#endif
	*/
		}
		getline(fin, line);	// Skip the dashes

	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	halfTexel = glm::vec2(
		0.5f / tilesheet.width(),
		0.5f / tilesheet.height());
	for (int k = 0; k < map.size(); k++) {
		vector<int> &layer = map[k];
		for (int j = 0; j < mapSize.y; j++) {
			for (int i = 0; i < mapSize.x; i++) {
				tile = layer[j * mapSize.x + i];
				if (tile != 0) {
					// Non-empty tile
					nTiles++;
					posTile = glm::vec2(minCoords.x + i * tileSizeX, minCoords.y + j * tileSizeY);
					// Generating texture coordinates
					texCoordTile[0] = glm::vec2(
						float((tile - 1) % tilesheetSize.x) / tilesheetSize.x,
						float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
					texCoordTile[1] = texCoordTile[0] + tileTexSize;
					//texCoordTile[0] += halfTexel;
					texCoordTile[1] -= halfTexel;
					// First triangle
					// First vertex
					vertices.push_back(posTile.x);
					vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[0].x);
					vertices.push_back(texCoordTile[0].y);
					// Second vertex
					vertices.push_back(posTile.x + blockSizeX);
					vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[1].x);
					vertices.push_back(texCoordTile[0].y);
					// Third vertex
					vertices.push_back(posTile.x + blockSizeX);
					vertices.push_back(posTile.y + blockSizeY);
					vertices.push_back(texCoordTile[1].x);
					vertices.push_back(texCoordTile[1].y);
					// Second triangle
					// First vertex
					vertices.push_back(posTile.x);
					vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[0].x);
					vertices.push_back(texCoordTile[0].y);
					// Second vertex
					vertices.push_back(posTile.x + blockSizeX);
					vertices.push_back(posTile.y + blockSizeY);
					vertices.push_back(texCoordTile[1].x);
					vertices.push_back(texCoordTile[1].y);
					// Third vertex
					vertices.push_back(posTile.x);
					vertices.push_back(posTile.y + blockSizeY);
					vertices.push_back(texCoordTile[0].x);
					vertices.push_back(texCoordTile[1].y);
				}
			}
		}

		if (nTiles == 0) continue;

		glGenVertexArrays(1, &vaos[k]);
		glBindVertexArray(vaos[k]);
		glGenBuffers(1, &vbos[k]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[k]);
		glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
		texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
		
		nTiles = 0;
		vertices.clear();
	}
}

bool isCollisionTile(int tileType, int layer) {
	switch (layer) {
	case LAYER_FLOOR:
	case LAYER_WALL:
		return tileType > 0 && tileType <= 25;	// Hardcoded magic
	case LAYER_WALL_DEPTH:
		return tileType > 0 && tileType == 24;
	case LAYER_BACKGROUND:
	case LAYER_WALK_BEHIND:
	case LAYER_OVER_FLOOR:
		return false;
	default:
		return false;
	}
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1, tile;

	x = pos.x / tileSizeX;
	y0 = pos.y / tileSizeY;
	y1 = (pos.y + size.y - 1) / tileSizeY;
	for (int k = 0; k < map.size(); ++k) {
		const vector<int> &layer = map[k];
		
		tile = layer[y1*mapSize.x + x];
		if (k != LAYER_FLOOR && k != LAYER_WALL && isCollisionTile(tile, k)) {
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, x0, y0, y1, tile;

	x = (pos.x + size.x - 1) / tileSizeX;
	x0 = pos.x / tileSizeX;
	y0 = pos.y / tileSizeY;
	y1 = (pos.y + size.y - 1) / tileSizeY;
	for (int k = 0; k < map.size(); ++k) {
		const vector<int> &layer = map[k];

		tile = layer[y1*mapSize.x + x0 + 1];
		

		if (k != LAYER_FLOOR && isCollisionTile(tile, k)) {
			if (k == LAYER_WALL) {
				int tileBelow = map[LAYER_WALL_DEPTH][(y1 + 1)*mapSize.x + x0 + 1];
				return tileBelow == 24;
			}
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	/*
	int x0, y0, x1, y, tile;

	x0 = (pos.x / tileSizeX) + 1;
	y0 = (pos.y / tileSizeY) + 1;
	x1 = (pos.x + size.x - 1) / tileSizeX;
	y = (pos.y + size.y - 1) / tileSizeY;
	*/

	
	int x, y, tile;
	x = pos.x / tileSizeX;
	y = pos.y / tileSizeY;
	
	//cout << "Checking collision on coordinates (" << x << ", " << y << ")" << endl;

	for (int k = 0; k < map.size(); ++k) {
		const vector<int> &layer = map[k];
		tile = layer[y*mapSize.x + x];
		if (isCollisionTile(tile, k)) {
			//cout << "Down collision on layer " << layerName(k) << " with tile " << tile << endl;
			return true;
		}
	}
	// Player's position on the screen
	// based only on tile belongingness
	int realPosition = tileSizeY * y;
	// If the y pos of the player minus its tile position plus its sprite size
	// is less than a certain ammount
	if (*posY - realPosition + size.y <= CLOSE_ENOUGH_Y) {
		*posY = (y + 1)*tileSizeY - 100;
	}
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const {
	int x, y, tile;
	x = pos.x / tileSizeX;
	y = pos.y / tileSizeY;
	for (int k = 0; k < map.size(); ++k) {
		const vector<int> &layer = map[k];
		tile = layer[y*mapSize.x + x];

		if (isCollisionTile(tile, k)) {
			return true;
		}
	}
}