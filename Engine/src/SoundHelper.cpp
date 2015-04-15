#include "SoundHelper.h"

 void SoundHelper::Initialize()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	moveSound = Mix_LoadWAV("move.wav");
	//deadSound = Mix_LoadWAV("sounds/die.wav");
	//finishLevelSound = Mix_LoadWAV("sounds/move.wav");
	//gameOverSound = Mix_LoadWAV("sounds/gameOver.wav");

	/*Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	Mix_OpenAudio(0, 0, 0, 0);*/
}


void SoundHelper::PlaySound(GameSound sound)
{

	switch (sound)
	{
	case Move:
		Mix_PlayChannel(-1, moveSound, 0);
		break;
	case Dead:
		Mix_PlayChannel(-1, deadSound, 0);
		break;
	case EndGame:
		Mix_PlayChannel(-1, gameOverSound, 0);
		break;
	case FinishLevel:
		Mix_PlayChannel(-1, finishLevelSound, 0);
		break;
	default:
		break;
	}
	
}