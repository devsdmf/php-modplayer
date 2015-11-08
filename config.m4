LDFLAGS=`libmikmod-config --libs`

PHP_ARG_WITH(mod_player, for module audio files support,
[  --with-mod-player[=DIR]       include cli audio support])

if test "$PHP_MOD_PLAYER" != "no"; then
	for i in $PHP_MOD_PLAYER /usr/local /usr; do
		test -f $i/include/mikmod.h && MIKMOD_DIR=$i && break
	done

	if test -z "$PHP_MOD_PLAYER"; then
		AC_MSG_ERROR(mikmod.h not found. Please reinstall libmikmod.)
	fi

	PHP_ADD_LIBRARY_WITH_PATH(mod_player, $MIKMOD_DIR/$PHP_LIBDIR)
	PHP_ADD_INCLUDE($MIKMOD_DIR/include)

	PHP_NEW_EXTENSION(mod_player, mod_player.c, $ext_shared,,)
fi