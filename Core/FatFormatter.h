/*
 Copyright (c) 2008-2010 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Core_FatFormatter
#define TC_HEADER_Core_FatFormatter

#include "Platform/Platform.h"

namespace YourProduct
{
	class FatFormatter
	{
	public:
		struct WriteSectorCallback
		{
			virtual ~WriteSectorCallback () { }
			virtual bool operator() (const BufferPtr &sector) = 0;
		};

		static void Format (WriteSectorCallback &writeSector, uint64 deviceSize, uint32 clusterSize, uint32 sectorSize);
	};
}

#endif // TC_HEADER_Core_FatFormatter
