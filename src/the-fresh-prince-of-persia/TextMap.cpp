#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TextMap.h"
#include <GL/glut.h>

#define TILESET_PATH "sprites/text/text_tileset.png"

using namespace std;

TextMap *TextMap::createTextMap(const glm::vec2 screenSize, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TextMap *map = new TextMap(screenSize, minCoords, program);
	return map;
}


TextMap::TextMap(const glm::vec2 screenSize, const glm::vec2 &_minCoords, ShaderProgram &_program)
{
	minCoords = minCoords;
	program = program;

	initCharMap();

	tileSizeX = tileSizeY = blockSizeX = blockSizeY = 64;
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

TextMap::~TextMap()
{
	/*if (m != NULL)
	delete m;*/
}


void TextMap::addText(const glm::vec2 coords, string text) {
	int index = (coords.y*mapSize.y + coords.x);
	for (int i = 0; i < text.size(); ++i) {
		index += i;
		if (index < m.size()) {
			m[index] = CHARS[text[i]];
		}
		else break;
	}
	prepareArrays();
}

void TextMap::initCharMap() {
	string validChars = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	for (int i = 0; i < validChars.size(); ++i) {
		CHARS[validChars[i]] = 33 + i;	// 33 is the space character in this tileset
	}
	// I'm not quite sure C++ supports extended ASCII
	// For my name <3
	//CHARS['é'] = 139;
	// For Alcázar
	//CHARS['á'] = 142;
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
	m = vector<int>(mapSize.x*mapSize.y, 0);
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
			tile = m[j * mapSize.x + i];
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

		if (nTiles == 0) return;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
		texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	}
}