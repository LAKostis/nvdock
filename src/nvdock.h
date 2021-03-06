/*//. nvDock

Copyright (c) 2007, Bob Majdak Jr <bob@kateos.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.
      
	* Neither the name of the organization nor the names of its contributors
	may be used to endorse or promote products derived from this software
	without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

The NVIDIA name and Logo are property of NVIDIA.
.//*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>
#include <gtk/gtk.h>

#define VERSION "1.02"

#define CMD_SUFFIX " &> /dev/null &"
#define CMD_NVCLOCK_GTK "nvclock_gtk"
#define CMD_NVIDIA_RELOAD "nvidia-settings -load-config-only"
#define CMD_NVIDIA_SETTINGS "nvidia-settings"
#define CMD_NVIDIA_TEMP "nvidia-settings -q GPUCoreTemp | grep Attribute | cut -d ' ' -f 6 | cut -d '.' -f 1"
#define CMD_NVIDIA_LOAD "nvidia-settings -load-config-only"
#define CMD_NVIDIA_VERSION "nvidia-settings -q NvidiaDriverVersion | grep Attribute | cut -d : -f 3 | cut -d ' ' -f 2"
#define CMD_WHICH_QUERY "which %s 2> /dev/null"

#define ICON "/usr/share/pixmaps/nvdock.png"
#define REFRESH_ICON "GTK_STOCK_REFRESH"
#define VERSION_PRINTF "nvDock v%s\nCopyright (c) 2007 Bob Majdak Jr <bob@kateos.org>\nReleased under the BSD License.\n"

typedef struct _BobStatusIcon {
	GtkStatusIcon *icon;
	
	GtkWidget *menu, **item;
	
	gulong signal_activate;
	gulong signal_popup_menu;

	unsigned long last_time;
	unsigned short tooltip_interval;
	gboolean first_right_click;
} BobStatusIcon;

typedef struct _argc_argv {

	unsigned char version:1;
	unsigned char help:1;
	unsigned char unknown:1;
	unsigned char fork:1;
	unsigned char simple_menu:1;
	unsigned char skip_load:1;
	
	gboolean has_nvclock:1;
	gboolean has_nvclock_gtk:1;
	
	char nvversion[32];

} argstruct;

extern argstruct *arg;
extern BobStatusIcon *bsi;
void argc_argv_parse(int argc, char **argv);

gboolean exists_application(const char *);
gboolean exists_icon_file(const char *);

int read_nvidia_temp(void);
void read_nvidia_version(char *);
unsigned char bob_clean_string(char *);

void bob_main_quit(void);

void bob_status_icon_new(BobStatusIcon *, int tooltip_interval);
void bob_status_icon_on_tooltip_interval(void);
void bob_status_icon_update_tooltip(const char *);
void bob_status_icon_exec_system(const char *);

void bob_status_icon_on_activate(void);
void bob_status_icon_on_popup_menu(GtkStatusIcon *, guint, guint);
