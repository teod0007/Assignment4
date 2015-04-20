#include "Game.h"
#include <GameObject.h>
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <InputManager.h>
#include <SoundHelper.h>

#include "Cube.h"
#include "Player.h"
#include "Enemy.h"
#include "../Projectile.h"
#include <Graphics/GraphicsOpenGL.h>

#include <Cameras/Camera.h>
#include <Cameras/PerspectiveCamera.h>
#include <Cameras/OrthographicCamera.h>

#include <Parallax/ParallaxLayer.h>
#include <Parallax/ParallaxSystem.h>

#define MAX_ENEMY_NUMBER 5

// Initializing our static member pointer.
GameEngine* GameEngine::_instance = nullptr;

GameEngine* GameEngine::CreateInstance()
{
  if (_instance == nullptr)
  {
    _instance = new Game();
  }
  return _instance;
}

Game::Game() : GameEngine()
{

}

Game::~Game()
{

  // Clean up our pointers.
  delete _parallaxCamera;
  _parallaxCamera = nullptr;

  ParallaxLayer *layerToDelete = nullptr;
  while (_backgroundParallaxSystem->LayerCount() > 0)
  {
    // Delete all of the layers inside of our parallax system.
    layerToDelete = _backgroundParallaxSystem->PopLayer();

    delete layerToDelete;
    layerToDelete = nullptr;
  }

  delete _backgroundParallaxSystem;
  _backgroundParallaxSystem;
}

SDL_Renderer *_renderer;
SDL_Texture *_texture;

void Game::InitializeImpl()
{
  SDL_SetWindowTitle(_window, "Game");

  float nearPlane = 0.01f;
  float farPlane = 100.0f;
  //Vector4 position(3.5f, 2.5f, 2.5f, 0.0f);
  Vector4 position(0.0f, 0.0f, 2.5f, 0.0f);
  Vector4 lookAt = Vector4::Normalize(Vector4::Difference(Vector4(0.0f, 0.0f, 0.0f, 0.0f), position));
  Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);

  //_gameCamera = new PerspectiveCamera(100.0f, 1.0f, nearPlane, farPlane, position, lookAt, up);
  //_gameCamera = new PerspectiveCamera(100.0f, 1.0f, nearPlane, farPlane, position, lookAt, up);
  _gameCamera = new OrthographicCamera(-10.0f, 10.0f, 10.0f, -10.0f, nearPlane, farPlane, position, lookAt, up);
  _parallaxCamera = new OrthographicCamera(-10.0f, 10.0f, 10.0f, -10.0f, nearPlane, farPlane, position, lookAt, up);

  _enemyList = new Enemy*[MAX_ENEMY_NUMBER];
  for (int i = 0; i < MAX_ENEMY_NUMBER; i++)
	  _enemyList[i] = nullptr;
  enemySpawnTimer.Start();

  _backgroundParallaxSystem = new ParallaxSystem();
  _soundManager.Initialize();
  _player = new Player();
  _lives = 3;
  _score = 0;
  message = false;

  ParallaxLayer *layer1 = new ParallaxLayer("space.jpg", Vector2(0, 2));
  layer1->GetTransform().position = Vector3(0,20,0);
  _backgroundParallaxSystem->PushLayer(layer1);

  _objects.push_back(_player);
  _objects.push_back(new Enemy());

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Initialize(_graphicsObject);
  }

  _backgroundParallaxSystem->Initialize(_graphicsObject);
  _soundManager.PlaySound(Music);
  _soundManager.PlaySound(letsRock);
}

