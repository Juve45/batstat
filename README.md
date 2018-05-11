# batstat
CLI battery status for linux

# Features
* Current battery level
* Current Energy
* Full charge energy
* Time elapsed from the start of the program, without tracking the sleep time of the machine.
* Battery level history

# Install
* Download the "batstat" file from the "bin" folder, and run it in your machine

# Limitations
* Only 1 battery supported
* Gathers informations only from this folder: "/sys/class/power_supply/".
If your machine contains the battery information on a different folder, this program will not work.

# Dependencies
* ncurses C++ library

# Compilation command
g++ main.cpp -lncurses -pthread -std=c++11 -o batstat
