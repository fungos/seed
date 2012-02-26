#ifndef _IOS_SYSTEM_STUB_H_
#define _IOS_SYSTEM_STUB_H_

#include "Defines.h"

/// iOS Touch Info
struct iosTouchInfo
{
	u32 iTaps;
	f32 fPosX;
	f32 fPosY;
	f32 fRelX;
	f32 fRelY;
	f32 fOldX;
	f32 fOldY;
	u32 bStatus; // 1 press, 2 hold, 3 release, outro valor = invalido
};

extern iosTouchInfo iosTouchBuff[PLATFORM_MAX_INPUT];

#endif // _IOS_SYSTEM_STUB_H_
