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
* Download the [`batstat`](batstat) file and run it.
* You may copy this file into `/usr/bin` (or another folder in your `$PATH`) to run it from anywhere.

# Limitations
* Only 1 battery supported
* Gathers data only from `/sys/class/power_supply/`
  If your machine contains the battery information in a different folder, this program will not work.

# Dependencies
* ncurses C++ library

# Compilation command
```sh
g++ main.cpp -lncurses -pthread -std=c++11 -o batstat
```
