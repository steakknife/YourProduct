/*
 Copyright (c) 2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#include "Platform.h"
#include "Bios.h"

#pragma pack(1)

struct BiosMemoryMapEntry
{
	uint64 BaseAddress;
	uint64 Length;
	uint32 Type;
};

#pragma pack()

bool GetFirstBiosMemoryMapEntry (BiosMemoryMapEntry &entry);
bool GetNextBiosMemoryMapEntry (BiosMemoryMapEntry &entry);
