/*
 Copyright (c) 2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#include "CoreException.h"
#include "Platform/SerializerFactory.h"

namespace YourProduct
{
	void ElevationFailed::Deserialize (shared_ptr <Stream> stream)
	{
		ExecutedProcessFailed::Deserialize (stream);
	}

	void ElevationFailed::Serialize (shared_ptr <Stream> stream) const
	{
		ExecutedProcessFailed::Serialize (stream);
	}

#define TC_EXCEPTION(TYPE) TC_SERIALIZER_FACTORY_ADD(TYPE)
#undef TC_EXCEPTION_NODECL
#define TC_EXCEPTION_NODECL(TYPE) TC_SERIALIZER_FACTORY_ADD(TYPE)

	TC_SERIALIZER_FACTORY_ADD_EXCEPTION_SET (CoreException);
}
