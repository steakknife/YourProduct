/*
 Copyright (c) 2008 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_DRIVER_VOLUME_FILTER
#define TC_HEADER_DRIVER_VOLUME_FILTER

#include "TCdefs.h"

extern uint32 HiddenSysLeakProtectionCount;

NTSTATUS VolumeFilterAddDevice (PDRIVER_OBJECT driverObject, PDEVICE_OBJECT pdo);
NTSTATUS VolumeFilterDispatchIrp (PDEVICE_OBJECT DeviceObject, PIRP Irp);

#endif // TC_HEADER_DRIVER_VOLUME_FILTER
