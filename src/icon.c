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

#include "nvdock.h"
#include "icon.h"

void
bob_status_icon_new(BobStatusIcon *s, int tooltip_interval) {

	s->icon = gtk_status_icon_new_from_file(ICON);

	s->signal_activate = g_signal_connect_swapped(
		G_OBJECT(s->icon),
		"activate",
		G_CALLBACK(bob_status_icon_on_activate),
		s
	);

	s->signal_popup_menu = g_signal_connect_swapped(
		G_OBJECT(s->icon),
		"popup-menu",
		G_CALLBACK(bob_status_icon_on_popup_menu),
		s
	);

	if(get_nvidia_temp(0)) {
		g_timeout_add(
			(tooltip_interval * 1000),
			(GSourceFunc)&bob_status_icon_on_tooltip_interval,
			s
		); bob_status_icon_on_tooltip_interval();

		s->tooltip_interval = 30;
	} else {
		s->tooltip_interval = 0;
	}

	s->last_time = 0;
	s->first_right_click = TRUE;

	return;
}

void
bob_status_icon_on_activate(void) {
	unsigned long now = time(NULL);

	if((now - bsi->last_time) < 1) {
		bob_status_icon_exec_system(CMD_NVIDIA_SETTINGS);
	}

	bsi->last_time = now;

	return;
}

void
bob_status_icon_on_popup_menu(GtkStatusIcon *icon, guint button, guint when) {

	char nvtempstring[255],nvversion[255];
	GtkWidget *menu, *item;
	menu = gtk_menu_new();
	void *temp;
	int a, idx_gpu;
	double nvtemp;

	num_gpus = read_gpus();
	for (idx_gpu = 0; idx_gpu < num_gpus; idx_gpu++) {
        	//. read/format the temperature.
        	nvtemp = get_nvidia_temp(idx_gpu);
        	if(nvtemp) {
        		snprintf(
        			nvtempstring, 255,
        			"%d: %s Temp: %.2f°\n",
        			idx_gpu,feature[idx_gpu]->name,nvtemp
        		);
		}
		bob_status_icon_update_tooltip(nvtempstring);
	}

	//. format the nvidia version.
	snprintf(
		nvversion, 255,
		"Driver: %s",
		arg->nvversion
	);


	if(bsi->first_right_click == FALSE) {
		//. this will free all the menu items and the containment menu from
		//. the last click.
		gtk_widget_destroy(bsi->menu);
	} else {
		bsi->first_right_click = FALSE;
	}

	//. if we had not got rid of the old menu every right click would drive RAM
	//. use higher and higher. i know, i watched it happen. because the menu
	//. items are added to the menu we can forget their reference here.

	BobMenuItemDef *def;
	int menusize = (sizeof(menuItemDef) / sizeof(BobMenuItemDef));
	for(a = 0; a < menusize; a++) {
		def = &menuItemDef[a];

		if(def->type == BOB_MENU_ICON && arg->simple_menu) {
			def->type = BOB_MENU_ITEM;
		}

		//. normal menu items.
		switch(def->type) {
			case(BOB_MENU_ITEM): {

				//. special cases
				if(def->id == BOB_NVVERSION_ITEM) {
					strcpy(def->label,nvversion);
				}
				else if(def->id == BOB_NVTEMP_ITEM && nvtemp) {
					strcpy(def->label,nvtempstring);
				}

				item = gtk_menu_item_new_with_label(def->label);

				if(def->callback != NULL) {
					g_signal_connect_swapped(G_OBJECT(item),"activate",
						G_CALLBACK(def->callback),def->command);
				}

				gtk_widget_set_sensitive(GTK_WIDGET(item),def->clickable);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
				break;
			}

			//. menu items with icons.
			case(BOB_MENU_ICON): {
				item = gtk_image_menu_item_new_with_label(def->label);

				if(def->icon != NULL) {
					if(def->icon[0] != '/') {
						gtk_image_menu_item_set_image(
							GTK_IMAGE_MENU_ITEM(item),
							gtk_image_new_from_stock(def->icon,GTK_ICON_SIZE_MENU)
						);
					} else {
						if(exists_icon_file(def->icon)) {

							temp = (GtkWidget *)gtk_image_new_from_file(def->icon);
							gtk_image_set_pixel_size(GTK_IMAGE(temp),GTK_ICON_SIZE_MENU);

							gtk_image_menu_item_set_image(
								GTK_IMAGE_MENU_ITEM(item),
								temp
							);

							//. GtkImageMenuItem now has the reference so
							//. we can just forget it.
							temp = NULL;
						}
					}
				}

				if(def->callback != NULL) {
					g_signal_connect_swapped(G_OBJECT(item),"activate",
						G_CALLBACK(def->callback),def->command);
				}

				gtk_widget_set_sensitive(GTK_WIDGET(item),def->clickable);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
				break;
			}

			//. menu separator.
			case(BOB_MENU_HBAR): {
				item = gtk_separator_menu_item_new();
				gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
				break;
			}
		}

	}

	gtk_widget_show_all(GTK_WIDGET(menu));

	gtk_menu_popup(
		GTK_MENU(menu),
		NULL, NULL,
		gtk_status_icon_position_menu,
		bsi->icon,
		button,
		when
	);

	//. keeping the menu references.
	bsi->menu = menu;

	return;
}

void
bob_status_icon_on_tooltip_interval(void) {

	int nvtemp, idx_gpu;
	char string[255];

	num_gpus = read_gpus();
	for (idx_gpu = 0; idx_gpu < num_gpus; idx_gpu++) {
        	//. read/format the temperature.
        	nvtemp = get_nvidia_temp(idx_gpu);
        	if(nvtemp) {
        		snprintf(
        			string, 255,
        			"%d: %s Temp: %.2f°",
        			idx_gpu,feature[idx_gpu]->name,nvtemp
        		);
		}
	}
	bob_status_icon_update_tooltip(string);

	return;
}

void
bob_status_icon_update_tooltip(const char *text) {

	gtk_status_icon_set_tooltip(GTK_STATUS_ICON(bsi->icon), text);

	return;
}

void bob_status_icon_exec_system(const char *string) {

	unsigned int len = (
		(sizeof(char) * strlen(string)) +
		(sizeof(char) * strlen(CMD_SUFFIX)) +
		1
	);

	char test[len];

	snprintf(test,len,"%s%s",string,CMD_SUFFIX);

	g_signal_handler_block(G_OBJECT(bsi->icon), bsi->signal_activate);
	g_signal_handler_block(G_OBJECT(bsi->icon), bsi->signal_popup_menu);

	system(test);

	while(gtk_events_pending()) {
		gtk_main_iteration_do(TRUE);
	}

	g_signal_handler_unblock(G_OBJECT(bsi->icon), bsi->signal_activate);
	g_signal_handler_unblock(G_OBJECT(bsi->icon), bsi->signal_popup_menu);

	return;

}
