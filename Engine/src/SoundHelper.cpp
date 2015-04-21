#include "SoundHelper.h"


SoundHelper::~SoundHelper()
{
	 
	delete enemyShotSound;
	delete terminatedSound;
	delete playerShotSound;
	delete letsRockSound;
	delete gameOverSound;
	delete themeMusic;

	terminatedSound = enemyShotSound = playerShotSound = letsRockSound = gameOverSound = nullptr;
	themeMusic = nullptr;
}

 void SoundHelper::Initialize()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 4096);
	themeMusic = Mix_LoadMUS("rickroll.mp3");
	gameOverSound = Mix_LoadWAV("gameover.wav");
	enemyShotSound = Mix_LoadWAV("enemy_shot.wav");
	playerShotSound = Mix_LoadWAV("player_shot.wav");
	terminatedSound = Mix_LoadWAV("terminated.wav");
	letsRockSound = Mix_LoadWAV("lets_rock.wav");
	
	Mix_VolumeMusic(50);
	
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
	case Dead:
		Mix_PlayChannel(-1, deadSound, 0);
		break;
	case EndGame:
		Mix_PlayChannel(-1, gameOverSound, 0);
		break;
	case Terminated:
		Mix_PlayChannel(-1, terminatedSound, 0);
		break;
	case enemyShot:
		Mix_PlayChannel(-1, enemyShotSound, 0);
		break;
	case playerShot:
		Mix_PlayChannel(-1, playerShotSound, 0);
		break;
	case letsRock:
		Mix_PlayChannel(-1, letsRockSound, 0);
		break;
	case Music:
		Mix_PlayMusic(themeMusic, -1);
		break;
	default:
		break;
	}
	
}