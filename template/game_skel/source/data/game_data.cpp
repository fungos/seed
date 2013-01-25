#include "game_data.h"

GameData::GameData()
    : iScore(0)
    , bGameOver(false)
{
}

GameData::~GameData()
{
}

u32 GameData::GetScore()
{
	return iScore;
}

void GameData::SetScore(const u32 score)
{
	iScore = score;
}

bool GameData::IsGameOver()
{
	return bGameOver;
}

void GameData::SetGameOver(const bool gameOver)
{
	bGameOver = gameOver;
}

bool GameData::IsSoundFXEnable()
{
	return bSoundFXEnable;
}

void GameData::SetSoundFXEnable(const bool soundFX)
{
	bSoundFXEnable = soundFX;
}

bool GameData::IsMusicEnable()
{
	return bGameOver;
}

void GameData::SetMusicEnable(const bool musicEnable)
{
	bMusicEnable = musicEnable;
}

const String GameData::GetClassName() const
{
	return "GameData";
}

int GameData::GetObjectType() const
{
	return TYPE_GAMEDATA;
}
