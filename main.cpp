#include <bits/stdc++.h>
#include <unistd.h>
#include <curses.h>
#include <string>
#include <dirent.h>

#define LOG_P 20
using namespace std;

int refreshRate = 3;
//const string logFile = "log.txt";

const string PATH = "/sys/class/power_supply/";

float maxEnergy, currentPower, currentEnergy, initEnergy;
int initTime, percent, timeNow, timeElapsed = -refreshRate;
int lastTime = 0, logIndex = 0;

string Path = PATH, status, energyPath, powerPath;
string chargePath, voltagePath, currentPath;
char powerUnit;

bool quit = false;
WINDOW * mainwin;
vector <string> logCache;

bool checkdir(string str) {
	DIR* dir = opendir(str.c_str());
	if (dir) {
		closedir(dir);
		return true;
	}
	return false;
}

void init()
{

  if(checkdir(Path + "BAT0")) {
    Path += "BAT0/";
  } else {
    Path += "BAT1/";
  }

  chargePath = Path + "charge";
  voltagePath = Path + "voltage";
  currentPath = Path + "current";

  energyPath = Path + "energy";
  powerPath = Path + "power";

  if(checkdir(powerPath) or (checkdir(voltagePath) and checkdir(currentPath)))
  	powerUnit = 'W';
  else
  	powerUnit = 'A';

  if ( (mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initializing ncurses.\n");
    exit(EXIT_FAILURE);
  }
  keypad(stdscr, TRUE);
  
  if(has_colors() == FALSE) {
    printf("Your terminal does not support color\n");
  }
  start_color();

  assume_default_colors(COLOR_WHITE,COLOR_BLACK);

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);

  bkgd(COLOR_PAIR(1));

  ifstream enNow(energyPath + "_now");
  enNow >> initEnergy;
  
  initTime = time(NULL);
  
  ifstream maxEnergyFile(energyPath + "_full");
  maxEnergyFile >> maxEnergy;
}

void refreshValues()
{
	if(checkdir(powerPath)) { // Unit is Watts
	  ifstream powNow(powerPath + "_now");
	  ifstream enNow(energyPath + "_now");
	  powNow >> currentPower;
	  enNow >> currentEnergy;
	} else {
			ifstream currNow(currentPath + "_now");
		  ifstream chgNow(chargePath + "_now");
		  ifstream st(Path + "status");
		  
		  currNow >> currentPower;
		  chgNow >> currentEnergy;
		if(checkdir(voltagePath)) { // Unit is Watts
			float voltage;
		  ifstream volNow(voltagePath + "_now");
		  volNow >> voltage;
		  currentPower *= voltage / 1000000;
		  currentEnergy *= voltage / 1000000;
		} // Else unit is Amps
	} 

	ifstream st(Path + "status");
  timeNow = time(NULL);
  //timeElapsed = timeElapsed + timeNow - lastTime;
  timeElapsed += refreshRate;
  lastTime = timeNow;
  st >> status;
}

void newPrint()
{
  char buff[255];
  erase();
  sprintf(buff, "%-30s\n", "Status: ");
  mvaddstr(0, 0, buff);
  
  if(status[0] == 'C')
    attron(COLOR_PAIR(3));
  else
    attron(COLOR_PAIR(2));
  mvaddstr(0, strlen(buff) - 1, status.c_str());

  attron(COLOR_PAIR(1));
  sprintf(buff, "%-30s%.2lf %ch\n", "Max energy:", powerUnit, maxEnergy/1000000);
  mvaddstr(1, 0, buff);
  sprintf(buff, "%-30s%.2lf %ch\n", "Energy left:", powerUnit, currentEnergy/1000000);
  mvaddstr(2, 0, buff);
  sprintf(buff, "%-30s%.2lf %c\n", "Power Consumption:", powerUnit, currentPower/1000000);
  mvaddstr(3, 0, buff);
  sprintf(buff, "%-30s%.2lf%%\n", "Percentage left:", currentEnergy/maxEnergy*100);
  mvaddstr(4, 0, buff);
  sprintf(buff, "%-30s%.2lf %c\n", "Average power Consumption:", powerUnit, (initEnergy - currentEnergy) / 1000000 / (1. * timeElapsed / 3600.));
  ofstream ferr("log.txt");
  ferr << initEnergy - currentEnergy << " Whr" << endl;
  ferr << (1. * timeElapsed / 3600.) << " s" << endl;
  mvaddstr(5, 0, buff);
  sprintf(buff, "%-30s%2d:%2d:%2d since %.2lf%%\n", "Time elapsed:", timeElapsed/3600, (timeElapsed/60)%60, timeElapsed%60, initEnergy/maxEnergy*100);
  mvaddstr(6, 0, buff);

  sprintf(buff, "= Time   ======== Percent ============================================\n");
  mvaddstr(7, 0, buff);

  for(int i = logIndex; i < logCache.size(); i++)
    mvaddstr(8 + i - logIndex, 0, logCache[i].c_str());

  refresh();
}

void addLog()
{
  char buff[200];
  sprintf(buff, "%2d:%2d:%2d          %.2lf\%\n", timeElapsed/3600, (timeElapsed/60)%60, timeElapsed%60, currentEnergy/maxEnergy*100);
  logCache.push_back(buff);
}

void keyListenerFunction()
{
  int ch = 0;
  while((ch = wgetch(mainwin)) != 'q')
  {
    if(ch == 27)
      break;

    switch ( ch ) {

    case KEY_UP:
      if (logIndex > 0)
      	--logIndex;
      newPrint();
      break;

    case KEY_DOWN:
      if (logIndex + 1 < logCache.size())
      	++logIndex;
      newPrint();
      break;
    }
  }
  quit = 1;
  return;
}

void coreFunction() {
  lastTime = time(NULL);
  // sleep(1);
  while(!quit) {
    refreshValues();
    newPrint();
    if(timeElapsed % LOG_P == 0)
      addLog();
    sleep(refreshRate);
  }
}

int main() {
  
  init();
  thread keyListener(keyListenerFunction);
  thread core(coreFunction);
  thread *p;
  keyListener.join();
  //core.join();
  //terminate();
  //delete p;
  core.detach();
  erase();
  delwin(mainwin);
  endwin();
  return 0;
}