void Game::UpdateImpl(float dt)
{
  enemySpawnTimer.Update();
  if (enemySpawnTimer.GetElapsedTime() >= 2)
  {
	  enemySpawnTimer.Stop();
	  for (int i = 0; i < MAX_ENEMY_NUMBER; i++)
	  {
		  if (_enemyList[i] == nullptr)
		  {
			  _enemyList[i] = new Enemy();
			  _enemyList[i]->Initialize(_graphicsObject);
			  _objects.push_back(_enemyList[i]);
			  break;
		  }
		  else
		  {
			  if (_enemyList[i]->GetState() == DEAD)
			  {
				  _enemyList[i]->GetState() = MOVING;
			  }
		  }
	  }
	  enemySpawnTimer.Start();
  }
  //SDL_Event evt;
  //SDL_PollEvent(&evt);
  InputManager::GetInstance()->Update(dt);

  if (InputManager::GetInstance()->GetKeyState(SDLK_UP, SDL_KEYDOWN) == true)
  {
	  _player->Move(UP);
	  //_soundManager.PlaySound(Move);
  }
  if (InputManager::GetInstance()->GetKeyState(SDLK_DOWN, SDL_KEYDOWN) == true)
  {
	  _player->Move(DOWN);
	  //_soundManager.PlaySound(Move);
  }
  if (InputManager::GetInstance()->GetKeyState(SDLK_RIGHT, SDL_KEYDOWN) == true)
  {
	  _player->Move(RIGHT);
	  //_soundManager.PlaySound(Move);
  }
  if (InputManager::GetInstance()->GetKeyState(SDLK_LEFT, SDL_KEYDOWN) == true)
  {
	  _player->Move(LEFT);
	  //_soundManager.PlaySound(Move);
  }

  if (InputManager::GetInstance()->GetKeyState(SDLK_SPACE, SDL_KEYUP) == true)
  {
	  _soundManager.PlaySound(playerShot);
	  Projectile* p = new Projectile(_player->GetTransform().position);
	  p->Initialize(_graphicsObject);
	  _playerProjectiles.push_back(p);
	  _objects.push_back(p);
	  //_soundManager.PlaySound(Move);
  }
  
  
  ShootWithEnemies();
  CheckCollisionPlayer(_enemyProjectiles);
  CheckCollisionEnemies(_playerProjectiles);
  CheckDeadProjectiles(_playerProjectiles);
  CheckDeadProjectiles(_enemyProjectiles);

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Update(dt);
  }

  _backgroundParallaxSystem->Update(Vector2(0,-1), dt);
}

