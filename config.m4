LDFLAGS=`libmikmod-config --libs`

PHP_ARG_WITH(modplayer, for module audio files support,
[  --with-modplayer[=DIR]       include module player support])

if test "$PHP_MODPLAYER" != "no"; then
	for i in $PHP_MODPLAYER /usr/local /usr; do
		test -f $i/include/mikmod.h && MIKMOD_DIR=$i && break
	done

	if test -z "$PHP_MODPLAYER"; then
		AC_MSG_ERROR(mikmod.h not found. Please reinstall libmikmod.)
	fi

	PHP_ADD_LIBRARY_WITH_PATH(modplayer, $MIKMOD_DIR/$PHP_LIBDIR)
	PHP_ADD_INCLUDE($MIKMOD_DIR/include)

	PHP_NEW_EXTENSION(modplayer, modplayer.c, $ext_shared, cli,)
fi