#ifndef PHP_CLI_AUDIO_H
#define PHP_CLI_AUDIO_H 1

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(cli_audio)
long pid;
ZEND_END_MODULE_GLOBALS(cli_audio)

#ifdef ZTS
#define CLI_AUDIO_G(v) TSRM(cli_audio_globals_id, zend_cli_audio_globals *, v)
#else
#define CLI_AUDIO_G(v) (cli_audio_globals.v)
#endif

#define PHP_CLI_AUDIO_EXTNAME "cli_audio"
#define PHP_CLI_AUDIO_VERSION "1.0.0"

PHP_MINIT_FUNCTION(cli_audio);
PHP_MSHUTDOWN_FUNCTION(cli_audio);

PHP_FUNCTION(cli_audio_stream);
PHP_FUNCTION(cli_audio_getpid);

extern zend_module_entry cli_audio_module_entry;
#define phpext_cli_audio_ptr &cli_audio_module_entry;

#endif