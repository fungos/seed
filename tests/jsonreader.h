#ifndef __TEST_JSONREADER__
#define __TEST_JSONREADER__

#include <Seed.h>
#include "testbase.h"

class TestJsonReader : public TestBase
{
	public:
		TestJsonReader();
		~TestJsonReader();

		virtual bool Update(f32 dt);

	private:
		SEED_DISABLE_COPY(TestJsonReader);

	private:
		Sprite spt;
		f32 fRot;
		u32 iCol;
		u32 iCount;
		bool bDir;
};

#endif // __TEST_JSONREADER__
