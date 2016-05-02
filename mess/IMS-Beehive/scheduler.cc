#include "scheduler.h"

Scheduler* SCHEDULER = 0;

string months[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

Scheduler::Scheduler(){
	this->time = Time;
	this->hour = (int)T_START % 24;
	this->day = (int)T_START / 24;
	this->month = this->day / 30;
	
	this->setSeason();
}

void Scheduler::Behavior(){
	this->time = Time;

	//Kazdy den ma 24 hodin
	if(this->hour == 24){ this->day++; this->hour = 0; }
	
	//Kazdy mesic ma 30 dni
	if(this->day % 30 == 0) this->month = (((int)day / 30)%12);
	
	//Kazdy mesic poslem report
	if((this->day % 30 == 0 || this->day % 15 == 0) && this->hour == 0) this->PrintStatistics();
	
	this->setSeason();

	cout << "Day " << this->day + 1 << " | " << (int)(this->day % 30)+1 << ". "<< months[this->month] <<" " << this->hour << ":00 (" << ((SCHEDULER->isDay())? "day)" : "night)") << ", model time: " << this->time << endl;
	
	this->hour++;	
	this->Activate(this->time + T_STEP);
}

void Scheduler::Schedule(Role role, double time){
	(new Bee(role))->Activate(this->time + time); 
	COUNTERS[role]++;
}

void Scheduler::setSeason(){
	if(this->month >= 2 && this->month <= 4) this->season = S_SPRING;
	else if(this->month >= 5 && this->month <= 7) this->season = S_SUMMER;
	else if(this->month >= 8 && this->month <= 10) this->season = S_AUTUMN;
	else this->season = S_WINTER;
}

void Scheduler::PrintStatistics(){
	ostringstream out;
	
	out << "+------------------------------- Statistics ------------------------------" << endl;
	out << "| Day " << this->day + 1 << " | " << (int)(this->day % 30)+1 << ". "<< months[this->month] <<" " << this->hour << ":00 (" << ((SCHEDULER->isDay())? "day)" : "night)") << ", model time: " << this->time << endl;
	out << "+------------------------------- Statistics ------------------------------" << endl;
	out << "| Honey in total: " << (COUNTER_HONEY / 1000 / 1000) << " kg, " << (COUNTER_HONEY / 1000) << " g ("<< COUNTER_HONEY << " mg )" << endl;
	out << "| Honey consumed in total: " << (COUNTER_CONSUMED / 1000 / 1000) << " kg, " << (COUNTER_CONSUMED / 1000) << " g (" << COUNTER_CONSUMED << " mg )" << endl;
	out << "| Eggs in total: " << COUNTER_EGGS << " pcs" << endl;
	out << "+------------------------------- Counts -----------------------------------" << endl;
	out << "| WORKER_EGG: " << COUNTERS[WORKER_EGG] << endl;
	out << "| WORKER_MAGGOT: " << COUNTERS[WORKER_MAGGOT] << endl;
	out << "| WORKER_CAWL: " << COUNTERS[WORKER_CAWL] << endl;
	out << "| WORKER_CLEANER: " << COUNTERS[WORKER_CLEANER] << endl;
	out << "| WORKER_FEEDER: " << COUNTERS[WORKER_FEEDER] << endl;
	out << "| WORKER_BUILDER: " << COUNTERS[WORKER_BUILDER] << endl;
	out << "| WORKER_GUARD: " << COUNTERS[WORKER_GUARD] << endl;
	out << "| WORKER_OUT: " << COUNTERS[WORKER_OUT] << endl;
	out << "| WORKER_OUT_SUMMER: " << COUNTERS[WORKER_OUT_SUMMER] << endl;
	out << "| WORKER_OUT_WINTER: " << COUNTERS[WORKER_OUT_WINTER] << endl;
	out << "| MOTHER_EGG: " << COUNTERS[MOTHER_EGG] << endl;
	out << "| MOTHER_MAGGOT: " << COUNTERS[MOTHER_MAGGOT] << endl;
	out << "| MOTHER_CAWL: " << COUNTERS[MOTHER_CAWL] << endl;
	out << "| MOTHER_WEDDING: " << COUNTERS[MOTHER_WEDDING] << endl;
	out << "| MOTHER: " << COUNTERS[MOTHER] << endl;
	out << "| MOTHER_VIRGIN: " << COUNTERS[MOTHER_VIRGIN] << endl;
	out << "| DRONE_EGG: " << COUNTERS[DRONE_EGG] << endl;
	out << "| DRONE_MAGGOT: " << COUNTERS[DRONE_MAGGOT] << endl;
	out << "| DRONE_CAWL: " << COUNTERS[DRONE_CAWL] << endl;
	out << "| DRONE: " << COUNTERS[DRONE] << endl;
	out << "| DRONE_READY: " << COUNTERS[DRONE_READY] << endl;
	out << "| ACCIDENTALLY_KILLED: " << COUNTERS[ACCIDENTALLY_KILLED] << endl;
	out << "+------------------------------- Sums -----------------------------------" << endl;
	out << "| WORKERS: " << COUNTERS[WORKER_CLEANER] + COUNTERS[WORKER_FEEDER] + COUNTERS[WORKER_BUILDER] + COUNTERS[WORKER_GUARD] + COUNTERS[WORKER_OUT] + COUNTERS[WORKER_OUT_SUMMER] + COUNTERS[WORKER_OUT_WINTER] << endl;
	out << "| DRONES: " << COUNTERS[DRONE] + COUNTERS[DRONE_READY] << endl;
	out << "+------------------------------- Parsable -----------------------------------" << endl;
	out << "CSV;";
	out << (int)(this->day % 30)+1 << "."<< this->month+1 << ".;";
	out << (COUNTER_HONEY / 1000 / 1000) << ";";
	out << (COUNTER_CONSUMED / 1000 / 1000) << ";";
	out << (COUNTER_EGGS) << ";";
	out << COUNTERS[WORKER_EGG] << ";";
	out << COUNTERS[WORKER_MAGGOT] << ";";
	out << COUNTERS[WORKER_CAWL] << ";";
	out << COUNTERS[WORKER_CLEANER] << ";";
	out << COUNTERS[WORKER_FEEDER] << ";";
	out << COUNTERS[WORKER_BUILDER] << ";";
	out << COUNTERS[WORKER_GUARD] << ";";
	out << COUNTERS[WORKER_OUT] << ";";
	out << COUNTERS[WORKER_OUT_SUMMER] << ";";
	out << COUNTERS[WORKER_OUT_WINTER] << ";";
	
	out << COUNTERS[MOTHER_EGG] << ";";
	out << COUNTERS[MOTHER_MAGGOT] << ";";
	out << COUNTERS[MOTHER_CAWL] << ";";
	out << COUNTERS[MOTHER_WEDDING] << ";";
	out << COUNTERS[MOTHER] << ";";
	out << COUNTERS[MOTHER_VIRGIN] << ";";
	out << COUNTERS[DRONE_EGG] << ";";
	out << COUNTERS[DRONE_MAGGOT] << ";";
	out << COUNTERS[DRONE_CAWL] << ";";
	out << COUNTERS[DRONE] << ";";
	out << COUNTERS[DRONE_READY] << ";";
	out << COUNTERS[ACCIDENTALLY_KILLED] << ";";
	out << COUNTERS[WORKER_CLEANER] + COUNTERS[WORKER_FEEDER] + COUNTERS[WORKER_BUILDER] + COUNTERS[WORKER_GUARD] + COUNTERS[WORKER_OUT] + COUNTERS[WORKER_OUT_SUMMER] + COUNTERS[WORKER_OUT_WINTER] << ";";
	
	out << endl;
	out << "+------------------------------- End ---------------------------------------" << endl << endl << endl;

	const string tmp = out.str();

	Print(tmp.c_str());
	
}
