#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "php.h"
#include "stream.h"
#include "mikmod.h"

int stream_audio(FILE *file, int maxchan, int curious)
{
	signed int s_pid = 0;
	pid_t pid = fork();

	s_pid = getpid();

	if (pid == 0) {
		MODULE *module;

		MikMod_InitThreads();
        MikMod_RegisterAllDrivers();
        MikMod_RegisterAllLoaders();

        md_mode |= DMODE_SOFT_MUSIC | DMODE_NOISEREDUCTION | DMODE_INTERP;
        if (MikMod_Init("")) {
            // fprintf(stderr, "Could not initialize sound, reason: %s\n",MikMod_strerror(MikMod_errno));
            zend_error(E_ERROR, "Could not initialize the MikMod library");
            return;
        }

        module = Player_LoadFP(file, maxchan, curious);
        if (module) {
        	module->wrap = 1;
        	module->loop = 0;

        	Player_Start(module);

        	while (Player_Active()) {
        		usleep(10000);
        		MikMod_Update();
        	}
        } else {
        	zend_error(E_ERROR, "Could not load module");
        }

        MikMod_Exit();
	} else if (pid < 0) {
		zend_error(E_ERROR, "Failed to fork CLI Audio Stream process");
		exit(1);
	}

	return s_pid;
}