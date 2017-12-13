/*************************************************************************/ /*!
@File
@Title          Server bridge for renesas
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Declares common defines and structures that are used by both
                the client and sever side of the bridge for renesas
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

#include <stddef.h>
#include <asm/uaccess.h>

#include "img_defs.h"

#include "common_renesas_bridge.h"

#include "pvr_debug.h"
#include "connection_server.h"
#include "pvr_bridge.h"
#include "rgx_bridge.h"
#include "srvcore.h"
#include "handle.h"


#if defined(SUPPORT_EGL_EXTENSION_GET_MEMINFO_REL)
extern IMG_UINT32 g_TotalPageSize;
extern IMG_UINT32 g_PeakPageSize;
extern IMG_UINT32 g_TotalDCBPageSize;
extern IMG_UINT32 g_PeakDCBPageSize;
#endif /* defined(SUPPORT_EGL_EXTENSION_GET_MEMINFO_REL) */

/* ***************************************************************************
 * Server-side bridge entry points
 */

#if defined(SUPPORT_EGL_EXTENSION_GET_MEMINFO_REL)
static IMG_INT
PVRSRVGetMenInfoKM(IMG_UINT32                        ui32BridgeID,
                   void                               *psDebugInfoIN,
                   PVRSRV_BRIDGE_OUT_DEBUG_GET_INFO   *psDebugInfoOUT,
                   CONNECTION_DATA *psConnection)
{
	psDebugInfoOUT->ui32HostCurrent   = 0;
	psDebugInfoOUT->ui32HostPeak      = 0;
	psDebugInfoOUT->ui32DeviceCurrent = g_TotalPageSize;
	psDebugInfoOUT->ui32DevicePeak    = g_PeakPageSize;
#if defined(SUPPORT_DISPLAY_CLASS)
	psDebugInfoOUT->ui32DeviceCurrent += g_TotalDCBPageSize;
	psDebugInfoOUT->ui32DevicePeak    += g_PeakDCBPageSize;
#endif /* defined(SUPPORT_DISPLAY_CLASS) */
    psDebugInfoOUT->eError = PVRSRV_OK;

	return 0;
}
#endif /* defined(SUPPORT_EGL_EXTENSION_GET_MEMINFO_REL) */

/* *************************************************************************** 
 * Server bridge dispatch related glue 
 */

static IMG_BOOL bUseLock = IMG_TRUE;

PVRSRV_ERROR InitRenesasBridge(void);
PVRSRV_ERROR DeinitRenesasBridge(void);

/*
 * Register all Renesas functions with services
 */
PVRSRV_ERROR InitRenesasBridge(void)
{
#if defined(SUPPORT_EGL_EXTENSION_GET_MEMINFO_REL)
	SetDispatchTableEntry(PVRSRV_BRIDGE_RENESAS, PVRSRV_BRIDGE_DEBUG_GET_INFO, PVRSRVGetMenInfoKM,
					NULL, bUseLock);
#endif /* defined(SUPPORT_EGL_EXTENSION_GET_MEMINFO_REL) */

#if defined(SUPPORT_EXTENSION_QUERY_PARAM_BUFFER_REL)
	SetDispatchTableEntry(PVRSRV_BRIDGE_RENESAS, PVRSRV_BRIDGE_QUERY_PARAMBUFFER_OVERFLOW_REL, PVRSRVRGXQueryParamBufferOverflowKM,
					NULL, bUseLock);

	SetDispatchTableEntry(PVRSRV_BRIDGE_RENESAS, PVRSRV_BRIDGE_QUERY_PARAMBUFFER_SIZE_REL, PVRSRVRGXQueryParamBufferSizeKM,
					NULL, bUseLock);
#endif

	return PVRSRV_OK;
}

/*
 * Unregister all Renesas functions with services
 */
PVRSRV_ERROR DeinitRenesasBridge(void)
{
	return PVRSRV_OK;
}
