LDFLAGS=`libmikmod-config --libs`

PHP_ARG_WITH(cli_audio, for cli audio stream support,
[  --with-cli-audio[=DIR]       include cli audio support])

if test "$PHP_CLI_AUDIO" != "no"; then
	for i in $PHP_CLI_AUDIO /usr/local /usr; do
		test -f $i/include/mikmod.h && MIKMOD_DIR=$i && break
	done

	if test -z "$PHP_CLI_AUDIO"; then
		AC_MSG_ERROR(mikmod.h not found. Please reinstall libmikmod.)
	fi

	PHP_ADD_LIBRARY_WITH_PATH(cli_audio, $MIKMOD_DIR/$PHP_LIBDIR)
	PHP_ADD_INCLUDE($MIKMOD_DIR/include)

	PHP_NEW_EXTENSION(cli_audio, cli_audio.c, $ext_shared,,)
	PHP_ADD_MAKEFILE_FRAGMENT
fi