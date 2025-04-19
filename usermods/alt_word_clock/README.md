# Alt word clock mod for WLED

This is an 'alt word clock' usermod for the WLED firmware.
There are other word clock usermods for WLED, but they use different word positions and layouts.
Hence this is an `alt` (alternative) word clock.

## How it works

The word clock is implemented as an 'overlay'.
Specifically, the usermod simply disables - or 'turns off' - LEDs which should not be active at the current time.
It doesn't 'turn on' any additional pixels that are not already turned on due to the currently active preset or effect.

## Building

To include this usermod copy the `platformio_override.ini` file to the top of the repository source tree - so it is in the same directory as the `platformio.ini` file.

Then build the firmware as usual. E.g.

```
pio run -t upload
```

The above command is for those using PlatformIO via the command line. If you are using PlatformIO within Visual Studio Code the procedure might be a little different.

(Basically the procedure is the same as for building the WLED firmware in any other scenario. Just don't forget to copy the `platformio_override.ini` file as mentioned above.)

## Enabling

To enable the usermod you will need to open the `Config` section in the WLED web interface and then navigate to the `Usermods` page.
Scroll down to the `AltWordClock` section and click `Enabled`.
Then save your changes.

