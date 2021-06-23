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
argc_argv_parse(int argc, char **argv) {

	//. this function requires the typedef struct _argc_argv, which has been
	//. placed in nvdock.h for this application.
	arg = (argstruct *)malloc(sizeof(argstruct));

	int a = 1;

	arg->version = 0;
	arg->help = 0;
	arg->unknown = 0;
	arg->fork = 1;
	arg->simple_menu = 0;
	arg->skip_load = 0;

	while(a < argc) {

		if(!strcmp("--version",argv[a])) {
			arg->version = 1;
		}

		else if(!strcmp("--parent",argv[a])) {
			arg->fork = 0;
		}

		else if(!strcmp("--simple-menu",argv[a])) {
			arg->simple_menu = 1;
		}

		else if(!strcmp("--help",argv[a])) {
			arg->help = 1;
		}

		else if(!strcmp("--no-load",argv[a])) {
			arg->skip_load = 1;
		}

		else {
			arg->help = 1;
			arg->unknown = 1;
		}

		++a;
	}
}

gboolean
exists_application(const char *string) {

	unsigned int len = (
		(sizeof(char) * strlen(CMD_WHICH_QUERY)) - 2) +
		(sizeof(char) * strlen(string) +
		1
	);

	char test[len];
	gboolean status;
	FILE *cmd;

	snprintf(test,len,CMD_WHICH_QUERY,string);

	//. as which sends output to stderr if the command was not found, if we get
	//. any data on stdout for the command it must be a path to the
	//. nvidia-settings executable. no data on stdin means it was not found.

	cmd = popen(test,"r");

	if(fgetc(cmd) != EOF) {
		status = TRUE;
	} else {
		status = FALSE;
	} pclose(cmd);

	return status;
}

gboolean
exists_icon_file(const char *string) {

	FILE *fp;

	fp = fopen(string,"r");
	if(fp) {
		fclose(fp);
		return TRUE;
	} else {
		return FALSE;
	}

}
