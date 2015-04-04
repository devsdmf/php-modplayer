PHP_ARG_ENABLE(cli_audio, whether to enable CLI Audio Stream support,
[ --enable-cli-audio   Enable CLI Audio Stream support])

if test "$PHP_CLI_AUDIO" = "yes"; then
  AC_DEFINE(HAVE_CLI_AUDIO, 1, [Whether you have CLI Audio Stream])
  PHP_NEW_EXTENSION(cli_audio, cli_audio.c, $ext_shared)
fi