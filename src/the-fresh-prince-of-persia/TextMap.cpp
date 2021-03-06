#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TextMap.h"
#include <GL/glut.h>

#define TILESET_PATH "sprites/text/text_tileset.png"

using namespace std;


void TextMap::init(const glm::vec2 screenSize, const glm::vec2 &_minCoords, ShaderProgram &_program) {
	// Somewhere this is being assigned to and it causes some problems
	// with the rendering.
	//minCoords = _minCoords;
	minCoords = glm::vec2(0, 0);

	program = _program;

	initCharMap();

	tileSizeX = tileSizeY = blockSizeX = blockSizeY = 32;
	tilesheetSize.x = 16;
	tilesheetSize.y = 16;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	bool b = tilesheet.loadFromFile(TILESET_PATH, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	prepareMap(screenSize);
}

TextMap::TextMap()
{
}

TextMap::~TextMap()
{
	/*if (m != NULL)
	delete m;*/
}

void TextMap::clearText() {
	charMap = vector<int>(mapSize.x*mapSize.y, 0);
	prepareArrays();
}

void TextMap::addText(const glm::vec2 coords, string text) {
	int index = (coords.y*mapSize.x + coords.x);
	for (int i = 0; i < text.size(); ++i, ++index) {
		if (index < charMap.size()) {
			charMap[index] = CHARS[text[i]];
		}
		else break;
	}
	prepareArrays();
}

void TextMap::addText(int y, string text, Alignment alignment) {
	switch (alignment) {
	case CENTER: {
		int m = text.size() / 2;
		int x = mapSize.x / 2 - m;
		addText(glm::vec2(x, y), text);
		break;
	}
	case LEFT:
		addText(glm::vec2(0, y), text);
		break;
	case RIGHT:
		addText(glm::vec2(mapSize.x - text.size(), y), text);
		break;
	}
}

void TextMap::initCharMap() {
	string validChars = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	for (int i = 0; i < validChars.size(); ++i) {
		CHARS[validChars[i]] = 33 + i;	// 33 is the space character in this tileset
	}
	// I'm not quite sure C++ supports extended ASCII
	// For my name <3
	CHARS['�'] = 139;
	// For Alc�zar
	CHARS['�'] = 142;
}

void TextMap::updatePosition(int x, int y)
{
	minCoords = glm::vec2(x, y);
}

void TextMap::render() const
{
	if (vao <= 0) return;

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();

	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TextMap::free()
{
	glDeleteBuffers(1, &vbo);
}

void TextMap::prepareMap(const glm::vec2 screenSize) {
	mapSize.x = screenSize.x / tileSizeX;
	mapSize.y = screenSize.y / tileSizeY;
	charMap = vector<int>(mapSize.x*mapSize.y, 0);
}

void TextMap::prepareArrays()
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	halfTexel = glm::vec2(
		0.5f / tilesheet.width(),
		0.5f / tilesheet.height());

	for (int j = 0; j < mapSize.y; j++) {
		for (int i = 0; i < mapSize.x; i++) {
			tile = charMap[j * mapSize.x + i];
			if (tile != 0) {
				// Non-empty tile
				nTiles++;
				//posTile = glm::vec2(-minCoords.x/4 + 
				//	i * tileSizeX, -minCoords.y/4 + 
				//	j * tileSizeY);
				posTile = glm::vec2(
					i * tileSizeX - minCoords.x, 
					j * tileSizeY - minCoords.y);
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

		if (nTiles == 0) continue;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
		texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	}
}