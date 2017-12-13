/* -*- mode: c; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* vi: set ts=8 sw=8 sts=8: */
/*************************************************************************/ /*!
@File
@Title          PowerVR DRM PCI driver
@Codingstyle    LinuxKernel
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
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

#include <drm/drmP.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/version.h>

#include "module_common.h"
#include "pvr_drv.h"
#include "pvrmodule.h"
#include "sysinfo.h"

static struct drm_driver pvr_drm_pci_driver;

static int pvr_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	DRM_DEBUG_DRIVER("device %p\n", &pdev->dev);

	return drm_get_pci_dev(pdev, ent, &pvr_drm_pci_driver);
}

static void pvr_remove(struct pci_dev *pdev)
{
	struct drm_device *ddev = pci_get_drvdata(pdev);

	DRM_DEBUG_DRIVER("device %p\n", &pdev->dev);

	drm_put_dev(ddev);
}

static void pvr_shutdown(struct pci_dev *pdev)
{
	struct drm_device *ddev = pci_get_drvdata(pdev);

	DRM_DEBUG_DRIVER("device %p\n", &pdev->dev);

	PVRSRVCommonDeviceShutdown(ddev->dev_private);
}

static const struct pci_device_id pvr_pci_ids[] = {
	{ PCI_DEVICE(SYS_RGX_DEV_VENDOR_ID, SYS_RGX_DEV_DEVICE_ID) },
#if defined(SYS_RGX_DEV1_DEVICE_ID)
	{ PCI_DEVICE(SYS_RGX_DEV_VENDOR_ID, SYS_RGX_DEV1_DEVICE_ID) },
#endif
	{ 0 }
};

MODULE_DEVICE_TABLE(pci, pvr_pci_ids);

static struct pci_driver pvr_pci_driver = {
	.name		= DRVNAME,
	.driver.pm	= &pvr_pm_ops,
	.id_table	= pvr_pci_ids,
	.probe		= pvr_probe,
	.remove		= pvr_remove,
	.shutdown	= pvr_shutdown,
};

static int __init pvr_init(void)
{
	int err;

	DRM_DEBUG_DRIVER("\n");

	pvr_drm_pci_driver = pvr_drm_generic_driver;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 18, 0))
	pvr_drm_pci_driver.set_busid = drm_pci_set_busid;
#endif

	err = PVRSRVCommonDriverInit();
	if (err)
		return err;

	return drm_pci_init(&pvr_drm_pci_driver, &pvr_pci_driver);
}

static void __exit pvr_exit(void)
{
	DRM_DEBUG_DRIVER("\n");

	drm_pci_exit(&pvr_drm_pci_driver, &pvr_pci_driver);

	PVRSRVCommonDriverDeinit();

	DRM_DEBUG_DRIVER("done\n");
}

module_init(pvr_init);
module_exit(pvr_exit);
