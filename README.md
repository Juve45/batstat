# batstat
CLI battery status for linux

# Features
* Current battery level
* Current Energy
* Full charge energy
* Time elapsed from the start of the program, without tracking the sleep time of the machine
* Battery level history

![Screenshot from application](example.png)

# Install
```sh

$ sudo apt-get install make g++ libncurses5-dev
$ sudo make build
$ sudo make install

```

# Limitations
* Only 1 battery supported
* Gathers data only from `/sys/class/power_supply/`
  If your machine contains the battery information in a different folder, this program will not work.
* On arch systems (or other distros which use current/charge/voltage instead of energy/power) the program will now show the battery capacity in Ah and discharge in A. A fix for this is not obvious since 

# Dependencies
* ncurses C++ library

# Compilation command
```sh
$ make
```
