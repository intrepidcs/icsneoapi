icsneoapi
=========

An open source library for communicating with vehicle network tools developed by Intrepid Control Systems.

Requires libftdi1.

```$ sudo apt-get install libftdi1
```

On some distros (for example Ubuntu 14.04) libftdi1 doesn't create a symlink correctly for 64-bit versions. 
You'll need to point libftdi.so -> libftdi.so.1.

```$ cd /usr/lib/x86_64-linux-gnu
$ sudo ln -s libftdi.so.1 libftdi.so
```