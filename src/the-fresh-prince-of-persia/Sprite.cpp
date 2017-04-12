#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"
#include <iostream>

#define PLAYER_OFFSET_TURN 85

Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);
	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = { 
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, 0.f, sizeInSpritesheet.x, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, quadSize.y, 0.f, sizeInSpritesheet.y
	};
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0.f);
	isFacingLeft = true;
}

// This function is executed once every frame
void Sprite::update(int deltaTime)
{
	// We check wether if our animation is valid or not
	if (currentAnimation >= 0)
	{
		// We add deltaTime to out animation time
		timeAnimation += deltaTime;
		// While our animation time is greater than the milliseconds per keyframe required
		while (timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			// We change keyframe according to the times out delta time is greater to out animations
			// time per frame. This is done to execute animations when our GPU is slower than our
			// animation time per frame.
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			++ currentKeyframe;

			if (currentKeyframe == endingKeyframe) {
				++timesLoopedCurrentAnimation;
			}

			if (currentKeyframe > endingKeyframe)
			{				
				currentKeyframe = startingKeyframe;
			}
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render() const
{
	glm::mat4 modelview = glm::mat4(1.0f);
	modelview *= glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	// This saves us a lot of repetitive work
	if (!isFacingLeft) {
		// Yes, PLAYER_OFFSET_TURN is a magic number.
		// Yes, i am sorry.
		modelview *= glm::translate(glm::mat4(1.0f), glm::vec3(PLAYER_OFFSET_TURN, 0.f, 0.f));
		modelview *= glm::rotate(glm::mat4(1.0f), 3.1416f, glm::vec3(0, 1, 0));
	}

	
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if (animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if (animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId, int _startingKeyframe)
{
	changeAnimation(animId, _startingKeyframe, animations[animId].keyframeDispl.size() - 1);
}

void Sprite::changeAnimation(int animId, int _startingKeyframe, int _endingKeyframe)
{
	if (animId < int(animations.size()))
	{
		currentAnimation = animId;
		timeAnimation = 0.f;
		timesLoopedCurrentAnimation = 0;
		startingKeyframe = _startingKeyframe;
		currentKeyframe = startingKeyframe;
		endingKeyframe = _endingKeyframe;
		texCoordDispl = animations[animId].keyframeDispl[currentKeyframe];
	}
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}

glm::vec2 Sprite::getPosition()
{
	return position;
}

// This can generate some problems. If there are any bugs with animations
// looping where they must not, this function may be a good place to start debugging.
// FIXME
bool Sprite::isAtEndingKeyframe()
{
	return currentKeyframe == endingKeyframe;
}