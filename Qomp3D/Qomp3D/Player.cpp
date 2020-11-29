#include <iostream>
#include "Player.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>


void Player::init(ShaderProgram& shaderProgram)
{
	model = new AssimpModel();
	model->loadFromFile("models/cube10.obj", shaderProgram);

	velocity.x = 0.1f;
	velocity.y = 0.1f;

}

void Player::update(int deltaTime, vector<Wall*>* walls)
{
	// puede que mejor sin deltaTime, cuando hay lag puede petar...
	/*glm::vec3 compPosition_x = posPlayer + glm::vec3(velocity.x, 0, 0);
	glm::vec3 compPosition_y = posPlayer + glm::vec3(0,velocity.y,0);
	posPlayer += velocity;

	if (map->collisionMoveRight(posPlayer, model->getSize()))
	{
		if (map->collisionMoveRight(compPosition_x, model->getSize()))
		{
			velocity.x = -velocity.x;
		}
	}
	else if (map->collisionMoveLeft(posPlayer, model->getSize()))
	{
		if (map->collisionMoveLeft(compPosition_x, model->getSize()))
		{
			velocity.x = -velocity.x;
		}
	}

	if (map->collisionMoveUp(posPlayer, model->getSize()))
	{
		if (map->collisionMoveUp(compPosition_y, model->getSize()))
		{
			velocity.y = -velocity.y;
		}
	}
	else if (map->collisionMoveDown(posPlayer, model->getSize()))
	{
		if (map->collisionMoveDown(compPosition_y, model->getSize()))
		{
			velocity.y = -velocity.y;
		}
	}*/
	posPlayer.x += velocity.x;

	if (map->collisionMoveRight(posPlayer, model->getSize()))
	{
		posPlayer.x -= velocity.x;
		velocity.x = -abs(velocity.x);
	}
	else if (map->collisionMoveLeft(posPlayer, model->getSize()))
	{
		posPlayer.x -= velocity.x;
		velocity.x = abs(velocity.x);
	}

	for (int i = 0; i < (*walls).size(); ++i) {
		if (collideWall((*walls)[i])) {
			posPlayer.x -= velocity.x;
			velocity.x = -velocity.x;
		}
	}

	posPlayer.y += velocity.y;

	if (map->collisionMoveUp(posPlayer, model->getSize()))
	{
		posPlayer.y -= velocity.y;
		velocity.y = abs(velocity.y);
	}
	else if (map->collisionMoveDown(posPlayer, model->getSize()))
	{
		posPlayer.y -= velocity.y;
		velocity.y = -abs(velocity.y);
	}

	for (int i = 0; i < (*walls).size(); ++i) {
		if (collideWall((*walls)[i])) {
			posPlayer.y -= velocity.y;
			velocity.y = -velocity.y;
		}
	}

	if (map->lineCollision(posPlayer, model->getSize(), false)) {
		if (lastVelocity == 0)
			lastVelocity = velocity.y;
		velocity.y = 0;
	}
	else if (map->lineCollision(posPlayer, model->getSize(), true)) {
		if (lastVelocity == 0)
			lastVelocity = velocity.x;
		velocity.x = 0;
	}
	else {
		if (velocity.y == 0) {
			velocity.y = lastVelocity;
			lastVelocity = 0;
		}
		if (velocity.x == 0) {
			velocity.x = lastVelocity;
			lastVelocity = 0;
		}
	}
}

void Player::render(ShaderProgram& program)
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(posPlayer.x, -posPlayer.y, 0));
	program.setUniformMatrix4f("model", modelMatrix);

	model->render(program);
}

void Player::setPosition(const glm::vec3& position)
{
	posPlayer = position;
}

void Player::setVelocity(const glm::vec3& vel)
{
	velocity = vel;
}

glm::vec3 Player::getPosition()
{
	return posPlayer;
}

glm::vec3 Player::getSize()
{
	return model->getSize();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}


void Player::keyPressed(int key)
{
	bool already_pressed = Game::instance().getKey(key);
	if (key == ' ' && !already_pressed)
	{
		velocity.y = -velocity.y;
	}
}

bool Player::collideWall(Wall* wall)
{
	glm::vec3 wallPos = wall->getPosition();
	glm::vec3 wallSize = wall->getSize();
	bool bVertical = wall->getOrientation();

	glm::vec3 playerSize = getSize();

	float Wxmin = wallPos.x;
	float Wxmax = wallPos.x + wallSize.x;
	float Wymin = wallPos.y;
	float Wymax = wallPos.y + wallSize.y;
	
	float Pxmin = posPlayer.x;
	float Pxmax = posPlayer.x + playerSize.x;
	float Pymin = posPlayer.y;
	float Pymax = posPlayer.y + playerSize.y;

	return ((Wxmin < Pxmax&& Pxmin < Wxmax) && (Wymin < Pymax&& Pymin < Wymax));
}