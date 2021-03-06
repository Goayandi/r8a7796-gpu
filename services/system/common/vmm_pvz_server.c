/*************************************************************************/ /*!
@File			vmm_pvz_server.c
@Title          VM manager server para-virtualization handlers
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Header provides VMM server para-virtz handler APIs
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/

#include "pvrsrv.h"
#include "img_types.h"
#include "pvrsrv_vz.h"
#include "pvrsrv_error.h"

#include "vz_support.h"
#include "vz_vm.h"
#include "vmm_pvz_server.h"
#include "vz_physheap.h"


static void
PvzServerLockAcquire(void)
{
	PVRSRV_DATA *psPVRSRVData = PVRSRVGetPVRSRVData();
	PVRSRV_VIRTZ_DATA *psPVRSRVVzDaata;
	
	psPVRSRVVzDaata = psPVRSRVData->hVzData;
	OSLockAcquire(psPVRSRVVzDaata->hPvzLock);
}

static void
PvzServerLockRelease(void)
{
	PVRSRV_DATA *psPVRSRVData = PVRSRVGetPVRSRVData();
	PVRSRV_VIRTZ_DATA *psPVRSRVVzDaata;

	psPVRSRVVzDaata = psPVRSRVData->hVzData;
	OSLockRelease(psPVRSRVVzDaata->hPvzLock);
}


/*
 * ===========================================================
 *  The following server para-virtualization (pvz) functions
 *  are exclusively called by the VM manager (hypervisor) on
 *  behalf of guests to complete guest pvz calls 
 *  (guest -> vm manager -> host)
 * ===========================================================
 */

PVRSRV_ERROR
PvzServerCreateDevConfig(IMG_UINT32 ui32OSID,
						 IMG_UINT32 ui32FuncID,
						 IMG_UINT32 ui32DevID,
						 IMG_UINT32 *pui32IRQ,
						 IMG_UINT32 *pui32RegsSize,
						 IMG_UINT64 *pui64RegsCpuPBase)
{
	PVRSRV_ERROR eError;

	PVR_ASSERT(ui32FuncID == PVZ_BRIDGE_CREATEDEVICECONFIG);

	PvzServerLockAcquire();

	eError = SysVzCreateDevConfig(ui32OSID,
								  ui32DevID,
								  pui32IRQ,
								  pui32RegsSize,
								  pui64RegsCpuPBase);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;
}

PVRSRV_ERROR
PvzServerDestroyDevConfig(IMG_UINT32 ui32OSID,
						  IMG_UINT32 ui32FuncID,
						  IMG_UINT32 ui32DevID)
{
	PVRSRV_ERROR eError;

	PVR_ASSERT(ui32FuncID == PVZ_BRIDGE_DESTROYDEVICECONFIG);

	PvzServerLockAcquire();

	eError = SysVzDestroyDevConfig(ui32OSID, ui32DevID);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;
}

PVRSRV_ERROR
PvzServerCreateDevPhysHeaps(IMG_UINT32 ui32OSID,
							IMG_UINT32 ui32FuncID,
							IMG_UINT32  ui32DevID,
							IMG_UINT32 *peHeapType,
							IMG_UINT64 *pui64FwSize,
							IMG_UINT64 *pui64FwAddr,
							IMG_UINT64 *pui64GpuSize,
							IMG_UINT64 *pui64GpuAddr)
{
	PVRSRV_ERROR eError;

	PVR_ASSERT(ui32FuncID == PVZ_BRIDGE_CREATEDEVICEPHYSHEAPS);

	PvzServerLockAcquire();

	eError = SysVzCreateDevPhysHeaps(ui32OSID,
									 ui32DevID,
									 peHeapType,
									 pui64FwSize,
									 pui64FwAddr,
									 pui64GpuSize,
									 pui64GpuAddr);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;
}

PVRSRV_ERROR
PvzServerDestroyDevPhysHeaps(IMG_UINT32 ui32OSID,
							 IMG_UINT32 ui32FuncID,
							 IMG_UINT32 ui32DevID)
{
	PVRSRV_ERROR eError;

	PVR_ASSERT(ui32FuncID == PVZ_BRIDGE_DESTROYDEVICEPHYSHEAPS);

	PvzServerLockAcquire();

	eError = SysVzDestroyDevPhysHeaps(ui32OSID, ui32DevID);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;
}

PVRSRV_ERROR
PvzServerMapDevPhysHeap(IMG_UINT32 ui32OSID,
						IMG_UINT32 ui32FuncID,
						IMG_UINT32 ui32DevID,
						IMG_UINT64 ui64Size,
						IMG_UINT64 ui64PAddr)
{
	PVRSRV_ERROR eError;

	PVR_ASSERT(ui32FuncID == PVZ_BRIDGE_MAPDEVICEPHYSHEAP);

	PvzServerLockAcquire();

	eError = SysVzRegisterFwPhysHeap(ui32OSID,
									 ui32DevID,
									 ui64Size,
									 ui64PAddr);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;
}

PVRSRV_ERROR
PvzServerUnmapDevPhysHeap(IMG_UINT32 ui32OSID,
						  IMG_UINT32 ui32FuncID,
						  IMG_UINT32 ui32DevID)
{
	PVRSRV_ERROR eError;

	PVR_ASSERT(ui32FuncID == PVZ_BRIDGE_UNMAPDEVICEPHYSHEAP);

	PvzServerLockAcquire();

	eError = SysVzUnregisterFwPhysHeap(ui32OSID, ui32DevID);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;
}


/*
 * ============================================================
 *  The following server para-virtualization (pvz) functions
 *  are exclusively called by the VM manager (hypervisor) to
 *  pass side band information to the host (vm manager -> host)
 * ============================================================
 */

PVRSRV_ERROR
PvzServerOnVmOnline(IMG_UINT32 ui32OSID, IMG_UINT32 ui32Priority)
{
	PVRSRV_ERROR eError;

	PvzServerLockAcquire();

	eError = SysVzOnVmOnline(ui32OSID, ui32Priority);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;
}

PVRSRV_ERROR
PvzServerOnVmOffline(IMG_UINT32 ui32OSID)
{
	PVRSRV_ERROR eError;

	PvzServerLockAcquire();

	eError = SysVzOnVmOffline(ui32OSID);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;
}

PVRSRV_ERROR
PvzServerVMMConfigure(VMM_CONF_PARAM eVMMParamType, IMG_UINT32 ui32ParamValue)
{
	PVRSRV_ERROR eError;

	PvzServerLockAcquire();

	eError = SysVzVMMConfigure(eVMMParamType, ui32ParamValue);
	PVR_ASSERT(eError == PVRSRV_OK);

	PvzServerLockRelease();

	return eError;

}

/******************************************************************************
 End of file (vmm_pvz_server.c)
******************************************************************************/
