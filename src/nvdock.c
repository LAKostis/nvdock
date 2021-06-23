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

void
bob_main_quit(void) {
	gtk_main_quit();

	return;
}

int main(int argc, char **argv) {

	if(!exists_application("nvidia-settings")) {
		puts("nvidia settings was not found.");
		return 1;
	}

	if(!exists_icon_file(ICON)) {
		printf("icon file '%s' does not exist.\n",ICON);
		return 2;
	}

	unsigned int pid = 0;

	argc_argv_parse(argc,argv);

	if(arg->version) {
		printf(VERSION_PRINTF,VERSION);
		return 0;
	}

	if(arg->help) {
		if(arg->unknown) {
			puts("error: unknown options found.\n");
		}
		printf(VERSION_PRINTF,VERSION);
		puts("\nUsage: nvdock [OPTIONS]");
		puts("    --no-load        Do not automatically reload video settings on start.");
		puts("    --parent         Do not fork into a background process.");
		puts("    --simple-menu    Disable menu icons (less memory use).");
		puts("    --version        Display version info.");
		puts("    --help           Display this help.\n");
		return 0;
	}

	int num_gpus = 0;
	num_gpus = read_gpus();
	if(num_gpus == 0) {
		puts("No NVIDIA GPUs detected, quitting.");
		return 1;
	}

	if(arg->fork) {
		if((pid = fork()) < 0) {
			puts("unable to fork, quitting.");
			return 3;
		}
	}

	if(pid == 0) {
		gtk_init(NULL,NULL);

		BobStatusIcon s;
		bsi = &s;

		get_nvidia_version(0);

		bob_status_icon_new(&s,30);

		if(!arg->skip_load) {
			bob_status_icon_exec_system(CMD_NVIDIA_LOAD);
		}

		while(gtk_events_pending()) {
			gtk_main_iteration_do(TRUE);
		}

		gtk_main();
	}

	free(arg);

	return 0;
}
