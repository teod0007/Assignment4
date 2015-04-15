#pragma once

#include <GameEngine.h>

union SDL_Event;
class Graphics;
class Camera;
class SoundHelper;
class Player;
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

  Camera *_gameCamera;
  SoundHelper _soundManager;
  Player *_player;
  Camera *_parallaxCamera;

  ParallaxSystem *_backgroundParallaxSystem;
};