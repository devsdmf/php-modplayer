/**
 * PHP CLI AUDIO STREAMMER
 *
 * This is a extension to provide a stream of module audio files under
 * the CLI SAPI. This library is licensed under the MIT license.
 * 
 * Developed by: devSDMF <devsdmf@gmail.com>
 *
 */
#ifndef PHP_MODPLAYER_H
#define PHP_MODPLAYER_H 1

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(modplayer)
long pid;
ZEND_END_MODULE_GLOBALS(modplayer)

#ifdef ZTS
#define MODPLAYER_G(v) TSRM(modplayer_globals_id, zend_modplayer_globals *, v)
#else
#define MODPLAYER_G(v) (modplayer_globals.v)
#endif

#define PHP_MODPLAYER_EXTNAME "modplayer"
#define PHP_MODPLAYER_VERSION "1.1.0"

PHP_MINIT_FUNCTION(modplayer);
PHP_MSHUTDOWN_FUNCTION(modplayer);

PHP_FUNCTION(play_module_file);
PHP_FUNCTION(mod_player_getpid);
PHP_FUNCTION(stop_module_file);

int stream_audio(FILE *fptr, int maxchan, int curious);

extern zend_module_entry modplayer_module_entry;
#define phpext_mod_player_ptr &modplayer_module_entry;

#endif