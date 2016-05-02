#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <sstream>
#include <string>
#include "config.h"
#include "bee.h"

using namespace std;

extern string months[];

class Bee;

class Scheduler : public Event{
	public:
		Scheduler();		
		void Behavior();
		void Schedule(Role, double = T_STEP);
		void PrintStatistics();
		
		//Gettery
		inline int getTime(){ return this->time; }
		inline int getHour(){ return this->hour; }
		inline int getDay(){ return this->day; }
		inline int getMonth(){ return this->month; }
		inline Season getSeason(){ return this->season; }
		
		inline bool isDay(){ return (this->hour >= T_DAY_START && this->hour <= T_DAY_END); }
		//Aktivni jsou od brezna do rijna
		inline bool isActive(){ return (this->month >= 2 && this-> month <= 9); }
	private:
		void setSeason();
	
		int time;
		int hour;
		int day;
		int month;
		Season season;

};

extern Scheduler* SCHEDULER;

#endif
