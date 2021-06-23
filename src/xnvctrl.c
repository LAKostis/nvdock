/* File: nvidia.c
 *
 * Copyright (c) 2011 Amir Aupov <fads93@gmail.com>
 * Copyright (c) 2017 Fabian Nowak <timystery@arcor.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <nvdock.h>

#include <X11/Xlib.h> /* Must be before NVCtrl includes */
#include <NVCtrl/NVCtrl.h>
#include <NVCtrl/NVCtrlLib.h>

/* Global variables */
Display *nvidia_display;

#define ZERO_KELVIN -273 /*.15 */

/* -------------------------------------------------------------------------- */
double
get_nvidia_temp (int idx_gpu)
{
    int temperature = 0;
    double result = ZERO_KELVIN;

    if (XNVCTRLQueryTargetAttribute (nvidia_display,
                                     NV_CTRL_TARGET_TYPE_GPU,
                                     idx_gpu,
                                     0,
                                     NV_CTRL_GPU_CORE_TEMPERATURE,
                                     &temperature)) {
        result = (double) (1.0 * temperature);
    }

    return result;
}

/* -------------------------------------------------------------------------- */
void
get_nvidia_version(int idx_gpu)
{
    gchar* driver_version = NULL;

    /* create the connection to the X server */
    nvidia_display = XOpenDisplay (NULL);
    if (nvidia_display) {

        if (XNVCTRLQueryTargetStringAttribute (nvidia_display,
                                           NV_CTRL_TARGET_TYPE_GPU,
                                               idx_gpu,
                                               0,
                                               NV_CTRL_STRING_NVIDIA_DRIVER_VERSION,
                                               &driver_version)) {
            arg->nvversion = driver_version;
        }
    }
}

/* -------------------------------------------------------------------------- */
int
read_gpus (void)
{
    int found = 0;
    int event, error;
    int idx_gpu;

    /* create the connection to the X server */
    nvidia_display = XOpenDisplay (NULL);
    if (nvidia_display) {

        /* check if the NVCtrl is available on this X server
         * if so - add sensors*/
        if (XNVCTRLQueryExtension (nvidia_display, &event, &error)) {
            XNVCTRLQueryTargetCount (nvidia_display,
                NV_CTRL_TARGET_TYPE_GPU,
                &found);
        }
    }

    for (idx_gpu = 0; idx_gpu < found; idx_gpu++) {
        gchar* gpuname = NULL; /* allocated by libxnvctrl */
        feature[idx_gpu] = g_new0 (t_nvfeature, 1);

        if (XNVCTRLQueryTargetStringAttribute (nvidia_display,
                                               NV_CTRL_TARGET_TYPE_GPU,
                                               idx_gpu,
                                               0,
                                               NV_CTRL_STRING_PRODUCT_NAME,
                                               &gpuname)) {
            g_assert (gpuname != NULL);
            feature[idx_gpu]->devicename = gpuname;  /* "it is the caller's responsibility to free ..." */
        }
        else
        {
            feature[idx_gpu]->devicename = g_strdup_printf ("GPU %d", idx_gpu);
        }
        feature[idx_gpu]->name = g_strdup (feature[idx_gpu]->devicename);
    }

    return found;
}
