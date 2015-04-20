#pragma once 

#include <SDL_mixer.h>

enum GameSound { Move = 0, Dead, Terminated, FinishLevel, enemyShot, playerShot, letsRock, EndGame, Music };

class SoundHelper
{
	Mix_Chunk *moveSound, *deadSound, *terminatedSound, *enemyShotSound, *playerShotSound, *letsRockSound, *gameOverSound;
	Mix_Music *themeMusic;

public:

	void Initialize();
	void PlaySound(GameSound sound);

};