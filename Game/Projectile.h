#pragma once

#include <GameObject.h>
#include "src/Cube.h"
#include <Timer.h>
#include <time.h>
#include <stdlib.h>

class Cube;
class Timer;

class Projectile : public GameObject
{
public:
	Projectile();
	~Projectile();
	Projectile(Vector3 position);
	Projectile(Vector3 position,Vector3 destination);

	void Initialize(Graphics *graphics);
	void Update(float dt);
	void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);
	EntityState& GetState();
protected:
	//void *_vertexPointer;

	Cube *_projectileCube;
	EntityState _state; //Used to set the current state of the entity
	Timer bailOutTimer;//Timer used for "AI" purposes
	Vector3* destination;

	float _moveSpeedX;
	float _moveSpeedY;
};