/*
 Copyright (c) 2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#include "Platform/Time.h"
#include <sys/time.h>
#include <time.h>

namespace YourProduct
{
	uint64 Time::GetCurrent ()
	{
		struct timeval tv;
		gettimeofday (&tv, NULL);

		// Unix time => Windows file time
		return  ((uint64) tv.tv_sec + 134774LL * 24 * 3600) * 1000LL * 1000 * 10;
	}
}
