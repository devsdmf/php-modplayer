/**
 * PHP CLI AUDIO STREAMMER
 *
 * This is a extension to provide a stream of module audio files under
 * the CLI SAPI. This library is licensed under the MIT license.
 * 
 * Developed by: devSDMF <devsdmf@gmail.com>
 *
 */
#ifndef PHP_MOD_PLAYER_H
#define PHP_MOD_PLAYER_H 1

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(mod_player)
long pid;
ZEND_END_MODULE_GLOBALS(mod_player)

#ifdef ZTS
#define MOD_PLAYER_G(v) TSRM(mod_player_globals_id, zend_mod_player_globals *, v)
#else
#define MOD_PLAYER_G(v) (mod_player_globals.v)
#endif

#define PHP_MOD_PLAYER_EXTNAME "mod_player"
#define PHP_MOD_PLAYER_VERSION "1.1.0"

PHP_MINIT_FUNCTION(mod_player);
PHP_MSHUTDOWN_FUNCTION(mod_player);

PHP_FUNCTION(play_module_file);
PHP_FUNCTION(mod_player_getpid);
PHP_FUNCTION(stop_module_file);

int stream_audio(FILE *fptr, int maxchan, int curious);

extern zend_module_entry mod_player_module_entry;
#define phpext_mod_player_ptr &mod_player_module_entry;

#endif