void Game::DrawImpl(Graphics *graphics, float dt)
{
  std::vector<GameObject *> renderOrder = _objects;
  //CalculateDrawOrder(renderOrder);

  // Draw parallax backgrounds
  glPushMatrix();
  {
    CalculateCameraViewpoint(_parallaxCamera);

    _backgroundParallaxSystem->Draw(graphics, _gameCamera->GetProjectionMatrix(), dt);
  }
  glPopMatrix();

  // Draw scenery on top.
  glPushMatrix();
  {
    glClear(GL_DEPTH_BUFFER_BIT);
    CalculateCameraViewpoint(_gameCamera);

    for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++)
    {
      (*itr)->Draw(graphics, _gameCamera->GetProjectionMatrix(), dt);
    }
  }
  glPopMatrix();


  std::string title = "Space Cube Annihilation ---- Score :: " + std::to_string(_score) + "  Lives::" + std::to_string(_lives);

  SDL_SetWindowTitle(_window, title.c_str());

  if (_player->GetState() == DEAD && !message)
  {
	  _soundManager.PlaySound(EndGame);
	  SDL_MessageBoxData* data = new SDL_MessageBoxData();
	  data->numbuttons = 2;
	  SDL_MessageBoxButtonData buttons[] = {
			  { /* .flags, .buttonid, .text */        0, 0, "no" },
			  { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" } };
	  SDL_MessageBoxColorScheme colorScheme = {
			  { /* .colors (.r, .g, .b) */
				  /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
				  { 255, 0, 0 },
				  /* [SDL_MESSAGEBOX_COLOR_TEXT] */
				  { 0, 255, 0 },
				  /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
				  { 255, 255, 0 },
				  /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
				  { 0, 0, 255 },
				  /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
				  { 255, 0, 255 }
			  }
	  };
	  data->colorScheme = &colorScheme;
	  data->buttons = buttons;
	  data->message = "Game Over - Play Again?";
	  int id;
	  if (SDL_ShowMessageBox(data, &id) < 0) {
		  SDL_Log("Error displaying message box");
	  }

	  message = true;

	  if (buttons[id].buttonid == 0)
	  {
		  GameEngine::Shutdown();

		  return;
	  }
	  else //if (buttons[id].buttonid == 1)
	  {

		  _objects.clear();
		  _playerProjectiles.clear();
		  _enemyProjectiles.clear();

		  for (int i = 0; i < MAX_ENEMY_NUMBER; i++)
		  {
			  delete _enemyList[i];
		  }

		  InitializeImpl();
	  }
  }


}

void Game::CalculateDrawOrder(std::vector<GameObject *>& drawOrder)
{
  // SUPER HACK GARBAGE ALGO.
  drawOrder.clear();


  auto objectsCopy = _objects;
  auto farthestEntry = objectsCopy.begin();
  while (objectsCopy.size() > 0)
  {
    bool entryFound = true;
    for (auto itr = objectsCopy.begin(); itr != objectsCopy.end(); itr++)
    {
      if (farthestEntry != itr)
      {
        if ((*itr)->GetTransform().position.y < (*farthestEntry)->GetTransform().position.y)
        {
          entryFound = false;
          farthestEntry = itr;
          break;
        }
      }
    }

    if (entryFound)
    {
      GameObject *farthest = *farthestEntry;

      drawOrder.push_back(farthest);
      objectsCopy.erase(farthestEntry);
      farthestEntry = objectsCopy.begin();
    }
  }
}

void Game::CalculateCameraViewpoint(Camera *camera)
{
  camera->Apply();

  Vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
  Vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
  Vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

  Vector3 cameraVector(camera->GetLookAtVector().x, camera->GetLookAtVector().y, camera->GetLookAtVector().z);
  Vector3 lookAtVector(0.0f, 0.0f, -1.0f);

  Vector3 cross = Vector3::Normalize(Vector3::Cross(cameraVector, lookAtVector));
  float dot = MathUtils::ToDegrees(Vector3::Dot(lookAtVector, cameraVector));

  glRotatef(cross.x * dot, 1.0f, 0.0f, 0.0f);
  glRotatef(cross.y * dot, 0.0f, 1.0f, 0.0f);
  glRotatef(cross.z * dot, 0.0f, 0.0f, 1.0f);

  glTranslatef(-camera->GetPosition().x, -camera->GetPosition().y, -camera->GetPosition().z);
}


void Game::CheckDeadProjectiles(std::vector<Projectile *>& projectileVector)
{
	int projN = 0;
	int objN = 0;
	bool erased = false;
	for (auto itr = projectileVector.begin(); itr != projectileVector.end(); itr++)
	{
		objN = 0;
		if ((*itr)->GetState() != DEAD)
		{
			projN++;
			continue;
		}
			
		for (auto itr2 = _objects.begin(); itr2 != _objects.end(); itr2++)
		{
			if ((*itr) == (*itr2))
			{
				projectileVector.erase(projectileVector.begin() + projN);
				_objects.erase(_objects.begin() + objN);
				erased = true;
				break;
			}
			objN++;
		}
		if (erased) break;
		projN++;
	}

}

void Game::CheckCollisionPlayer(std::vector<Projectile *>& projectileVector)
{
	int projN = 0;
	int objN = 0;
	bool erased = false;
	
		SDL_Rect* r = new SDL_Rect();
		for (auto itr = projectileVector.begin(); itr != projectileVector.end(); itr++)
		{
			if (_player->GetState() != DEAD && (*itr)->GetState() != DEAD)
			{
				SDL_Rect rP = SDL_Rect();
				SDL_Rect rE = SDL_Rect();
				rP.x = (*itr)->GetTransform().position.x;
				rP.y = (*itr)->GetTransform().position.y;
				rP.h = 1;
				rP.w = 1;
				rE.x = _player->GetTransform().position.x;
				rE.y = _player->GetTransform().position.y;
				rE.h = 1;
				rE.w = 1;
				if (SDL_IntersectRect(&rP, &rE, r))
				{
					(*itr)->GetState() = DEAD;
					if (_lives > 0)
					{
						
						_soundManager.PlaySound(Terminated);
						_lives--;
						_player->GetTransform().position.x = 0;
						_player->GetTransform().position.y = -7;
					}
					else
					{
						_player->GetState() = DEAD;
					}
					//_soundManager.PlaySound(EnemyDeath);

				}

			}
		}
		delete r;
}

void Game::CheckCollisionEnemies(std::vector<Projectile *>& projectileVector)
{
	int projN = 0;
	int objN = 0;
	bool erased = false;
	for (auto itr = projectileVector.begin(); itr != projectileVector.end(); itr++)
	{
		for (int i = 0; i < MAX_ENEMY_NUMBER; i++)
		{
			SDL_Rect* r = new SDL_Rect();

			if (_enemyList[i] != nullptr)
			{
				if (_enemyList[i]->GetState() != DEAD && (*itr)->GetState() != DEAD)
				{
					SDL_Rect rP = SDL_Rect();
					SDL_Rect rE = SDL_Rect();
					rP.x = (*itr)->GetTransform().position.x;
					rP.y = (*itr)->GetTransform().position.y;
					rP.h = 1;
					rP.w = 1;
					rE.x = _enemyList[i]->GetTransform().position.x;
					rE.y = _enemyList[i]->GetTransform().position.y;
					rE.h = 1;
					rE.w = 1;
					if (SDL_IntersectRect(&rP, &rE, r))
					{
  						_soundManager.PlaySound(Terminated);
						(*itr)->GetState() = DEAD;
						_enemyList[i]->ResetPosition();
 						_enemyList[i]->GetState() = DEAD;
						_score++;
						
						//_soundManager.PlaySound(EnemyDeath);

					}
				}
				

			}
			delete r;
		}


	}

	for (int i = 0; i < MAX_ENEMY_NUMBER; i++)
	{
		SDL_Rect* r = new SDL_Rect();

		if (_enemyList[i] != nullptr)
		{
			if (_enemyList[i]->GetState() != DEAD)
			{
				SDL_Rect rP = SDL_Rect();
				SDL_Rect rE = SDL_Rect();
				rP.x = _player->GetTransform().position.x;
				rP.y = _player->GetTransform().position.y;
				rP.h = 1;
				rP.w = 1;
				rE.x = _enemyList[i]->GetTransform().position.x;
				rE.y = _enemyList[i]->GetTransform().position.y;
				rE.h = 1;
				rE.w = 1;
				if (SDL_IntersectRect(&rP, &rE, r))
				{

					_enemyList[i]->ResetPosition();
					_enemyList[i]->GetState() = DEAD;
					if (_lives > 0)
					{
						_soundManager.PlaySound(Terminated);
						_lives--;
						_player->GetTransform().position.x = 0;
						_player->GetTransform().position.y = -7;
					}
					else
					{
						_player->GetState() = DEAD;
					}

					//_soundManager.PlaySound(EnemyDeath);

				}
			
			}
			

		}
		delete r;
	}


}

void Game::ShootWithEnemies()
{
	for (int i = 0; i < MAX_ENEMY_NUMBER; i++)
	{
		if (_enemyList[i] != nullptr)
			if (_enemyList[i]->GetShotStatus())
			{
				_soundManager.PlaySound(enemyShot);
				Projectile* p = new Projectile(_enemyList[i]->GetTransform().position, _player->GetTransform().position);
				p->Initialize(_graphicsObject);
				_enemyProjectiles.push_back(p);
				_objects.push_back(p);
				_enemyList[i]->GetShotStatus() = false;
			}
	}

}