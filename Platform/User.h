/*
 Copyright (c) 2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Platform_User
#define TC_HEADER_Platform_User

#include "PlatformBase.h"

#ifdef TC_UNIX
#include <unistd.h>
#include <sys/types.h>
#endif

namespace YourProduct
{
	struct UserId
	{
		UserId () { }
#ifdef TC_UNIX
		UserId (uid_t systemId) : SystemId (systemId) { }

		uid_t SystemId;
#endif
	};
}

#endif // TC_HEADER_Platform_User
