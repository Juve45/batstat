#include <bits/stdc++.h>
#include <unistd.h>
#include <curses.h>
#include <string>
#include <dirent.h>

#define PATH "/sys/class/power_supply/"
#define LOG_P 20
using namespace std;

int refreshRate = 3;
//const string logFile = "log.txt";

float maxEnergy, currentPower, currentEnergy, initEnergy;
int initTime, percent, timeNow, timeElapsed = -refreshRate;
int lastTime = 0, logIndex = 0;
string Path = PATH, status;
bool quit = false;
WINDOW * mainwin;
vector <string> logCache;

void init()
{
	DIR* dir = opendir("/sys/class/power_supply/BAT0");
	if (dir)
	{
	   Path += "BAT0/";
	}
	else Path += "BAT1/";

  if ( (mainwin = initscr()) == NULL ) {
	  fprintf(stderr, "Error initialising ncurses.\n");
	  exit(EXIT_FAILURE);
  }
  keypad(stdscr, TRUE);
	
	if(has_colors() == FALSE)
	{	
		printf("Your terminal does not support color\n");
	}
	start_color();

  assume_default_colors(COLOR_WHITE,COLOR_BLACK);

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);

	bkgd(COLOR_PAIR(1));

	ifstream enNow(Path+"energy_now");
	enNow >> initEnergy;
	
	initTime = time(NULL);
	
	ifstream maxEnergyFile(Path+"energy_full");
	maxEnergyFile >> maxEnergy;
}

void refreshValues()
{
	ifstream powNow(Path+"power_now");
	ifstream enNow(Path+"energy_now");
	ifstream st(Path+"status");
	powNow >> currentPower;
	enNow >> currentEnergy;
	timeNow = time(NULL);
	//timeElapsed = timeElapsed + timeNow - lastTime;
	timeElapsed += refreshRate;
	lastTime = timeNow;
	st >> status;
}

void print()
{
	system("clear");
	printf("%-30s%s\n", "Status: ", status.c_str());
	printf("%-30s%.2lf Wh\n", "Max energy:", maxEnergy/1000000);
	printf("%-30s%.2lf Wh\n", "Energy left:", currentEnergy/1000000);
	printf("%-30s%.2lf W\n", "Power Consumption:", currentPower/1000000);
	printf("%-30s%.2lf\%\n", "Percentage left:", currentEnergy/maxEnergy*100);
	printf("%-30s%2d:%2d:%2d since %.2lf\%\n", "Time elapsed:", timeElapsed/3600, (timeElapsed/60)%60, timeElapsed%60, initEnergy/maxEnergy*100);
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
	sprintf(buff, "%-30s%.2lf Wh\n", "Max energy:", maxEnergy/1000000);
	mvaddstr(1, 0, buff);
	sprintf(buff, "%-30s%.2lf Wh\n", "Energy left:", currentEnergy/1000000);
	mvaddstr(2, 0, buff);
	sprintf(buff, "%-30s%.2lf W\n", "Power Consumption:", currentPower/1000000);
	mvaddstr(3, 0, buff);
	sprintf(buff, "%-30s%.2lf%%\n", "Percentage left:", currentEnergy/maxEnergy*100);
	mvaddstr(4, 0, buff);
	sprintf(buff, "%-30s%.2lf W\n", "Average power Consumption:", (initEnergy - currentEnergy) / 1000000 / (1. * timeElapsed / 3600.));
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
        if ( logIndex > 0 )
        --logIndex;
      	newPrint();
        break;

    case KEY_DOWN:
        if ( logIndex + 1 < logCache.size() )
        ++logIndex;
    		newPrint();
        break;
    }
	}
	quit = 1;
	return;
}

void coreFunction()
{
	lastTime = time(NULL);
	// sleep(1);
	while(!quit)
	{
		refreshValues();
		newPrint();
		if(timeElapsed % LOG_P == 0)
			addLog();
		sleep(refreshRate);
	}
}

int main()
{
	
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