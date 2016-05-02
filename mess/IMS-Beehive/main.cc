#include <iostream>
#include "simlib.h"
#include "time.h"
#include "config.h"
#include "scheduler.h"
#include "bee.h"

using namespace std;


void InitBeehive(){
	int i;

	for(i = 0; i < S_COUNT_MOTHER; i++){ SCHEDULER->Schedule(MOTHER); }
		
	for(i = 0; i < S_COUNT_WORKERS_CLEANER; i++) SCHEDULER->Schedule(WORKER_CLEANER);
	for(i = 0; i < S_COUNT_WORKERS_FEEDER; i++) SCHEDULER->Schedule(WORKER_FEEDER);
	for(i = 0; i < S_COUNT_WORKERS_BUILDER; i++) SCHEDULER->Schedule(WORKER_BUILDER);
	for(i = 0; i < S_COUNT_WORKERS_GUARD; i++) SCHEDULER->Schedule(WORKER_GUARD);
	for(i = 0; i < S_COUNT_WORKERS_OUT; i++) SCHEDULER->Schedule(WORKER_OUT);	
	for(i = 0; i < S_COUNT_DRONE; i++) SCHEDULER->Schedule(DRONE_EGG);
	
	int total = 0;
	for(int i = 0; i < END; i++) total += COUNTERS[i];
	double winterSurviveRatio = 1.0 - (double)WINTER_RATE / 100;
	TMP = (int)(total * winterSurviveRatio);
	
	cout << "Total: " << total << endl;
	cout << "Ratio: " << winterSurviveRatio << endl;
	cout << "Survive: " << TMP << endl;	

	cout << "Initialization done" << endl;
}

void PrintStatistics();

int main(int argc, char* argv[]){
	
	Init(T_START, T_END);
	
	RandomSeed(time(NULL));
	srand(time(NULL));
	
	SCHEDULER = new Scheduler();
	SCHEDULER->Activate();
	
	InitBeehive();	
	
	SetOutput("ims.output");
	
	Run();
	
	SIMLIB_statistics.Output();

	return 0;
}


