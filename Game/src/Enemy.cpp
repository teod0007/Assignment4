
#include "Enemy.h"

Enemy::Enemy()
{
	_transform.position = Vector3(0.0f, 0.0f, 0.0f);
}

Enemy::~Enemy()
{
	delete _enemyCube;
}
Enemy::Enemy(Vector3 position)
{
	_transform.position = position;
}


void Enemy::Initialize(Graphics *graphics)
{
	srand(time(NULL));

	int position = rand() % 3;

	switch (position)
	{
	case 0: _transform.position = Vector3(7.0f, 15.0f, 0.0f);
			break;
	case 1:_transform.position = Vector3(0.0f, 14.0f, 0.0f);
			break;
	case 2:_transform.position = Vector3(-7.0f, 13.0f, 0.0f);
			break;
	}

	_enemyCube = new Cube(_transform.position);
	_enemyCube->GetTransform().scale = Vector3(0.6f, 0.6f, 0.6f);
	_enemyCube->Initialize(graphics);
	_enemyCube->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	_moveSpeed = 2.0f;
	_state = MOVING;
	_movementDirection = RIGHT;
	_shoot = false;

	movementTimer.Start();
	bailOutTimer.Start();
	shootTimer.Start();

}

void Enemy::Update(float dt)
{
	movementTimer.Update();
	bailOutTimer.Update();
	shootTimer.Update();
	
	
	srand(time(NULL));


	
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


	if (bailOutTimer.GetElapsedTime() >= 30)
	{
		_movementDirection = DOWN;
		
	}
	if (shootTimer.GetElapsedTime() >= 3)
	{
		shootTimer.Stop();
		_shoot = true;;
		shootTimer.Start();
	}
	if (_transform.position.y <= -12)
		ResetPosition();


	_enemyCube->GetTransform().position = _transform.position;

}
void Enemy::Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt)
{
	_enemyCube->Draw(graphics, relativeTo, dt);
}

void Enemy::Move(Direction d)
{
	
		_state = MOVING;
		_movementDirection = d;

}

EntityState& Enemy::GetState()
{
	return _state;
}

bool& Enemy::GetShotStatus()
{
	return _shoot;
}

void Enemy::ResetPosition()
{
	srand(time(NULL));

	bailOutTimer.Stop();

	int position = rand() % 3;

	switch (position)
	{
	case 0: _transform.position = Vector3(7.0f, 15.0f, 0.0f);
		break;
	case 1:_transform.position = Vector3(0.0f, 14.0f, 0.0f);
		break;
	case 2:_transform.position = Vector3(-7.0f, 13.0f, 0.0f);
		break;
	}
	_movementDirection = RIGHT;
	bailOutTimer.Start();

}