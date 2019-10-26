#include <stdio.h>
#include <chrono>
#include <thread>
#include <string>
#include <cstring>
#include <ncurses.h>
#include <iostream>
#include "StatusFetcher.h"

using namespace std;

void printWaterMeter(int startX, int startY, int waterLevel);
void printMoistureMeter(int startX, int startY, int moistureLevel);
void printTimeLevel(string timeLevel);
void printTimestamp();

int main(int argc, char **argv)
{
	initscr();
    
  if(has_colors() == FALSE) {
    printf("No color support!\n");
    return 1;
  }

  start_color();
  curs_set(FALSE);
  
  init_pair(COLORS::WATER, COLOR_BLACK, COLOR_CYAN);
  init_pair(COLORS::MOISTURE, COLOR_BLACK, COLOR_GREEN);
  init_pair(COLORS::WHITE, COLOR_WHITE, COLOR_RED);
  init_pair(COLORS::NEUTRAL, COLOR_BLACK, COLOR_WHITE);
    
  int waitTimeMinutes = 1;
  int waitTimeMilliSeconds = (waitTimeMinutes * 60 * 1000);
    
  StatusFetcher *flower = new StatusFetcher();
  
  while(true) {
      
      if(flower->fetchStatus()) {
        printWaterMeter(10, 4, flower->getWaterLevel());
        printMoistureMeter(30, 4, flower->getMoistureLevel());
        printTimeLevel(flower->getTimeLevel());
        printTimestamp();
      }
      
      refresh();
      
      this_thread::sleep_for(chrono::milliseconds(waitTimeMilliSeconds));
  }
  
  endwin();
	return 0;
}

void printWaterMeter(int startX, int startY, int waterLevel) {
 
  move(startY, startX);
  
  int currentRow = startY;
  currentRow++;
  
  for(int i = 9; i >= 0; i--) {
    if(waterLevel > i) {
      attron(COLOR_PAIR(COLORS::WATER));
      addstr("-   ");
      attroff(COLOR_PAIR(COLORS::WATER));
    } else {
      attron(COLOR_PAIR(COLORS::WHITE));
      addstr("-   ");
      attroff(COLOR_PAIR(COLORS::WHITE));
    }
    move(currentRow++, startX);
  }
  move(currentRow++, startX - 3);
  addstr("Water level");
  
}

void printMoistureMeter(int startX, int startY, int moistureLevel) {
  move(startY, startX);
  
  int currentRow = startY;
  currentRow++;
  
  for(int i = 9; i >= 0; i--) {
    if(moistureLevel > i) {
      attron(COLOR_PAIR(COLORS::MOISTURE));
      addstr("-   ");
      attroff(COLOR_PAIR(COLORS::MOISTURE));
    } else {
      attron(COLOR_PAIR(COLORS::WHITE));
      addstr("-   ");
      attroff(COLOR_PAIR(COLORS::WHITE));
    }
    move(currentRow++, startX);
  }
  move(currentRow++, startX - 5);
  addstr("Moisture level");
}

void printTimeLevel(string timeLevel) {
  move(2,1);
  addstr("Device time: ");
  addstr(timeLevel.c_str());
}

void printTimestamp() {
    
  time_t rawtime;
  time(&rawtime);
  
  char *t = ctime(&rawtime);
  if (t[strlen(t)-1] == '\n') {
      t[strlen(t)-1] = '\0';
  }
  
  move(0,0);
  clrtoeol();
  move(1,1);
  addstr("Last update: ");
  addstr(t);
}

