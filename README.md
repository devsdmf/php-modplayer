# CLI Audio Stream for PHP

A PHP extension to stream audio through CLI applications.

# Installation

Compile and install the libmikmod ([download here](http://mikmod.sourceforge.net/)):

```
./configure
make
sudo make install
```

Now, you must compile and install the extension:

```
git clone git@github.com:devsdmf/php-cli-audio.git
cd php-cli-audio

phpize
./configure --with-cli-audio
make
sudo make install
```

# Usage

```php
// Start the audio stream
cli_audio_stream("audio.xm");

// Get the PID of the streammer process
cli_audio_getpid();

// Stop the stream
cli_audio_stop();
```

# Supported Formats

For now the CLI stream was build to support .xm, .mod and .it files (module audio files).

# License

This library is licensed under the [MIT license](LICENSE).