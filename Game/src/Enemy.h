#pragma once

#include <GameObject.h>
#include "Cube.h"
#include <Timer.h>
#include <time.h>
#include <stdlib.h>

class Cube;
class Timer;

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();
	Enemy(Vector3 position);

	void Initialize(Graphics *graphics);
	void Update(float dt);
	void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);
	void Move(Direction d);
	EntityState& GetState();
	void ResetPosition();
	bool& GetShotStatus();
protected:
	//void *_vertexPointer;

	Cube *_enemyCube;
	EntityState _state; //Used to set the current state of the entity
	Direction _movementDirection; //If the state is "MOVING", which direction it is moving
	Timer movementTimer; //Timer used for "AI" purposes
	Timer bailOutTimer;//Timer used for "AI" purposes
	Timer shootTimer;//Timer used for "AI" purposes

	float _moveSpeed;
	bool _shoot;
	float _destX, _destY, _destZ; //Where it is going if in movement
};