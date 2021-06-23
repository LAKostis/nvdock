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

#define BOB_MENU_HBAR 1
#define BOB_MENU_ITEM 2
#define BOB_MENU_ICON 3

typedef struct _popup_menu_conf {

	int id;
	int type;
	char label[64];
	char command[64];
	char icon[64];

	int parent;
	gboolean clickable;

	void (*callback)();

} BobMenuItemDef;

/*//.
	All (BOB_MENU_ITEM | BOB_MENU_ICON) that should have no actions should have callback = NULL;
	All BOB_MENU_HBAR should have empty label, command, icon, and a NULL callback.
.//*/

BobMenuItemDef menuItemDef[] = {
//	{ id, type, label, command, icon, parent id, clickable, callback }
	{ 0, BOB_MENU_ITEM, "NVIDIA Dock", "", ICON, -1, FALSE, NULL },
	{ 1, BOB_MENU_HBAR, "", "", "", -1, FALSE, NULL },
	{ 10, BOB_MENU_ITEM, "<nvversion>", "", "", -1, FALSE, NULL },
	{ 2, BOB_MENU_ITEM, "<nvtemp>", "", "", -1, FALSE, NULL },
	{ 3, BOB_MENU_HBAR, "", "", "", -1, FALSE, NULL },
	{ 4, BOB_MENU_ICON, "NVIDIA Settings...", CMD_NVIDIA_SETTINGS, "gtk-preferences", -1, TRUE, bob_status_icon_exec_system },
	{ 6, BOB_MENU_HBAR, "", "", "", -1, FALSE, NULL },
	{ 7, BOB_MENU_ICON, "Reload Settings", CMD_NVIDIA_RELOAD, "gtk-refresh", -1, TRUE, bob_status_icon_exec_system },
	{ 8, BOB_MENU_HBAR, "", "", "", -1, FALSE, NULL },
	{ 9, BOB_MENU_ICON, "Quit", "", "gtk-quit", -1, TRUE, bob_main_quit }
};

argstruct *arg;
BobStatusIcon *bsi;

#define BOB_NVTEMP_ITEM 2
#define BOB_NVCLOCK_ITEM 5
#define BOB_NVVERSION_ITEM 10
