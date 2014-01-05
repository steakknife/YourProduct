/*
 Copyright (c) 2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Platform_Unix_Poller
#define TC_HEADER_Platform_Unix_Poller

#include "Platform/PlatformBase.h"

namespace YourProduct
{
	class Poller
	{
	public:
		Poller (int fileDescriptor1, int fileDescriptor2 = -1, int fileDescriptor3 = -1, int fileDescriptor4 = -1);
		virtual ~Poller () { }

		list <int> WaitForData (int timeOut = -1) const;

	protected:
		vector <int> FileDescriptors;

	private:
		Poller (const Poller &);
		Poller &operator= (const Poller &);
	};
}

#endif // TC_HEADER_Platform_Unix_Poller
