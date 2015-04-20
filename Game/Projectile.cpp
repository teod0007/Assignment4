
#include "Projectile.h"

#include <math.h>
Projectile::Projectile()
{
	_transform.position = Vector3(0.0f, 0.0f, 0.0f);
}
Projectile::Projectile(Vector3 position)
{
	destination = nullptr;
	_transform.position = position;
}

Projectile::Projectile(Vector3 position, Vector3 destination)
{
	_transform.position = position;
	this->destination = new Vector3(destination.x, destination.y, destination.z);
	_transform.rotation.x = destination.x - position.x;
	_transform.rotation.y = destination.y - position.y;
}


void Projectile::Initialize(Graphics *graphics)
{
	_projectileCube = new Cube(_transform.position);
	_projectileCube->GetTransform().scale = Vector3(0.3f, 0.3f, 0.3f);
	_projectileCube->Initialize(graphics);
	_projectileCube->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	_state = MOVING;

	if (destination == nullptr)
	{
		_moveSpeedX = 0;
		_moveSpeedY = 4;
	}
	else
	{
		float radians = atan2f(_transform.rotation.x, _transform.rotation.y);
		_moveSpeedY = 4.0f * cos(radians);
		_moveSpeedX = 4.0f * sin(radians);
		_projectileCube->SetColor(0.0f, 1.0f, 1.0f, 1.0f);
	}

	bailOutTimer.Start();

}

void Projectile::Update(float dt)
{
	bailOutTimer.Update();


	srand(time(NULL));

	_transform.position.y += _moveSpeedY * dt;
	_transform.position.x += _moveSpeedX * dt;
	/*
	if ((movementTimer.GetElapsedTime() >= (rand() % 3) + 1 || _transform.position.x < -10 || _transform.position.x > 10) && _movementDirection != DOWN)
	{
		movementTimer.Stop();
		srand(time(NULL));

		if (_movementDirection == RIGHT)
			_movementDirection = LEFT;
		else
			if (_movementDirection == LEFT)
				_movementDirection = RIGHT;

		movementTimer.Start();
	}
	*/

	/*
	if (_state == MOVING)
	{


		if (_movementDirection == DOWN)
		{
			_transform.position.y -= dt * _moveSpeed;
			//_transform.position.y--;
			//_transform.position.x++;
		}
		else
		{
			_transform.position.y -= dt * _moveSpeed * 0.3;//slowly descends

		}

		if (_movementDirection == RIGHT)
		{
			_transform.position.x += dt * _moveSpeed;
			//_transform.position.y++;
			//_transform.position.z--;
		}
		if (_movementDirection == LEFT)
		{
			_transform.position.x -= dt * _moveSpeed;
			//_transform.position.y--;
			//_transform.position.z++;
		}


	}
	*/

	if (bailOutTimer.GetElapsedTime() >= 5)
	{
		bailOutTimer.Stop();
		_state = DEAD;

	}

	_projectileCube->GetTransform().position = _transform.position;

}

void Projectile::Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt)
{
	_projectileCube->Draw(graphics, relativeTo, dt);
}

EntityState& Projectile::GetState()
{
	return _state;
}