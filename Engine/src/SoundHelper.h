#pragma once 

#include <SDL_mixer.h>

enum GameSound { Move = 0, Dead, FinishLevel, EndGame };

class SoundHelper
{
	Mix_Chunk *moveSound,*deadSound, *fallSound, *finishLevelSound, *gameOverSound;

public:

	void Initialize();
	void PlaySound(GameSound sound);

};