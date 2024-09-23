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

# Configuration
* `batstat` uses default global config file `\etc\batstat.json`
* a user config file is searched before the global config, under `~/.config/batstat.json`. 
Note that this file is not automatically created, a user should manually create it.
* For changes in the configuration, eighter modify the global config, or create a user config file.
* The default configuration file can be seen below:
```json
{
  // folder for finding battery info. By default "/sys/class/power_supply/"
  "power_supply_path" : "/sys/class/power_supply/",
  
  // refresh rate in seconds, integer value between 1 and 120. If provided value is not valid, 3, is used by default
  "refresh_rate" : 2,

  // battery history log refresh rate in seconds, integer value between 1 and 120. If provided value is not valid, 20, is used by default
  "log_rate" : 3
}
```

# Limitations
* Only 1 battery supported
* On arch systems (or other distros which use current/charge/voltage instead of energy/power) the program will now show the battery capacity in Ah and discharge in A. A fix for this is not obvious since 

# Dependencies
* ncurses C++ library

# Compilation command
```sh
$ make
```
