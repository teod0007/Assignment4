#pragma once

#include <GameEngine.h>

union SDL_Event;
class Graphics;
class Camera;
class SoundHelper;
class Player;
class Projectile;
class Enemy;
class ParallaxSystem;

class Game: public GameEngine
{
  friend class GameEngine;

public:
  ~Game();

protected:
  Game();

  void InitializeImpl();
  void UpdateImpl(float dt);
  void DrawImpl(Graphics *graphics, float dt);

  void Reset();
  void CalculateDrawOrder(std::vector<GameObject *>& drawOrder);
  void CalculateCameraViewpoint(Camera *camera);
  void CheckDeadProjectiles(std::vector<Projectile *>& projectileVector);
  void CheckCollisionEnemies(std::vector<Projectile *>& projectileVector);
  void CheckCollisionPlayer(std::vector<Projectile *>& projectileVector);
  
  void ShootWithEnemies();

  Camera *_gameCamera;
  SoundHelper _soundManager;
  Player *_player;
  Camera *_parallaxCamera;
  std::vector<Projectile *> _enemyProjectiles;
  std::vector<Projectile *> _playerProjectiles;
  Enemy** _enemyList;
  Timer enemySpawnTimer;
  int _lives, _score;
  bool message;

  ParallaxSystem *_backgroundParallaxSystem;
};