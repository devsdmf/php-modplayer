/**
 * PHP CLI AUDIO STREAMMER
 *
 * This is a extension to provide a stream of module audio files under
 * the CLI SAPI. This library is licensed under the MIT license.
 * 
 * Developed by: devSDMF <devsdmf@gmail.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "php.h"
#include "php_modplayer.h"
#include "mikmod.h"

ZEND_DECLARE_MODULE_GLOBALS(modplayer)

static zend_function_entry modplayer_functions[] = {
    PHP_FE(modplayer_play, NULL)
    PHP_FE(modplayer_getpid, NULL)
    PHP_FE(modplayer_stop, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry modplayer_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_MODPLAYER_EXTNAME,
    modplayer_functions,
    PHP_MINIT(modplayer),
    PHP_MSHUTDOWN(modplayer),
    NULL,
    NULL,
    NULL,
    PHP_MODPLAYER_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_MODPLAYER
ZEND_GET_MODULE(modplayer)
#endif

static void php_modplayer_init_globals(zend_modplayer_globals *modplayer_globals)
{
    modplayer_globals->pid = 0;
}

PHP_MINIT_FUNCTION(modplayer)
{
    ZEND_INIT_MODULE_GLOBALS(modplayer, php_modplayer_init_globals, NULL);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(modplayer)
{
    if (MODPLAYER_G(pid) > 0) {
        kill(MODPLAYER_G(pid), SIGKILL);
    }

    return SUCCESS;
}

/* {{{ proto int modplayer_play(string file [, int max_channels [, bool curious [, int reverb]]])
   Play a module file from the disk and returns the PID of the player sub-process */
PHP_FUNCTION(modplayer_play)
{
    FILE *fptr;
    char *filename, resolved_path[MAXPATHLEN + 1];
    size_t filename_length;
    zend_long max_channels = 64;
    zend_bool curious = 0;
    zend_long reverb = 1;
    int s_pid;

    if (MODPLAYER_G(pid) > 0) {
        php_error_docref(NULL, E_ERROR, "You've already started the module player");
        RETURN_FALSE;
    }

    ZEND_PARSE_PARAMETERS_START(1, 4)
        Z_PARAM_PATH(filename, filename_length)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(max_channels)
        Z_PARAM_BOOL(curious)
        Z_PARAM_LONG(reverb)
    ZEND_PARSE_PARAMETERS_END();

    if (strlen(filename) == 0) {
        php_error_docref(NULL, E_ERROR, "You must specify a valid file name");
        RETURN_FALSE;
    }

    if (!expand_filepath(filename, resolved_path)) {
        php_error_docref(NULL, E_ERROR, "Could not resolve absolute file path");
        RETURN_FALSE;
    }

    if (access(resolved_path, F_OK) != -1) {
        fptr = fopen(resolved_path, "rb");
        if (fptr == NULL) {
            php_error_docref(NULL, E_ERROR, "An error occurred at try to open the module audio file specified");
            RETURN_FALSE;
        }

        s_pid = play_audio(fptr, max_channels, curious, reverb);

        if (s_pid > 0) {
            MODPLAYER_G(pid) = s_pid;
        }

        RETURN_LONG(s_pid);
    } else {
        php_error_docref(NULL, E_ERROR, "The specified module file does not exists");
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto int modplayer_getpid()
   Get the PID of the player sub-process, if the player is not running returns FALSE. */
PHP_FUNCTION(modplayer_getpid)
{
    if (MODPLAYER_G(pid) > 0) {
        RETURN_LONG(MODPLAYER_G(pid));
    } else {
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto bool modplayer_stop()
   Stop the module player.*/
PHP_FUNCTION(modplayer_stop)
{
    pid_t m_pid;

    m_pid = MODPLAYER_G(pid);

    if (m_pid > 0) {
        kill(m_pid, SIGKILL);
        MODPLAYER_G(pid) = 0;

        RETURN_TRUE;
    }

    php_error_docref(NULL, E_WARNING, "The player module was not started yet.");
    RETURN_FALSE;
}
/* }}} */

int play_audio(FILE *fptr, int maxchan, int curious, int reverb)
{
    pid_t m_pid;
    signed int s_pid;

    m_pid = fork();
    if (m_pid == 0) {
        MODULE *module;

        MikMod_InitThreads();
        MikMod_RegisterAllDrivers();
        MikMod_RegisterAllLoaders();

        md_mode |= DMODE_SOFT_MUSIC | DMODE_NOISEREDUCTION | DMODE_INTERP;
        
        if (reverb > 0) {
            md_reverb = reverb;
        }
        if (MikMod_Init("")) {
            php_error_docref(NULL, E_ERROR, "Could not initialize the MikMod library");
            return -1;
        }

        module = Player_LoadFP(fptr, maxchan, curious);
        if (module) {
            module->wrap = 1;
            module->loop = 0;

            Player_Start(module);

            s_pid = getpid();
            MODPLAYER_G(pid) = s_pid;

            while (Player_Active()) {
                usleep(10000);
                MikMod_Update();
            }
        } else {
            fclose(fptr);
            php_error_docref(NULL, E_ERROR, "Could not load module");
            return -1;
        }

        MikMod_Exit();
    } else if (m_pid < 0) {
        php_error_docref(NULL, E_ERROR, "Failed to fork CLI audio stream process");
        return -1;
    }

    return m_pid;
}
