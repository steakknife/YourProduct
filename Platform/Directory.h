/*
 Copyright (c) 2008-2009 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Platform_Directory
#define TC_HEADER_Platform_Directory

#include "PlatformBase.h"
#include "FilesystemPath.h"

namespace YourProduct
{
	class Directory
	{
	public:
		static void Create (const DirectoryPath &path);
		static DirectoryPath AppendSeparator (const DirectoryPath &path);
		static FilePathList GetFilePaths (const DirectoryPath &path = L".", bool regularFilesOnly = true);

	private:
		Directory ();
	};
}

#endif // TC_HEADER_Platform_Directory
