#ifndef _GAME_DATA_H
#define _GAME_DATA_H

#include <Seed.h>
using namespace Seed;

#define TYPE_GAMEDATA (Seed::TypeUser + 1)

class GameData : public IObject
{
	public:
		GameData();
		virtual ~GameData();

		virtual u32 GetScore();
		virtual void SetScore(u32);

		virtual bool IsGameOver();
		virtual void SetGameOver(bool);

		virtual bool IsSoundFXEnable();
		virtual void SetSoundFXEnable(bool);

		virtual bool IsMusicEnable();
		virtual void SetMusicEnable(bool);

		// IObject
		virtual const String GetClassName() const override;
		virtual int GetObjectType() const override;

	private:
		u32 iScore;
		bool bGameOver;
		bool bSoundFXEnable;
		bool bMusicEnable;
};

#endif // _GAME_DATA_H
