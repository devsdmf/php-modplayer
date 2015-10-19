# PHP Module Player

A PHP extension to play module audio files in CLI environment.

# Installation

Compiling the libmikmod:

```
cd lib/libmikmod-3.3.7/
mkdir build && cd !$
../configure
make
sudo make install
```

Now, you must compile and install the extension:

```
phpize
./configure
make
sudo make install
```

# Usage

```php
// Start the audio stream
play_module_file("audio.xm");

// Get the PID of the streammer process
mod_player_getpid();

// Stop the stream
stop_module_file();
```

# Supported Formats

For now this extension supports .xm, .mod and .it files (module audio files).

# License

This library is licensed under the [MIT license](LICENSE).