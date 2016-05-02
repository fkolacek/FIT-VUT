#ifndef CONFIG_H
#define CONFIG_H

#include "simlib.h"

//Pocatecni hodnoty simulace

//Zaciname v Listopadu
#define T_START (30*10*24) //30*10*24
//Koncime rok na to
#define T_END (T_START+ 30*12*24) //30*12*24
//Simulujeme kazdou hodinu
#define T_STEP 1.0

//Pracovni doba
#define T_DAY_START 4
#define T_DAY_END 20

// Vzdalenost potravy v km (0.0, 5.0>
#define D_WORKER_OUT 0.5

// Zapnout / vypnout rojeni
#define ENABLE_SWARM 1

// Pres zimu umira 20% az 33% populace
#define WINTER_RATE 20.0

//Pocatecny pocty vcel, vajicek, medu a pylu
#define S_COUNT_MOTHER 1
#define S_COUNT_WORKERS_CLEANER 3500
#define S_COUNT_WORKERS_FEEDER 3500
#define S_COUNT_WORKERS_BUILDER 3500
#define S_COUNT_WORKERS_GUARD 3500
#define S_COUNT_WORKERS_OUT 4000
#define S_COUNT_DRONE 0
#define S_COUNTER_EGGS 0
// 15-23kg medu potrebuje vcelstvo na zimu
#define S_COUNTER_HONEY (18* 1000 * 1000)

enum Role{
	WORKER_EGG,
	WORKER_MAGGOT,
	WORKER_CAWL,
	WORKER_CLEANER,
	WORKER_FEEDER,
	WORKER_BUILDER,
	WORKER_GUARD,
	WORKER_OUT,
	WORKER_OUT_SUMMER,
	WORKER_OUT_WINTER,
	
	MOTHER_EGG,
	MOTHER_MAGGOT,
	MOTHER_CAWL,
	MOTHER_WEDDING,	
	MOTHER,
	MOTHER_VIRGIN,
	
	DRONE_EGG,
	DRONE_MAGGOT,
	DRONE_CAWL,
	DRONE,
	DRONE_READY,
	
	ACCIDENTALLY_KILLED,
	FERTIZILED,
	
	END
};

enum Season{
	S_SPRING, S_SUMMER, S_AUTUMN, S_WINTER,
};

extern int COUNTER_EGGS; //ks
extern double COUNTER_HONEY; //mg
extern double COUNTER_CONSUMED; //mg
extern int COUNTERS[];
extern int COUNTERS_TMP[];
extern int TMP;

#endif
