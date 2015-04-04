#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_cli_audio.h"
#include "stream.h"

ZEND_DECLARE_MODULE_GLOBALS(cli_audio)

static zend_function_entry cli_audio_functions[] = {
	PHP_FE(cli_audio_stream, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry cli_audio_module_entry {
	#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
	#endif
	PHP_CLI_AUDIO_EXTNAME,
	cli_audio_functions,
	PHP_MINIT(cli_audio),
	PHP_MSHUTDOWN(cli_audio),
	NULL,
	NULL,
	NULL,
	#if ZEND_MODULE_API_NO >= 20010901
	PHP_CLI_AUDIO_VERSION,
	#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CLI_AUDIO
ZEND_GET_MODULE(cli_audio)
#endif;

PHP_INI_BEGIN()
PHP_INI_ENTRY("cli_audio.maxchan","64",PHP_INI_ALL,NULL)
PHP_INI_ENTRY("cli_audio.curious","0",PHP_INI_ALL,NULL)
PHP_INI_END()

static void php_cli_audio_init_globals(zend_cli_audio_globals *cli_audio_globals)
{
	cli_audio_globals->pid     = 0;
	cli_audio_globals->maxchan = 64;
	cli_audio_globals->curious = 0;
}

PHP_MINIT_FUNCTION(cli_audio)
{
	ZEND_INIT_MODULE_GLOBALS(cli_audio, php_cli_audio_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(cli_audio)
{
	if (CLI_AUDIO_G(pid) > 0) {
		 kill(pid, SIGKILL);
	}
	
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}


PHP_FUNCTION(cli_audio_stream)
{
	zval *filename;
	FILE *fptr;
	signed int pid;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &filename) == FAILURE) {
        RETURN_NULL();
    }

    convert_to_string(filename);

	fptr = fopen(filename, "rb");
	if (fptr == NULL) {
        perror("fopen");
        MikMod_Exit();
        zend_error(E_ERROR, "An error occurred at try to open the audio file specified");
        RETURN_NULL();
    }

    pid = stream_audio(fptr, INI_LONG("cli_audio.maxchan"), INI_LONG("cli_audio.curious"));
    if (pid > 0) {
    	CLI_AUDIO_G(pid) = pid;
    }
}